#include "rig.hpp"

void Rig::resetJoints() {
	for (uint32 i = 0; i < joints.size(); ++i) {
		transformSet[i] = globalInverseTransform * joints[i].localTransform;
	}
}

void Rig::calcRootJoint() {
	for (uint32 i = 0; i < joints.size(); ++i) {
		if (joints[i].parent < 0) {
			DEBUG_LOG_TEMP("Root joint is joint %d", i);
			rootJoint = i;
			return;
		}
	}
}
