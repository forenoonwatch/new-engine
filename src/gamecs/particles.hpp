#pragma once

#include "util-components.hpp"

class Particle {
	public:
		ParticleEmitter* emitter;
		Vector3f position;
		Vector3f velocity;
		float timeToLive;
};

class ParticleEmitter : public ECSComponent<ParticleEmitter> {
	public:
		Vector3f initialVelocity;
		Vector3f acceleration;
		Texture texture;
		float timeToLive;

		float particlesPerSecond;
		float counter;
};

class ParticleSystem : public BaseECSSystem {
	public:
		ParticleSystem(GameRenderContext& renderContext)
				: BaseECSSystem()
				, renderContext(renderContext) {
			addComponentType(TransformComponent::ID);
			addComponentType(ParticleEmitter::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override {
			TransformComponent* transform = (TransformComponent*)components[0];
			ParticleEmitter* emitter = (ParticleEmitter*)components[1];

			emitter->counter += delta;

			float ppd = emitter->particlesPerSecond * delta;

			while (emitter->counter >= ppd) {
				emitter->counter -= ppd;
				renderContext.emitParticle(*emitter);
			}
		}
	private:
		GameRenderContext& renderContext;
}:
