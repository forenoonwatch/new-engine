#include "terrain-collision.hpp"

void TerrainCollisionSystem::updateComponents(float delta,
		BaseECSComponent** components) {
	Transform& transform = ((TransformComponent*)components[0])->transform;
	TerrainColliderComponent* collider = (TerrainColliderComponent*)components[1];
	MotionComponent* mc = (MotionComponent*)components[2];

	Vector3f pos, normal;

	if (terrain.intersectsRay(terrainTransform.inverseTransform(transform.getTranslation(), 1.f),
			castDirection, &pos, &normal)) {
		pos = terrainTransform.transform(pos, 1.f);
		
		float d = transform.getTranslation()[1] - pos[1];
		if (d <= collider->halfHeight) {
			transform.setTranslation(transform.getTranslation()
					+ Vector3f(0, collider->halfHeight - d, 0));
			//mc->velocity += Vector3f(terrainTransform.transform(normal, 0.f)) * delta;
			mc->velocity = Vector3f(mc->velocity[0], 0, mc->velocity[2]);
		}
	}
}
