#pragma once

#include "asset-manager.hpp"

#include "rendering/render-context.hpp"
#include "math/transform.hpp"
#include "containers/tree-map.hpp"
#include "animation/rig.hpp"

#include "rendering/font.hpp"
#include "rendering/cube-map.hpp"

#include "util-components.hpp"
#include "camera.hpp"

// TODO: replace Texture with Material
// TODO: add proper sorting and draw tag command whatever things
// http://realtimecollisiondetection.net/blog/?p=86
class GameRenderContext : public RenderContext {
	public:
		GameRenderContext(RenderDevice& device, RenderTarget& target,
				RenderDevice::DrawParams& drawParams, AssetManager& assetManager,
				const Matrix& perspective);

		void renderMesh(VertexArray& vertexArray, Material& material, const Matrix& transform);
		void renderSkinnedMesh(VertexArray& vertexArray, Material& material,
				const Matrix& transform, Rig& rig);
		void renderText(Font& font, const String& text,
				const Vector3f& color, float x, float y);

		void flush();

		inline void setSkybox(CubeMap* skybox) { this->skybox = skybox; }

		inline Camera& getCamera() { return camera; }
	private:
		struct SkinnedMesh {
			inline SkinnedMesh(const Matrix& mvp,
				const Matrix& modelMatrix, Rig* rig)
					: rig(rig) {
				transforms[0] = mvp;
				transforms[1] = modelMatrix;
			}

			Rig* rig;
			Matrix transforms[2];
		};

		struct Text {
			Array<Vector3f> positions;
			Array<Vector3f> scales;
			Vector3f color;
		};

		RenderDevice::DrawParams& drawParams;
		
		Shader& staticMeshShader;
		Shader& skinnedMeshShader;
		Shader& fontShader;
		Shader& skyboxShader;

		Sampler mipmapSampler;
		Sampler linearSampler;

		Camera camera;
		Matrix screenProjection;
		
		VertexArray& textQuad;
		VertexArray& skyboxMesh;

		UniformBuffer dataBuffer;

		CubeMap* skybox;

		TreeMap<Pair<VertexArray*, Material*>, Array<Matrix>> meshRenderBuffer;
		TreeMap<Pair<VertexArray*, Material*>, Array<SkinnedMesh>> skinnedMeshRenderBuffer;
		TreeMap<Font*, Text> textRenderBuffer;
};

