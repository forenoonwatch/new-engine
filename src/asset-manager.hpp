#pragma once

#include <memory>

#include "rendering/shader.hpp"
#include "rendering/vertex-array.hpp"
#include "rendering/font.hpp"
#include "rendering/cube-map.hpp"

#include "animation/rig.hpp"
#include "animation/animation.hpp"

class AssetManager {
	public:
		AssetManager(RenderDevice& device);

		void load(const String& fileName);

		inline Texture& getTexture(const String& name) { return *textures[name]; }
		inline CubeMap& getCubeMap(const String& name) { return *cubeMaps[name]; }
		inline Material& getMaterial(const String& name) { return *materials[name]; }
		inline Font& getFont(const String& name) { return *fonts[name]; }
		
		inline IndexedModel& getModel(const String& name) { return models[name]; }
		inline VertexArray& getVertexArray(const String& name) { return *vertexArrays[name]; }
		inline Rig& getRig(const String& name) { return *rigs[name]; }
		inline Animation& getAnimation(const String& name) { return *animations[name]; }
		
		inline Shader& getShader(const String& name) { return *shaders[name]; }

		virtual ~AssetManager();
	private:
		RenderDevice& device;

		HashMap<String, IndexedModel> models;
		HashMap<String, std::shared_ptr<VertexArray>> vertexArrays;

		HashMap<String, std::shared_ptr<Texture>> textures;
		HashMap<String, std::shared_ptr<CubeMap>> cubeMaps;
		HashMap<String, std::shared_ptr<Material>> materials;
		
		HashMap<String, std::shared_ptr<Rig>> rigs;
		HashMap<String, std::shared_ptr<Animation>> animations;

		HashMap<String, std::shared_ptr<Shader>> shaders;
		HashMap<String, std::shared_ptr<Font>> fonts;

		void loadMesh(const Array<String>& tokens);
		void loadTexture(const Array<String>& tokens);
		void loadCubemap(const Array<String>& tokens);
		void loadFont(const Array<String>& tokens, FontLoader& fontLoader);
};
