#pragma once

#include "animation.hpp"
#include "rig.hpp"

#include "gamecs/skinned-mesh.hpp"

// idea: starting at the root node, go down the node tree and set the final transform
// code:
// void animateJoint(joint, parentTransform):
//		animTransform = calcAnimTransformForJoint(joint)
//		globalTransform = parentTransform * animTransform
//		joint.finalTransform = globalInverseTransform * globalTransform * joint.baseTransform
//
//		for each childJoint in joint.children:
//			animateJoint(childJoint, globalTransform);

class Animator : public ECSComponent<Animator> {
	public:
		Animation* currentAnim = nullptr;
		float animTime = 0.f;
		uint32 frameIndex = 0;
};

class AnimatorSystem : public BaseECSSystem {
	public:
		inline AnimatorSystem()
				: BaseECSSystem() {
			addComponentType(Animator::ID);
			addComponentType(SkinnedMeshComponent::ID);
		}

		virtual void updateComponents(float deltaTime, BaseECSComponent** components) override;
	private:
		void calcJointTransform(Rig& rig, KeyFrame& frame0, KeyFrame& frame1,
				float lerpAmt, Joint* joint, const Matrix& parentTransform);
};
