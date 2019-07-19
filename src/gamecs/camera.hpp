#pragma once

class Camera {
	public:
		Camera(const Matrix& projection)
			: projection(projection) {}

		Matrix projection;
		Matrix viewProjection;
		Vector3f position;
	private:
};

class CameraComponent : public ECSComponent<CameraComponent> {
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

			camera.viewProjection = camera.projection * transform.inverse();
			camera.position = transform.getTranslation();
		}
};
