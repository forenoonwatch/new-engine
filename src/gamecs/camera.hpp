#pragma once

#include "util-components.hpp"

class Camera {
	public:
		Camera(const Matrix& projection)
			: projection(projection) {}

		Matrix projection;
		Matrix viewProjection;
		Vector3f position;
		Matrix reflectMVP;
	private:
};

class CameraComponent : public ECSComponent<CameraComponent> { // TODO: merge with camera class
	public:
		CameraComponent(Camera& camera)
			: camera(camera) {}

		Camera& camera;
};

class CameraSystem : public BaseECSSystem {
	public:
		CameraSystem() : BaseECSSystem() {
			addComponentType(TransformComponent::ID);
			addComponentType(CameraComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override;
};
