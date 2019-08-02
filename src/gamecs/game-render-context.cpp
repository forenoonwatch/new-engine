#include "game-render-context.hpp"

#include "core/timing.hpp"

static constexpr size_t DATA_BUFFER_SIZE = /*sizeof(Matrix) * Rig::MAX_JOINTS 
		+ */sizeof(float) + 3 * (3 * sizeof(float)) + sizeof(float) + 3 * sizeof(float);
static constexpr size_t ANIM_BUFFER_SIZE = sizeof(Matrix) * Rig::MAX_JOINTS;
static constexpr size_t FONT_BUFFER_SIZE = sizeof(Matrix) + 3 * sizeof(float);
static constexpr size_t PARTICLE_BUFFER_SIZE = sizeof(Matrix) + 3 * sizeof(float);

static constexpr size_t CAMERA_POS_OFFSET = 8 * sizeof(float);
static constexpr size_t TIME_OFFSET = 11 * sizeof(float);
static constexpr size_t FOG_OFFSET = TIME_OFFSET + sizeof(float);

static void color(uint32 rgb, float* out) {
	out[0] = (float)((rgb >> 16) & 0xFF) / 255.f;
	out[1] = (float)((rgb >> 8) & 0xFF) / 255.f;
	out[2] = (float)(rgb & 0xFF) / 255.f;
}

GameRenderContext::GameRenderContext(RenderDevice& device, RenderTarget& target,
	RenderDevice::DrawParams& drawParams, AssetManager& assetManager, const Matrix& perspective)
			: RenderContext(device, target)
			, drawParams(drawParams)
			, staticMeshShader(assetManager.getShader("basic-shader"))
			, skinnedMeshShader(assetManager.getShader("skinned-shader"))
			, fontShader(assetManager.getShader("font-shader"))
			, skyboxShader(assetManager.getShader("skybox-shader"))
			, oceanShader(assetManager.getShader("ocean-shader"))
			, staticMirrorMeshShader(assetManager.getShader("static-mirror-shader"))
			, textureShader(assetManager.getShader("texture-shader"))
			, particleShader(assetManager.getShader("particle-shader"))
			, billboardShader(assetManager.getShader("billboard-shader"))
			, mipmapSampler(device, RenderDevice::FILTER_NEAREST_MIPMAP_NEAREST)
			, linearSampler(device, RenderDevice::FILTER_LINEAR)
			, repeatSampler(device, RenderDevice::FILTER_LINEAR,
					RenderDevice::FILTER_LINEAR, RenderDevice::WRAP_REPEAT,
					RenderDevice::WRAP_REPEAT)
			, reflectionTexture(device, RenderDevice::FORMAT_RGB, 400, 300)
			, reflectionTarget(device, reflectionTexture,
					RenderDevice::ATTACHMENT_COLOR)
			, reflectionContext(device, reflectionTarget)
			, refractionTexture(device, RenderDevice::FORMAT_RGB, 400, 300)
			, refractionTarget(device, refractionTexture)
			, refractionContext(device, refractionTarget)
			, camera(perspective)
			, screenProjection(Matrix::ortho(0.f, 800.f, 0.f, 600.f, 1.f, -1.f))
			, textQuad(assetManager.getVertexArray("text-quad"))
			, skyboxMesh(assetManager.getVertexArray("skybox-mesh"))
			, skybox(nullptr)
			, ocean(nullptr)
			, oceanNormal(nullptr)
			, oceanDUDV(nullptr)
			, dataBuffer(device, DATA_BUFFER_SIZE, RenderDevice::USAGE_DYNAMIC_DRAW)
			, animBuffer(device, ANIM_BUFFER_SIZE, RenderDevice::USAGE_DYNAMIC_DRAW)
			, fontBuffer(device, FONT_BUFFER_SIZE, RenderDevice::USAGE_DYNAMIC_DRAW)
			, particleBuffer(device, PARTICLE_BUFFER_SIZE, RenderDevice::USAGE_DYNAMIC_DRAW) {
	//staticMeshShader.setBufferBlock("ShaderData", 0);
	staticMeshShader.setUniformBuffer("ShaderData", dataBuffer, 0);
	skinnedMeshShader.setUniformBuffer("ShaderData", dataBuffer, 0);
	skyboxShader.setUniformBuffer("ShaderData", dataBuffer, 0);
	staticMirrorMeshShader.setUniformBuffer("ShaderData", dataBuffer, 0);

	skinnedMeshShader.setBufferBlock("AnimationData", 1);
	skinnedMeshShader.setUniformBuffer("AnimationData", animBuffer, 1);

	fontShader.setBufferBlock("ShaderData", 1);
	fontShader.setUniformBuffer("ShaderData", fontBuffer, 1);

	textureShader.setBufferBlock("ShaderData", 1);
	textureShader.setUniformBuffer("ShaderData", fontBuffer, 1);

	particleShader.setBufferBlock("ShaderData", 1);
	particleShader.setUniformBuffer("ShaderData", particleBuffer, 1);

	billboardShader.setBufferBlock("ShaderData", 1);
	billboardShader.setUniformBuffer("ShaderData", particleBuffer, 1);

	float data[] = {0.f, 50.f, -100.f, 0.4f, 1.f, 1.f, 1.f};
	//color(0xFAF7D9, &data[4]);
	color(0xFFFFFF, &data[4]);
	
	dataBuffer.update(data, 0, sizeof(data));

	data[0] = 0.5f;
	data[1] = 0.5f;
	data[2] = 0.5f;
	dataBuffer.update(data, FOG_OFFSET, 3 * sizeof(float));
}

void GameRenderContext::renderText(Font& font, const String& text,
		const Vector3f& color, float x, float y) {
	Text& textData = textRenderBuffer[&font];

	textData.color = color;

	for (uint32 i = 0; i < text.length(); ++i) {
		const Font::Character& ch = font.getCharacter(text[i]);

		float xPos = x + ch.bearingX;
		float yPos = y - ch.sizeY + ch.bearingY;

		x += ch.advance;

		textData.positions.emplace_back(xPos, yPos, ch.texCoordData[0], ch.texCoordData[1]);
		textData.scales.emplace_back((float)ch.sizeX, (float)ch.sizeY,
				ch.texCoordData[2], ch.texCoordData[3]);
	}
}

void GameRenderContext::flush() {
	dataBuffer.update(&camera.position, CAMERA_POS_OFFSET, 3 * sizeof(float));

	drawParams.sourceBlend = RenderDevice::BLEND_FUNC_NONE;
	drawParams.destBlend = RenderDevice::BLEND_FUNC_NONE;

	float t = (float)Time::getTime();
	dataBuffer.update(&t, TIME_OFFSET, sizeof(float));

	reflectionContext.clear(true);
	refractionContext.clear(true);

	// draw reflection and refraction skyboxes
	if (ocean != nullptr && skybox != nullptr) {
		skyboxShader.setSampler("cubemap", skybox->getId(), linearSampler, 0,
				RenderDevice::TEXTURE_TYPE_CUBE_MAP);
		Matrix m = camera.reflectMVP * Matrix::translate(camera.position
				* Vector3f(1.f, -1.f, 1.f));

		skyboxMesh.updateBuffer(4, &m, sizeof(Matrix));
		reflectionContext.draw(skyboxShader, skyboxMesh, drawParams, 1);

		m = camera.viewProjection * Matrix::translate(camera.position);
		skyboxMesh.updateBuffer(4, &m, sizeof(Matrix));
		refractionContext.draw(skyboxShader, skyboxMesh, drawParams, 1);
	}

	// draw static meshes
	getDevice().setClipEnabled(true);

	flushStaticMeshes();

	getDevice().setClipEnabled(false);

	// draw ocean
	if (ocean != nullptr) {
		Matrix mt = Matrix::translate(Vector3f(camera.position[0], 0.f, camera.position[2]));
		Matrix ms[] = {camera.viewProjection * mt, mt};
		ocean->updateBuffer(3, ms, 2 * sizeof(Matrix));

		oceanShader.setSampler("reflection", reflectionTexture, repeatSampler, 0);
		oceanShader.setSampler("refraction", refractionTexture, repeatSampler, 1);
		//oceanShader.setSampler("normalMap", *oceanNormal, repeatSampler, 2);
		oceanShader.setSampler("dudvMap", *oceanDUDV, repeatSampler, 2);
		draw(oceanShader, *ocean, drawParams, 1);
	}

	// draw rigged meshes
	flushSkinnedMeshes();

	// draw skybox
	if (skybox != nullptr) {
		skyboxShader.setSampler("cubemap", skybox->getId(), linearSampler, 0,
				RenderDevice::TEXTURE_TYPE_CUBE_MAP);
		Matrix m = camera.viewProjection * Matrix::translate(camera.position);

		skyboxMesh.updateBuffer(4, &m, sizeof(Matrix));
		draw(skyboxShader, skyboxMesh, drawParams, 1);
	}

	// draw particles
	flushParticles();

	// draw text
	fontBuffer.update(&screenProjection, sizeof(Matrix));

	drawParams.sourceBlend = RenderDevice::BLEND_FUNC_ONE;
	drawParams.destBlend = RenderDevice::BLEND_FUNC_ONE;

	flushText();
}

inline void GameRenderContext::flushStaticMeshes() {
	Material* currentMaterial = nullptr;
	Material* material;

	VertexArray* vertexArray;
	size_t numTransforms;

	for (auto it = std::begin(meshRenderBuffer), end = std::end(meshRenderBuffer); it != end; ++it) {
		numTransforms = it->second.size();

		if (numTransforms == 0) {
			continue;
		}

		vertexArray = it->first.first;
		material = it->first.second;

		if (material != currentMaterial) {
			currentMaterial = material;
			staticMeshShader.setMaterial(*material, mipmapSampler);
		}

		vertexArray->updateBuffer(4, &it->second[0], sizeof(Matrix) * numTransforms);
		
		draw(staticMeshShader, *vertexArray, drawParams, numTransforms);

		it->second.clear();
	}

	for (auto it = std::begin(mirrorMeshRenderBuffer), end = std::end(mirrorMeshRenderBuffer);
			it != end; ++it) {
		numTransforms = it->second.size();

		if (numTransforms == 0) {
			continue;
		}

		vertexArray = it->first.first;
		material = it->first.second;

		if (material != currentMaterial) {
			currentMaterial = material;
			staticMeshShader.setMaterial(*material, mipmapSampler);
		}

		vertexArray->updateBuffer(4, &it->second[0], sizeof(Matrix) * numTransforms);
		
		reflectionContext.draw(staticMirrorMeshShader, *vertexArray, drawParams, numTransforms);

		it->second.clear();
	}
}

inline void GameRenderContext::flushSkinnedMeshes() {
	Material* currentMaterial = nullptr;
	Material* material;

	VertexArray* vertexArray;
	size_t numTransforms;

	for (auto it = std::begin(skinnedMeshRenderBuffer),
			end = std::end(skinnedMeshRenderBuffer); it != end; ++it) {
		numTransforms = it->second.size();

		if (numTransforms == 0) {
			continue;
		}

		vertexArray = it->first.first;
		material = it->first.second;

		if (material != currentMaterial) {
			currentMaterial = material;
			skinnedMeshShader.setMaterial(*material, mipmapSampler);
		}

		for (auto matRig = std::begin(it->second), mrEnd = std::end(it->second);
				matRig != mrEnd; ++matRig) {
			animBuffer.update(matRig->rig->getTransformSet(), sizeof(Matrix) * Rig::MAX_JOINTS);
			vertexArray->updateBuffer(6, matRig->transforms, 2 * sizeof(Matrix));
			draw(skinnedMeshShader, *vertexArray, drawParams, numTransforms);
		}
		
		it->second.clear();
	}
}

inline void GameRenderContext::flushText() {
	Font* currentFont = nullptr;
	Font* font;

	size_t numTransforms;

	for (auto it = std::begin(textRenderBuffer), end = std::end(textRenderBuffer); it != end; ++it) {
		numTransforms = it->second.positions.size();

		if (numTransforms == 0) {
			continue;
		}
		
		fontBuffer.update(&it->second.color, sizeof(Matrix), 3 * sizeof(float));

		font = it->first;

		if (font != currentFont) {
			currentFont = font;
			fontShader.setSampler("text", font->getTextureID(), linearSampler, 0);
		}

		textQuad.updateBuffer(1, &it->second.positions[0], 4 * sizeof(float) * numTransforms);
		textQuad.updateBuffer(2, &it->second.scales[0], 4 * sizeof(float) * numTransforms);
		draw(fontShader, textQuad, drawParams, numTransforms);

		it->second.positions.clear();
		it->second.scales.clear();
	}
}

inline void GameRenderContext::flushParticles() {
	Texture* currentTexture = nullptr;
	Texture* texture;

	size_t numParticleEmitters;

	float dt = 1.f / 60.f;
	particleBuffer.update(&dt, sizeof(float));

	drawParams.sourceBlend = RenderDevice::BLEND_FUNC_SRC_ALPHA;
	drawParams.destBlend = RenderDevice::BLEND_FUNC_DST_ALPHA;

	getDevice().setRasterizerDiscardEnabled(true);

	for (auto it = std::begin(particleRenderBuffer), end = std::end(particleRenderBuffer);
			it != end; ++it) {
		numParticleEmitters = it->second.size();

		if (numParticleEmitters == 0) {
			continue;
		}

		for (uint32 i = 0; i < numParticleEmitters; ++i) {
			updateFeedbackBuffer(particleShader, *it->second[i]);
		}
	}

	getDevice().setRasterizerDiscardEnabled(false);
	getDevice().setDepthMaskEnabled(false);

	particleBuffer.update(&camera.viewProjection, sizeof(Matrix));
	particleBuffer.update(&camera.position, sizeof(Matrix), 3 * sizeof(float));

	for (auto it = std::begin(particleRenderBuffer), end = std::end(particleRenderBuffer);
			it != end; ++it) {
		numParticleEmitters = it->second.size();

		if (numParticleEmitters == 0) {
			continue;
		}

		texture = it->first;

		if (texture != currentTexture) {
			currentTexture = texture;
			billboardShader.setSampler("billboard", texture->getId(), linearSampler, 1);
		}

		for (uint32 i = 0; i < numParticleEmitters; ++i) {
			draw(billboardShader, *it->second[i], drawParams);
		}

		it->second.clear();
	}

	getDevice().setDepthMaskEnabled(true);
}
