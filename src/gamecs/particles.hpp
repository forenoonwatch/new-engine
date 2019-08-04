#pragma once

#include "util-components.hpp"
#include "game-render-context.hpp"

class ParticleEmitter;
class GameRenderContext;

// analogue of the particle data in the particle shader
struct Particle {
	float type = 0.f; // default type for an Emitter
	float position[3];
	float velocity[3];
	float age = 0.f;
	float transScale[4];

	static constexpr uintptr TYPE_SIZE = sizeof(float);
	static constexpr uintptr POSITION_SIZE = 3 * sizeof(float);
	static constexpr uintptr VELOCITY_SIZE = 3 * sizeof(float);
	static constexpr uintptr AGE_SIZE = sizeof(float);
	static constexpr uintptr TRANSPARENCY_SIZE = 2 * sizeof(float);
	static constexpr uintptr SCALE_SIZE = 2 * sizeof(float);

	static constexpr uintptr TYPE_OFFSET = 0;
	static constexpr uintptr POSITION_OFFSET = TYPE_OFFSET + TYPE_SIZE;
	static constexpr uintptr VELOCITY_OFFSET = POSITION_OFFSET + POSITION_SIZE;
	static constexpr uintptr AGE_OFFSET = VELOCITY_OFFSET + VELOCITY_SIZE;
	static constexpr uintptr TRANSPARENCY_OFFSET = AGE_OFFSET + AGE_SIZE;
	static constexpr uintptr SCALE_OFFSET = TRANSPARENCY_OFFSET + TRANSPARENCY_SIZE;
} __attribute__((packed));

// contains feedback buffer and additional render data for each particle emitter
class ParticleEmitter {
	public:
		inline ParticleEmitter(RenderDevice& device, uint32 maxParticles,
				uintptr* attribSizes, uint32 numAttribs)
			: feedbackBuffer(device, maxParticles * sizeof(Particle), attribSizes,
					numAttribs, (const float*)&emitterData, sizeof(Particle)) {}

		inline void setPosition(float x, float y, float z);
		inline void setPosition(const Vector3f& position);
		
		inline void setVelocity(float x, float y, float z);
		inline void setVelocity(const Vector3f& velocity);

		inline void setTransparency(float start, float end);
		inline void setScale(float start, float end);

		inline Particle& getEmitterData() { return emitterData; }
		inline FeedbackBuffer& getBuffer() { return feedbackBuffer; }
	private:
		Particle emitterData;
		FeedbackBuffer feedbackBuffer;
};

class ParticleEmitterComponent : public ECSComponent<ParticleEmitterComponent> {
	public:
		ParticleEmitter* emitter;
		Texture* texture;
		Shader* shader;
};

class ParticleRenderSystem : public BaseECSSystem {
	public:
		ParticleRenderSystem(GameRenderContext& renderContext)
				: BaseECSSystem()
				, renderContext(renderContext) {
			addComponentType(ParticleEmitterComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components);
	private:
		GameRenderContext& renderContext;
};

#include "particles.inl"
