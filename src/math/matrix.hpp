#pragma once

#include "vecmath.hpp"
#include "vector.hpp"
#include "quaternion.hpp"
#include "plane.hpp"

class Matrix {
	public:
		FORCEINLINE Matrix();
		FORCEINLINE Matrix(const Vector& vecX, const Vector& vecY,
				const Vector& vecZ, const Vector& vecOffset);
		FORCEINLINE Matrix(const float* data);

		static FORCEINLINE Matrix identity();
		static FORCEINLINE Matrix translate(float x, float y, float z);
		static FORCEINLINE Matrix translate(const Vector3f& amt);
		static FORCEINLINE Matrix scale(const Vector3f& amt);
		static FORCEINLINE Matrix scale(float amt);
		static FORCEINLINE Matrix ortho(float left, float right,
				float bottom, float top, float near, float far);
		static FORCEINLINE Matrix perspective(float halfFov, float aspect,
				float nearZ, float farZ);
		static FORCEINLINE Matrix transformMatrix(const Vector3f& translation,
				const Quaternion& rotation, const Vector3f& scale);

		void extractFrustumPlanes(Plane* planes) const;
		Matrix toNormalMatrix() const;
		
		FORCEINLINE Matrix operator+ (const Matrix& other) const;
		FORCEINLINE Matrix& operator+= (const Matrix& other);
		FORCEINLINE Matrix operator* (const Matrix& other) const;
		FORCEINLINE Matrix& operator*= (const Matrix& other);
		FORCEINLINE Matrix operator* (float amt) const;
		FORCEINLINE Matrix& operator*= (float amt);
		FORCEINLINE bool operator==(const Matrix& other) const;
		FORCEINLINE bool operator!=(const Matrix& other) const;

		FORCEINLINE bool equals(const Matrix& other, float errorMargin=1.e-4f) const;
		FORCEINLINE Vector transform(const Vector& vector) const;

		FORCEINLINE Matrix transpose() const;
		FORCEINLINE float determinant4x4() const;
		FORCEINLINE float determinant3x3() const;
		FORCEINLINE Matrix inverse() const;

		FORCEINLINE Matrix applyScale(const Vector& scale);
		FORCEINLINE Vector removeScale(float errorMargin=1.e-8f);
		FORCEINLINE Vector getScale() const;
		Quaternion getRotation() const;
		FORCEINLINE Vector getTranslation() const;

		FORCEINLINE Vector operator[](uint32 index) const {
			assertCheck(index < 4);
			return m[index];
		}
	private:
		Vector m[4];
};

#include "matrix.inl"
