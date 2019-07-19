#include "collider.hpp"

#define K_SLOP			0.05f
#define CORRECT_PERCENT 1.f

void Manifold::applyImpulse(MotionComponent* mA, MotionComponent* mB) const {
	Vector3f rv = mA->velocity - mB->velocity;
	float contactVel = rv.dot(contactVector);

	if (contactVel > 0.f) {
		return;
	}

	float j = -0.5f * 2 * contactVel;
	Vector3f impulse = contactVector * j;

	mA->velocity -= impulse;
	mB->velocity += impulse;
}

void Manifold::correctPositions(Transform& tA, Transform& tB) const {
	Vector3f correction = contactVector
			* ((Math::max(depth - K_SLOP, 0.f) / 2) * CORRECT_PERCENT);
	
	tA.setTranslation(tA.getTranslation() - correction);
	tB.setTranslation(tB.getTranslation() + correction);
}
