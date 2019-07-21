#include "quaternion.hpp"

Quaternion Quaternion::fromAxes(const Vector3f& right, const Vector3f& up,
		const Vector3f& forward) {
	float x, y, z, w;
	float trace = right[0] + up[1] + forward[2];

	if (trace > 0) {
		float s = 0.5f / sqrt(trace + 1.0f);
		w = 0.25f / s;
		x = (up[2] - forward[1]) * s;
		y = (forward[0] - right[2]) * s;
		z = (right[1] - up[0]) * s;
	}
	else {
		if (right[0] > up[1] && right[0] > forward[2]) {
			float s = 2.0f * sqrt(1.0f + right[0] - up[1] - forward[2]);
			w = (up[2] - forward[1]) / s;
			x = 0.25f * s;
			y = (up[0] + right[1]) / s;
			z = (forward[0] + right[2]) / s;
		}
		else if (up[1] > forward[2]) {
			float s = 2.0f * sqrt(1.0f + up[1] - right[0] - forward[2]);
			w = (forward[0] - right[2]) / s;
			x = (up[0] + right[1]) / s;
			y = 0.25f * s;
			z = (forward[1] + up[2]) / s;
		}
		else {
			float s = 2.0f * sqrt(1.0f + forward[2] - right[0] - up[1]);
			w = (right[1] - up[0]) / s;
			x = (forward[0] + right[2]) / s;
			y = (up[2] + forward[1]) / s;
			z = 0.25f * s;
		}
	}

	float mag = sqrt(x * x +  y * y + z * z + w * w);

	if (mag != 0) {
		x /= mag;
		y /= mag;
		z /= mag;
		w /= mag;
	}

	return Quaternion(x, y, z, w);
}

Quaternion Quaternion::normalized(float errorMargin) const {
	static const Vector defaultQuat = Vector::make(0.0f, 0.0f, 0.0f, 1.0f);
	Vector lenSq = vec.dot4(vec);
	Vector mask = lenSq >= Vector::load1f(errorMargin);
	Vector normalizedVec = vec * lenSq.rsqrt();
	return Quaternion(normalizedVec.select(mask, defaultQuat));
}

bool Quaternion::isNormalized(float errorMargin) const {
	return Math::abs(1.0f - lengthSquared()) < errorMargin;
}


Vector3f Quaternion::getAxis() const
{
	float w = vec[3];
	float rangleDivisor = Math::rsqrt(Math::max(1.0f - w*w, 0.0f));
	return Vector3f(vec * Vector::load1f(rangleDivisor));
}

float Quaternion::getAngle() const
{
	return 2.0f * Math::acos(vec[3]);
}

void Quaternion::axisAndAngle(Vector3f& axis, float& angle) const
{
	angle = getAngle();
	axis = getAxis();
}


Vector3f Quaternion::rotate(const Vector3f& other) const {
	return Vector3f(vec.quatRotateVec(other.toVector()));
}

Quaternion Quaternion::rotateBy(const Quaternion& other) const {
	return (other * (*this)).normalized();
}

Quaternion Quaternion::slerp(const Quaternion& dest, float amt, float errorMargin) const
{
	float cosAngleInitial = dot(dest);
	float cosAngle = Math::select(cosAngleInitial, cosAngleInitial, -cosAngleInitial);

	float lerpAmt1 = 1.0f - amt;
	float lerpAmt2 = amt;
	if(cosAngle < (1.0f - errorMargin))
	{
		float rsinAngle = Math::rsqrt(1.0f - cosAngle * cosAngle);
		float angle = Math::acos(cosAngle);
		// NOTE: You can also get rsinangle from doing
		//     Math::reciprocal(Math::sin(angle));
		lerpAmt1 = Math::sin(lerpAmt1 * angle) * rsinAngle;
		lerpAmt2 = Math::sin(lerpAmt2 * angle) * rsinAngle;
	}

	lerpAmt2 = Math::select(cosAngleInitial, lerpAmt2, -lerpAmt2);

	Vector lerpAmt1Vec = Vector::load1f(lerpAmt1);
	Vector lerpAmt2Vec = Vector::load1f(lerpAmt2);
	return Quaternion(vec * lerpAmt1Vec + dest.vec * lerpAmt2Vec);
}

Quaternion Quaternion::conjugate() const {
	static const Vector inverter = Vector::make(-1.0f, -1.0f, -1.0f, 1.0f);
	return Quaternion(vec * inverter);
}

Quaternion Quaternion::inverse() const {
	return normalized().conjugate();
}

Quaternion Quaternion::getRotationAroundAxis(const Vector3f& direction) const {
	Vector3f ra(vec[0], vec[1], vec[2]);
	Vector3f p = ra.projectOnto(direction);

	Quaternion twist(p[0], p[1], p[2], vec[3]);
	// Quaternion swing = rotation * twist.conjugate();
	return twist.normalized();
}
