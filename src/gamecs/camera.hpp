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

		virtual void updateComponents(float delta, BaseECSComponent** components) override {
			Transform& transform = ((TransformComponent*)components[0])->transform;
			Camera& camera = ((CameraComponent*)components[1])->camera;

			Matrix m = transform.toMatrix();
			float a = Math::acos(m[2][0]);
			Transform reflectTransform;
			reflectTransform.setTranslation(transform.getTranslation() * Vector3f(1.f, -1.f, 1.f));
			reflectTransform.setRotation(transform.getRotation().rotateBy(Quaternion(Vector3f(1.f,
								0.f, 0.f), -2.f * a)));

			camera.viewProjection = camera.projection * transform.inverse();
			camera.reflectMVP = camera.projection * reflectTransform.inverse();
			camera.position = transform.getTranslation();
			//camera.viewProjection = camera.reflectMVP;
			//camera.position = transform.getTranslation() * Vector3f(1.f, -1.f, 1.f);
		}
};
