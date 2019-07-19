#pragma once

#include "math/vector.hpp"

namespace MotionIntegrators {
	inline void modifiedEuler(Vector3f& position, Vector3f& velocity, const Vector3f& acceleration, float delta) {
		velocity += acceleration * delta;
		position += velocity * delta;
	}

	inline void verlet(Vector3f& pos, Vector3f& velocity, const Vector3f& acceleration, float delta) {
		float halfDelta = 0.5f * delta;
		pos += velocity * halfDelta;
		velocity += acceleration * delta;
		pos += velocity * halfDelta;
	}

	inline void forestRuth(Vector3f& pos, Vector3f& velocity, const Vector3f& acceleration, float delta) {
		static const float frCoefficient = 1.0f / (2.0f - Math::pow(2, 1.0f / 3.0f));
		static const float frComplement = 1.0f - 2.0f * frCoefficient;

		verlet(pos, velocity, acceleration, delta * frCoefficient);
		verlet(pos, velocity, acceleration, delta * frComplement);
		verlet(pos, velocity, acceleration, delta * frCoefficient);
	}
};
