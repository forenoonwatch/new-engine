
inline void ParticleEmitter::setPosition(float x, float y, float z) {
	emitterData.position[0] = x;
	emitterData.position[1] = y;
	emitterData.position[2] = z;

	feedbackBuffer.update(emitterData.position,
			Particle::POSITION_OFFSET, Particle::POSITION_SIZE);
}

inline void ParticleEmitter::setPosition(const Vector3f& position) {
	emitterData.position[0] = position[0];
	emitterData.position[1] = position[1];
	emitterData.position[2] = position[2];

	feedbackBuffer.update(emitterData.position,
			Particle::POSITION_OFFSET, Particle::POSITION_SIZE);
}

inline void ParticleEmitter::setVelocity(float x, float y, float z) {
	emitterData.velocity[0] = x;
	emitterData.velocity[1] = y;
	emitterData.velocity[2] = z;

	feedbackBuffer.update(emitterData.velocity,
			Particle::VELOCITY_OFFSET, Particle::VELOCITY_SIZE);
}

inline void ParticleEmitter::setVelocity(const Vector3f& velocity) {
	emitterData.velocity[0] = velocity[0];
	emitterData.velocity[1] = velocity[1];
	emitterData.velocity[2] = velocity[2];

	feedbackBuffer.update(emitterData.velocity,
			Particle::VELOCITY_OFFSET, Particle::VELOCITY_SIZE);
}

inline void ParticleEmitter::setTransparency(float start, float end) {
	emitterData.transScale[0] = start;
	emitterData.transScale[1] = end;

	feedbackBuffer.update(emitterData.transScale,
			Particle::TRANSPARENCY_OFFSET, Particle::TRANSPARENCY_SIZE);
}

inline void ParticleEmitter::setScale(float start, float end) {
	emitterData.transScale[2] = start;
	emitterData.transScale[3] = end;

	feedbackBuffer.update(&emitterData.transScale[2],
			Particle::SCALE_OFFSET, Particle::SCALE_SIZE);
}
