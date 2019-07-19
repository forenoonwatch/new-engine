#include "animator.hpp"

void AnimatorSystem::updateComponents(float deltaTime, BaseECSComponent** components) {
	Animator* animator = (Animator*)components[0];
	Rig& rig = *( ((SkinnedMeshComponent*)components[1])->rig );
	
	if (animator->currentAnim != nullptr) {
		animator->animTime += deltaTime;
		KeyFrame& currentFrame = animator->currentAnim->getKeyFrame(animator->frameIndex);
		KeyFrame& nextFrame = animator->currentAnim->getKeyFrame(animator->frameIndex + 1);

		float lerpAmt = (animator->animTime - currentFrame.getTime())
				/ (nextFrame.getTime() - currentFrame.getTime());
		
		calcJointTransform(rig, currentFrame, nextFrame,
				lerpAmt, rig.getRootJoint(), Matrix::identity());

		if (animator->animTime >= nextFrame.getTime()) {
			++animator->frameIndex;

			if (animator->frameIndex == animator->currentAnim->getNumFrames() - 1) {
				animator->frameIndex = 0;
				animator->animTime -= nextFrame.getTime();
			}
		}
	}
}

void AnimatorSystem::calcJointTransform(Rig& rig, KeyFrame& frame0, KeyFrame& frame1,
		float lerpAmt, Joint* joint, const Matrix& parentTransform) {
	Transform res;
	frame0.getJointTransform(joint->name).lerp(frame1.getJointTransform(joint->name), lerpAmt, res);
	
	Matrix globalTransform = parentTransform * res.toMatrix();

	rig.setFinalTransform(joint->index,
			rig.getGlobalInverseTransform() * globalTransform * joint->localTransform);

	for (uint32 i = 0; i < joint->children.size(); ++i) {
		calcJointTransform(rig, frame0, frame1, lerpAmt,
				&rig.getJoint(joint->children[i]), globalTransform);
	}
}
