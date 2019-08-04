#include "particles.hpp"

void ParticleRenderSystem::updateComponents(float delta, BaseECSComponent** components) {
	ParticleEmitterComponent* ec = (ParticleEmitterComponent*)components[0];

	renderContext.renderParticleEmitter(*ec->emitter, *ec->texture);
}
