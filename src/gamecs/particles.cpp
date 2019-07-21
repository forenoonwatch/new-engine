#include "particles.hpp"

void ParticleSystem::updateComponents(float delta, BaseECSComponent** components) {
	TransformComponent* transform = (TransformComponent*)components[0];
	ParticleEmitter* emitter = (ParticleEmitter*)components[1];

	emitter->counter += delta;

	float ppd = emitter->particlesPerSecond * delta;

	while (emitter->counter >= ppd) {
		emitter->counter -= ppd;
		renderContext.emitParticle(*emitter, transform->transform.getTranslation());
	}
}
