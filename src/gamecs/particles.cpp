#include "particles.hpp"

void ParticleUpdateSystem::updateComponents(float delta, BaseECSComponent** components) {
	ParticleEmitterComponent* ec = (ParticleEmitterComponent*)components[0];

	renderContext.updateFeedbackBuffer(*ec->shader, *ec->feedbackBuffer);
}

void ParticleRenderSystem::updateComponents(float delta, BaseECSComponent** components) {
	ParticleEmitterComponent* ec = (ParticleEmitterComponent*)components[0];

	renderContext.renderParticleEmitter(*ec->feedbackBuffer, *ec->texture);
}
