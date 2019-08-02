#pragma once

#include "asset-manager.hpp"

#include "rendering/render-context.hpp"
#include "math/transform.hpp"
#include "containers/tree-map.hpp"
#include "animation/rig.hpp"

#include "rendering/font.hpp"
#include "rendering/cube-map.hpp"

#include "camera.hpp"
#include "particles.hpp"

class Particle;
class ParticleEmitter;

// TODO: replace Texture with Material DONE
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
		void renderParticleEmitter(FeedbackBuffer& particleBuffer, Texture& texture);

		void flush();

		inline void setSkybox(CubeMap* skybox) { this->skybox = skybox; }
		inline void setOcean(VertexArray* ocean, Texture* oceanNormal, Texture* oceanDUDV) {
			this->ocean = ocean;
			this->oceanNormal = oceanNormal;
			this->oceanDUDV = oceanDUDV;
		}

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

		void flushStaticMeshes();
		void flushSkinnedMeshes();
		void flushText();
		void flushParticles(); // TODO: differentiate between opaque/transparent

		RenderDevice::DrawParams& drawParams;
		
		Shader& staticMeshShader;
		Shader& skinnedMeshShader;
		Shader& fontShader;
		Shader& skyboxShader;
		Shader& oceanShader;
		Shader& staticMirrorMeshShader;
		Shader& textureShader;
		Shader& particleShader;
		Shader& billboardShader;

		Sampler mipmapSampler;
		Sampler linearSampler;
		Sampler repeatSampler;

		Texture reflectionTexture;
		RenderTarget reflectionTarget;
		RenderContext reflectionContext;

		Texture refractionTexture;
		RenderTarget refractionTarget;
		RenderContext refractionContext;

		Camera camera;
		Matrix screenProjection;
		
		VertexArray& textQuad;
		VertexArray& skyboxMesh;

		UniformBuffer dataBuffer;
		UniformBuffer animBuffer;
		UniformBuffer fontBuffer;
		UniformBuffer particleBuffer;

		CubeMap* skybox;
		VertexArray* ocean;
		Texture* oceanNormal;
		Texture* oceanDUDV;

		TreeMap<Pair<VertexArray*, Material*>, Array<Matrix>> meshRenderBuffer;
		TreeMap<Pair<VertexArray*, Material*>, Array<SkinnedMesh>> skinnedMeshRenderBuffer;
		TreeMap<Pair<VertexArray*, Material*>, Array<Matrix>> mirrorMeshRenderBuffer;
		TreeMap<Font*, Text> textRenderBuffer;
		TreeMap<Texture*, Array<FeedbackBuffer*>> particleRenderBuffer;
};

#include "game-render-context.inl"
