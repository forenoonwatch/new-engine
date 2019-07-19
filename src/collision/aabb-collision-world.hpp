#pragma once

#include "collider.hpp"

class AABBCollisionWorld : public InteractionWorld {
	public:
		AABBCollisionWorld(ECS&);

		virtual void onAddComponent(EntityHandle, uint32) override;
		virtual void onRemoveComponent(EntityHandle, uint32) override;

		virtual void processInteractions(float delta) override;
	private:
		struct InteractionWorldCompare {
			InteractionWorldCompare(ECS& ecs, uint32 axis)
				: ecs(ecs)
				, axis(axis) {}

			inline bool operator()(EntityInternal& a, EntityInternal& b) {
				return ecs.getComponent<AABBColliderComponent>(a.handle)->transformedAABB.getMinExtents()[axis]
					< ecs.getComponent<AABBColliderComponent>(b.handle)->transformedAABB.getMinExtents()[axis];
			}

			uint32 axis;
			ECS& ecs;
		};

		InteractionWorldCompare compareAABB;
};
