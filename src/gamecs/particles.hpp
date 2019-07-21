#pragma once

#include "util-components.hpp"
#include "game-render-context.hpp"

class ParticleEmitter;
class GameRenderContext;

class Particle {
	public:
		Particle(ParticleEmitter* emitter = nullptr,
					const Vector3f& position = Vector3f(),
					const Vector3f& velocity = Vector3f(),
					float timeToLive = 0.f)
			: emitter(emitter)
			, position(position)
			, velocity(velocity)
			, timeToLive(timeToLive) {}

		Particle(Particle&&) = default;

		ParticleEmitter* emitter;
		Vector3f position;
		Vector3f velocity;
		float timeToLive;
};

class ParticleEmitter : public ECSComponent<ParticleEmitter> {
	public:
		Vector3f initialVelocity;
		Vector3f acceleration;
		Texture* texture;
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

		virtual void updateComponents(float delta, BaseECSComponent** components);
	private:
		GameRenderContext& renderContext;
};
