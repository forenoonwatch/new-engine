
inline void GameRenderContext::renderMesh(VertexArray& vertexArray,
		Material& material, const Matrix& transform) {
	meshRenderBuffer[std::make_pair(&vertexArray, &material)].push_back(camera.viewProjection * transform);
	meshRenderBuffer[std::make_pair(&vertexArray, &material)].push_back(transform);
}

inline void GameRenderContext::renderSkinnedMesh(VertexArray& vertexArray,
		Material& material, const Matrix& transform, Rig& rig) {
	skinnedMeshRenderBuffer[std::make_pair(&vertexArray, &material)].emplace_back(
			camera.viewProjection * transform, transform, &rig);
}

inline void GameRenderContext::emitParticle(ParticleEmitter& emitter,
		const Vector3f& position) {
	particles.emplace_back(&emitter, position, emitter.initialVelocity,
			emitter.timeToLive);
}
