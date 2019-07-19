#pragma once

#include "collider.hpp"

class TerrainColliderComponent : public ECSComponent<TerrainColliderComponent>, public Collider {
	public:
		float halfHeight;
		float radius;
};

class TerrainCollisionSystem : public BaseECSSystem {
	public:
		inline TerrainCollisionSystem(IndexedModel& terrain, const Transform& terrainTransform)
				: BaseECSSystem()
				, terrain(terrain)
				, terrainTransform(terrainTransform)
				, castDirection(terrainTransform.inverseTransform(Vector3f(0.f, -1.f, 0.f), 0.f)) {
			addComponentType(TransformComponent::ID);
			addComponentType(TerrainColliderComponent::ID);
			addComponentType(MotionComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override;
	private:
		IndexedModel& terrain;
		Transform terrainTransform;
		Vector3f castDirection;
};
