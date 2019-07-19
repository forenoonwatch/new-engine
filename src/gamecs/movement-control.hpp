#pragma once

#include "ecs/ecs.hpp"
//#include "math/transform.hpp"
#include "input/input-control.hpp"

#include "motion.hpp"

class MovementControl {
	public:
		MovementControl(const Vector3f& movement, InputControl* inputControl)
			: movement(movement)
			, inputControl(inputControl)
			, lastInputAmt(inputControl->getAmt()) {}

		Vector3f movement;
		InputControl* inputControl;
		float lastInputAmt;
};

class MovementControlComponent : public ECSComponent<MovementControlComponent> {
	public:
		Array<MovementControl> movementControls;
		Array<MovementControl> turnControls;
};

class MovementControlSystem : public BaseECSSystem {
	public:
		MovementControlSystem() : BaseECSSystem() {
			addComponentType(MovementControlComponent::ID);
			addComponentType(MotionComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override {
			MovementControlComponent* movementControl = (MovementControlComponent*)components[0];
			MotionComponent* motionComponent = (MotionComponent*)components[1];

			for (int i = 0; i < movementControl->movementControls.size(); ++i) {
				MovementControl& mvmtCtrl = movementControl->movementControls[i];
				motionComponent->acceleration += mvmtCtrl.movement * (mvmtCtrl.inputControl->getAmt() - mvmtCtrl.lastInputAmt);
				mvmtCtrl.lastInputAmt = mvmtCtrl.inputControl->getAmt();

				//Vector3f newPos = transform->transform.getTranslation()
				//	+ mvmtCtrl.first * (mvmtCtrl.second->getAmt() * delta);
				//transform->transform.setTranslation(newPos);
			}

			motionComponent->velocity *= 0.95; // cheap hack

			for (int i = 0; i < movementControl->turnControls.size(); ++i) {
				MovementControl& mvmtCtrl = movementControl->turnControls[i];
				motionComponent->rotVelocity = mvmtCtrl.movement * (mvmtCtrl.inputControl->getAmt() * delta);
			}
		}
	private:
};
