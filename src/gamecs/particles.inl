
inline void ParticleEmitter::setPosition(float x, float y, float z) {
	const float data[] = {x, y, z};

	particleSystem->getBuffer().update(data,
			getOffset() + Particle::POSITION_OFFSET, Particle::POSITION_SIZE);
}

inline void ParticleEmitter::setPosition(const Vector3f& position) {
	particleSystem->getBuffer().update(&position,
			getOffset() + Particle::POSITION_OFFSET, Particle::POSITION_SIZE);
}

inline void ParticleEmitter::setVelocity(float x, float y, float z) {
	const float data[] = {x, y, z};

	particleSystem->getBuffer().update(data,
			getOffset() + Particle::VELOCITY_OFFSET, Particle::VELOCITY_SIZE);
}

inline void ParticleEmitter::setVelocity(const Vector3f& velocity) {
	particleSystem->getBuffer().update(&velocity,
			getOffset() + Particle::VELOCITY_OFFSET, Particle::VELOCITY_SIZE);
}

inline void ParticleEmitter::setTransparency(float start, float end) {
	const float data[] = {start, end};

	particleSystem->getBuffer().update(data,
			getOffset() + Particle::TRANSPARENCY_OFFSET, Particle::TRANSPARENCY_SIZE);
}

inline void ParticleEmitter::setScale(float start, float end) {
	const float data[] = {start, end};

	particleSystem->getBuffer().update(data,
			getOffset() + Particle::SCALE_OFFSET, Particle::SCALE_SIZE);
}


inline ParticleEmitter& ParticleSystem::createEmitter(Texture* texture) {
	emitters[texture].emplace_back(*this, emitters[texture].size(), texture);
	return emitters[texture].back();
}

inline void ParticleSystem::removeEmitter(ParticleEmitter& emitter) {
	emitters[emitter.texture].back().id = emitter.id;
	emitters[emitter.texture].swap_remove(emitter.id);
}
