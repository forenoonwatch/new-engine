#pragma once

#include "ecs/ecs.hpp"
#include "math/transform.hpp"

class TransformComponent : public ECSComponent<TransformComponent> {
	public:
		Transform transform;
};

class EntityPointerComponent : public ECSComponent<EntityPointerComponent> {
	public:
		EntityHandle* entityPtr;
};
