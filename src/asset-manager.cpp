#include "asset-manager.hpp"

#include <fstream>

#include "rendering/model-loader.hpp"

AssetManager::AssetManager(RenderDevice& device)
		: device(device) {
	StringStream shaderText;
	String shaderFiles[] = {"./res/shaders/basic-shader.glsl",
			"./res/shaders/skinned-shader.glsl", "./res/shaders/font-shader.glsl",
			"./res/shaders/skybox-shader.glsl", "./res/shaders/ocean-shader.glsl"};
	String shaderNames[] = {"basic-shader", "skinned-shader", "font-shader",
			"skybox-shader", "ocean-shader"};

	for (uint32 i = 0; i < ARRAY_SIZE_IN_ELEMENTS(shaderFiles); ++i) {
		shaderText.str("");
		StringUtil::loadTextFileWithIncludes(shaderText, shaderFiles[i], "#include");
		shaders.insert(std::make_pair(shaderNames[i],
				std::make_shared<Shader>(device, shaderText.str()) ) );
	}

	IndexedModel quad;
	quad.initQuad();

	Array<IndexedModel> loadedModels;
	ModelLoader::loadOnlyModels("./res/models/flipped-cube.fbx", loadedModels);
	
	vertexArrays.insert(std::make_pair("text-quad",
			std::make_shared<VertexArray>(device, quad, RenderDevice::USAGE_STATIC_DRAW) ) );
	vertexArrays.insert(std::make_pair("skybox-mesh",
			std::make_shared<VertexArray>(device, loadedModels[0], RenderDevice::USAGE_STATIC_DRAW) ) );
}

void AssetManager::load(const String& fileName) {
	std::ifstream file;
	file.open(fileName);

	String line;
	Array<String> tokens;

	FontLoader fontLoader;

	Array<Array<String>> materialPrototypes;

	if (file.is_open()) {
		while (file.good()) {
			std::getline(file, line);
			tokens.clear();
			StringUtil::whitespaceSplit(tokens, line);

			if (tokens.size() < 3) {
				continue;
			}

			if (tokens[0].compare("MESH") == 0) {
				loadMesh(tokens);
			}
			else if (tokens[0].compare("TEXTURE") == 0) {
				loadTexture(tokens);
			}
			else if (tokens[0].compare("MATERIAL") == 0) {
				Array<String> materialPrototype;

				for (uint32 i = 1; i < tokens.size(); ++i) {
					materialPrototype.push_back(tokens[i]);
				}

				materialPrototypes.push_back(materialPrototype);
			}
			else if (tokens[0].compare("FONT") == 0) {
				loadFont(tokens, fontLoader);
			}
			else if (tokens[0].compare("CUBEMAP") == 0) {
				loadCubemap(tokens);
			}
		}
	}
	else {
		DEBUG_LOG(LOG_ERROR, LOG_TYPE_IO, "Failed to open scene file: %s",
				fileName.c_str());
	}

	file.close();

	for (int i = 0; i < materialPrototypes.size(); ++i) {
		Array<String>& prot = materialPrototypes[i];
		materials[prot[0]] = std::make_shared<Material>(*textures[prot[1]], *textures[prot[2]]);
	}
}

void AssetManager::loadMesh(const Array<String>& tokens) {
	Array<IndexedModel> indexedModels;
	Array<uint32> modelMaterialIndices;
	Array<MaterialSpec> modelMaterials;
	Array<Rig> loadedRigs;
	Array<Animation> loadedAnimations;

	if (tokens.size() % 2 != 0) {
		DEBUG_LOG(LOG_ERROR, LOG_TYPE_IO, "Invalid number of tokens to load mesh %s",
				tokens[1].c_str());
		return;
	}

	ModelLoader::loadModels(tokens[1].substr(1, tokens[1].length() - 2), indexedModels,
			modelMaterialIndices, modelMaterials, loadedRigs, loadedAnimations);

	uint32 arrayIndex = 0;
	uint32 modelIndex = 0;
	uint32 animationIndex = 0;
	uint32 rigIndex = 0;

	for (uint32 i = 2; i < tokens.size(); i += 2) {
		if (tokens[i].compare("ARRAY") == 0) {
			vertexArrays[tokens[i + 1]] = std::make_shared<VertexArray>(device,
					indexedModels[arrayIndex++], RenderDevice::USAGE_STATIC_DRAW);
		}
		else if (tokens[i].compare("MODEL") == 0) {
			models[tokens[i + 1]] = indexedModels[modelIndex++];
		}
		else if (tokens[i].compare("ANIMATION") == 0) {
			animations[tokens[i + 1]] = std::make_shared<Animation>(loadedAnimations[animationIndex++]);
		}
		else if (tokens[i].compare("RIG") == 0) {
			rigs[tokens[i + 1]] = std::make_shared<Rig>(loadedRigs[rigIndex++]);
		}
	}
}

void AssetManager::loadTexture(const Array<String>& tokens) {
	if (StringUtil::toLower(StringUtil::getFileExtension(tokens[2])).compare("dds")
			== 0) {
		DDSTexture ddsTexture;

		if (!ddsTexture.load(tokens[2].substr(1, tokens[2].length() - 2).c_str())) {
			DEBUG_LOG(LOG_ERROR, LOG_TYPE_IO, "Failed to load DDS Texture: %s", tokens[2].c_str());
			return;
		}

		textures[tokens[1]] = std::make_shared<Texture>(device, ddsTexture);
	}
	else {
		ArrayBitmap bmp;
		
		if (!bmp.load(tokens[2].substr(1, tokens[2].length() - 2).c_str())) {
			DEBUG_LOG(LOG_ERROR, LOG_TYPE_IO, "Failed to load Texture: %s", tokens[2].c_str());
			return;
		}

		textures.insert( std::make_pair(tokens[1], std::make_shared<Texture>(device, bmp,
				RenderDevice::FORMAT_RGB, false, false)) );
	}
}

void AssetManager::loadFont(const Array<String>& tokens, FontLoader& fontLoader) {
	Array<Font> loadedFonts;
	fontLoader.loadFont(tokens[2].substr(1, tokens[2].length() - 2).c_str(), std::stoi(tokens[3]), loadedFonts);

	textures[tokens[1] + "_Font"] = std::make_shared<Texture>(device, loadedFonts[0]);

	loadedFonts[0].setTextureID(textures[tokens[1] + "_Font"]->getId());
	fonts[tokens[1]] = std::make_shared<Font>(loadedFonts[0]);
}

void AssetManager::loadCubemap(const Array<String>& tokens) {
	if (tokens.size() != 8) {
		DEBUG_LOG(LOG_ERROR, LOG_TYPE_IO, "Invalid number of tokens for cubemap %s",
				tokens[1].c_str());
		return;
	}

	ArrayBitmap bitmaps[6];

	for (uint32 i = 0; i < 6; ++i) {
		if (!bitmaps[i].load(tokens[i + 2].substr(1, tokens[i + 2].length() - 2))) {
			DEBUG_LOG(LOG_ERROR, LOG_TYPE_IO, "Failed to load texture %s for cubemap %s",
					tokens[i + 2].c_str(), tokens[1].c_str());
			return;
		}
	}

	cubeMaps[tokens[1]] = std::make_shared<CubeMap>(device, bitmaps);
}

AssetManager::~AssetManager() {
}
