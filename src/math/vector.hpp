#pragma once

#include "core/common.hpp"
#include "vecmath.hpp"

class Vector3f {
	public:
		FORCEINLINE Vector3f(const Vector& vecIn)
			: vec(vecIn) {}

		FORCEINLINE Vector3f()
			: vec(VectorConstants::ZERO) {}

		FORCEINLINE Vector3f(float val)
			: vec(Vector::make(val, val, val, 0.0f)) {}

		FORCEINLINE Vector3f(float xIn, float yIn, float zIn)
			: vec(Vector::make(xIn, yIn, zIn, 0.0f)) {}

		FORCEINLINE Vector3f(const float* vals)
			: vec(Vector::make(vals[0], vals[1], vals[2], 0.0f)) {}

		FORCEINLINE Vector3f(float xIn, float yIn, float zIn, float wIn)
			: vec(Vector::make(xIn, yIn, zIn, wIn)) {}


		FORCEINLINE float dot(const Vector3f& other) const;
		FORCEINLINE Vector3f dotToVector(const Vector3f& other) const;
		FORCEINLINE Vector3f cross(const Vector3f& other) const;
		FORCEINLINE float dist(const Vector3f& other) const;
		FORCEINLINE float distSquared(const Vector3f& other) const;

		FORCEINLINE Vector3f operator+(const Vector3f& other) const;
		FORCEINLINE Vector3f operator-(const Vector3f& other) const;
		FORCEINLINE Vector3f operator*(const Vector3f& other) const;
		FORCEINLINE Vector3f operator/(const Vector3f& other) const;

		FORCEINLINE Vector3f operator+(float amt) const;
		FORCEINLINE Vector3f operator-(float amt) const;
		FORCEINLINE Vector3f operator*(float amt) const;
		FORCEINLINE Vector3f operator/(float amt) const;

		bool operator==(const Vector3f& other) const;
		bool operator!=(const Vector3f& other) const;
		bool equals(const Vector3f& other, float errorMargin=1.e-4f) const;
		bool equals(float val, float errorMargin=1.e-4f) const;

		FORCEINLINE Vector3f operator-() const;
		FORCEINLINE Vector3f operator+=(const Vector3f& other);
		FORCEINLINE Vector3f operator-=(const Vector3f& other);
		FORCEINLINE Vector3f operator*=(const Vector3f& other);
		FORCEINLINE Vector3f operator/=(const Vector3f& other);

		FORCEINLINE Vector3f operator+=(float val);
		FORCEINLINE Vector3f operator-=(float val);
		FORCEINLINE Vector3f operator*=(float val);
		FORCEINLINE Vector3f operator/=(float val);

		float operator[](uint32 index) const;

		void set(float x, float y, float z);
		void set(uint32 index, float val);
		void set(const Vector3f& other);

		float max() const;
		float min() const;
		float absMax() const;
		float absMin() const;

		Vector3f abs() const;
		Vector3f min(const Vector3f& other) const;
		Vector3f max(const Vector3f& other) const;

		FORCEINLINE float length() const;
		FORCEINLINE float lengthSquared() const;

		Vector3f normalized(float errorMargin=1.e-8f) const;
		bool isNormalized(float errorMargin=1.e-4f) const;

		void dirAndLength(Vector3f& dir, float& length) const;
		Vector3f project() const;
		Vector3f reciprocal() const;

		Vector3f rotate(const Vector3f& axis, float angle) const;

		Vector3f reflect(const Vector3f& normal) const;
		Vector3f refract(const Vector3f& normal, float indexOfRefraction) const;

		Vector3f lerp(const Vector3f& dest, float amt) const;

		Vector3f toDegrees() const;
		Vector3f toRadians() const;

		FORCEINLINE Vector toVector() const;
		Vector toVector(float w) const;
	private:
		Vector vec;
};

class Vector2f {
	public:
		FORCEINLINE Vector2f() {
			vals[0]=0.0f;
			vals[1]=0.0f;
		}

		FORCEINLINE Vector2f(float val) {
			vals[0] = val;
			vals[1] = val;
		}

		FORCEINLINE Vector2f(float xIn, float yIn) {
			vals[0] = xIn;
			vals[1] = yIn;
		}

		FORCEINLINE float dot(const Vector2f& other) const;
		FORCEINLINE Vector2f dotToVector(const Vector2f& other) const;
		FORCEINLINE float cross(const Vector2f& other) const;
		FORCEINLINE float dist(const Vector2f& other) const;
		FORCEINLINE float distSquared(const Vector2f& other) const;

		FORCEINLINE Vector2f operator+(const Vector2f& other) const;
		FORCEINLINE Vector2f operator-(const Vector2f& other) const;
		FORCEINLINE Vector2f operator*(const Vector2f& other) const;
		FORCEINLINE Vector2f operator/(const Vector2f& other) const;

		FORCEINLINE Vector2f operator+(float amt) const;
		FORCEINLINE Vector2f operator-(float amt) const;
		FORCEINLINE Vector2f operator*(float amt) const;
		FORCEINLINE Vector2f operator/(float amt) const;

		bool operator==(const Vector2f& other) const;
		bool operator!=(const Vector2f& other) const;
		bool equals(const Vector2f& other, float errorMargin=1.e-4f) const;
		bool equals(float val, float errorMargin=1.e-4f) const;

		FORCEINLINE Vector2f operator-() const;
		FORCEINLINE Vector2f operator+=(const Vector2f& other);
		FORCEINLINE Vector2f operator-=(const Vector2f& other);
		FORCEINLINE Vector2f operator*=(const Vector2f& other);
		FORCEINLINE Vector2f operator/=(const Vector2f& other);

		FORCEINLINE Vector2f operator+=(float val);
		FORCEINLINE Vector2f operator-=(float val);
		FORCEINLINE Vector2f operator*=(float val);
		FORCEINLINE Vector2f operator/=(float val);

		float operator[](uint32 index) const;

		void set(float x, float y);
		void set(uint32 index, float val);
		float max() const;
		float min() const;
		float absMax() const;
		float absMin() const;

		Vector2f abs() const;
		Vector2f min(const Vector2f& other) const;
		Vector2f max(const Vector2f& other) const;

		FORCEINLINE float length() const;
		FORCEINLINE float lengthSquared() const;

		Vector2f normalized(float errorMargin=1.e-8f) const;
		bool isNormalized(float errorMargin=1.e-4f) const;

		void dirAndLength(Vector2f& dir, float& length, float errorMargin=1.e-8f) const;
		Vector2f reciprocal() const;

		Vector2f rotate(float angle) const;

		Vector2f reflect(const Vector2f& normal) const;
		Vector2f refract(const Vector2f& normal, float indexOfRefraction) const;

		Vector2f toDegrees() const;
		Vector2f toRadians() const;

		Vector toVector() const;
		Vector toVector(float z, float w) const;
		Vector toVector(Vector2f other) const;
	private:
		float vals[2];
};

#include "vector.inl"
