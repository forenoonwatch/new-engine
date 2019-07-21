#pragma once

#include "physics/motion-integrators.hpp"
#include "util-components.hpp"

class MotionComponent : public ECSComponent<MotionComponent> {
	public:
		Vector3f velocity = Vector3f(0, 0, 0);
		Vector3f acceleration = Vector3f(0, 0, 0);
		Vector3f rotVelocity = Vector3f(0, 0, 0);
};

class MotionSystem : public BaseECSSystem {
	public:
		MotionSystem() : BaseECSSystem() {
			addComponentType(TransformComponent::ID);
			addComponentType(MotionComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override {
			TransformComponent* transform = (TransformComponent*)components[0];
			MotionComponent* motion = (MotionComponent*)components[1];

			Vector3f pos = transform->transform.getTranslation();
			Quaternion rot = transform->transform.getRotation();

			MotionIntegrators::forestRuth(pos, motion->velocity, motion->acceleration, delta);

			float mag = motion->rotVelocity.length();

			transform->transform.setTranslation(pos);

			if (mag > 0) {
				rot = (rot * Quaternion(motion->rotVelocity / mag, mag)).normalized();
				transform->transform.setRotation(rot);
			}
		}
	private:
};
