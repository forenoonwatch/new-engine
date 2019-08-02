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
	float age = 0;
	float transScale[4];
};

// encapsulates emitter params
class ParticleEmitter {
	public:
		inline ParticleEmitter(const Particle& emitterData)
			: emitterData(emitterData) {}

		inline Particle& getEmitterData() { return emitterData; }
	private:
		Particle emitterData;
};

class ParticleEmitterComponent : public ECSComponent<ParticleEmitterComponent> {
	public:
		ParticleEmitter* emitter;
		FeedbackBuffer* feedbackBuffer;
		Texture* texture;
		Shader* shader;
};

class ParticleUpdateSystem : public BaseECSSystem {
	public:
		ParticleUpdateSystem(GameRenderContext& renderContext)
				: BaseECSSystem()
				, renderContext(renderContext) {
			addComponentType(ParticleEmitterComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components);
	private:
		GameRenderContext& renderContext;
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
