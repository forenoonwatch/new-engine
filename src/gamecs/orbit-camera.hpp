#pragma once

#include "camera.hpp"
#include "movement-control.hpp"

class CameraControlComponent : public ECSComponent<CameraControlComponent> {
	public:
		Array<MovementControl> movementControls;
		Vector3f offsets; // pitch, yaw, distance
};

class OrbitCameraSystem : public BaseECSSystem {
	public:
		OrbitCameraSystem(ECS& ecs, GameEventHandler& eventHandler)
				: BaseECSSystem()
				, ecs(ecs)
				, eventHandler(eventHandler) {
			addComponentType(TransformComponent::ID);
			addComponentType(CameraComponent::ID);
			addComponentType(EntityPointerComponent::ID);
			addComponentType(CameraControlComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override {
			Transform& camTransform = ((TransformComponent*)components[0])->transform;
			EntityHandle* eh = ((EntityPointerComponent*)components[2])->entityPtr;
			Transform& targTransform = ecs.getComponent<TransformComponent>(*eh)->transform;
			CameraControlComponent* ccc = (CameraControlComponent*)components[3];

			if (eventHandler.isMouseDown(Input::MOUSE_RIGHT_BUTTON)) {
				for (auto it = std::begin(ccc->movementControls), end = std::end(ccc->movementControls);
						it != end; ++it) {
					ccc->offsets += it->movement * it->inputControl->getAmt();
				}

				ccc->offsets = Vector3f(Math::clamp(ccc->offsets[0], -1.27f, 1.27f),
						ccc->offsets[1],
						Math::clamp(ccc->offsets[2], 0.1f, 16.f));
			}

			Quaternion qy = Quaternion(Vector3f(0, 1, 0), ccc->offsets[1]);
			Quaternion qx = Quaternion(Vector3f(1, 0, 0), ccc->offsets[0]);
			Vector3f offs = qx.rotateBy(qy).rotate(Vector3f(0.f, 0.f, ccc->offsets[2]));

			camTransform.setTranslation(targTransform.getTranslation() + offs);
			camTransform.lookAt(targTransform.getTranslation());
		}
	private:
		ECS& ecs;
		GameEventHandler& eventHandler;
};
