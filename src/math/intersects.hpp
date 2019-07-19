#pragma once

#include "plane.hpp"
#include "aabb.hpp"
#include "sphere.hpp"

namespace Intersects {
	static FORCEINLINE void intersectPlaneAABBFast(const Vector& center, const Vector& extents, 
			const Plane& plane, const Plane& absPlane, bool& isFullyInside, bool& isPartiallyInside) {
		float d = plane.dotVector(center);
		float r = absPlane.dotVector(extents);
		isFullyInside = d-r >= 0;
		isPartiallyInside = d+r > 0;
	}

	static FORCEINLINE bool intersectPlaneAABB(const AABB& aabb, const Plane& plane, bool& isFullyInside, bool& isPartiallyInside) {
		intersectPlaneAABBFast(aabb.getCenter().toVector(1.0f), aabb.getExtents().toVector(1.0f), plane, plane.abs(),
				isFullyInside, isPartiallyInside);
		return isPartiallyInside && !isFullyInside;
	}

	static FORCEINLINE bool intersectPlaneAABB(const AABB& aabb, const Plane& plane) {
		bool isFullyInside;
		bool isPartiallyInside;
		return intersectPlaneAABB(aabb, plane, isFullyInside, isPartiallyInside);
	}

	static FORCEINLINE void intersectPlaneSphereFast(const Vector& center, float radius, const Plane& plane, bool& isFullyInside, bool& isPartiallyInside) {
		float d = plane.dot(center);
		isFullyInside = d >= radius;
		isPartiallyInside = d >= -radius;
	}

	static FORCEINLINE bool intersectPlaneSphere(const Sphere& sphere, const Plane& plane, bool& isFullyInside, bool& isPartiallyInside) {
		intersectPlaneSphereFast(sphere.getCenter().toVector(1.0f), sphere.getRadius(), plane, isFullyInside, isPartiallyInside);
		return isPartiallyInside && !isFullyInside; 
	}

	static FORCEINLINE bool intersectPlaneSphere(const Sphere& sphere, const Plane& plane) {
		bool isFullyInside;
		bool isPartiallyInside;
		return intersectPlaneSphere(sphere, plane, isFullyInside, isPartiallyInside); 
	}

	static FORCEINLINE bool intersectSphereAABBFast(const Vector& sphereCenter, const Vector& aabbMins, const Vector& aabbMaxs, float radiusSq) {
		Vector result =
			  (sphereCenter - aabbMins).select(sphereCenter < aabbMins, VectorConstants::ZERO) 
			+ (sphereCenter - aabbMaxs).select(sphereCenter > aabbMaxs, VectorConstants::ZERO);
		return result.dot3(result)[0] < radiusSq;
	}

	static FORCEINLINE bool intersectSphereAABB(const Sphere& sphere, const AABB& aabb) {
		Vector sphereCenter = sphere.getCenter().toVector();
		float radiusSq = sphere.getRadius()*sphere.getRadius();
		Vector aabbMins = aabb.getMinExtents().toVector();
		Vector aabbMaxs = aabb.getMaxExtents().toVector();
		return intersectSphereAABBFast(sphereCenter, aabbMins, aabbMaxs, radiusSq);
	}

	static FORCEINLINE bool intersectTriangle(const Vector3f& pos, const Vector3f& dir,
			const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, Vector3f* intersectPos,
			Vector3f* normal) {
		Vector3f e1 = v1 - v0;
		Vector3f e2 = v2 - v0;
		
		Vector3f h = dir.cross(e2);
		float a = e1.dot(h);

		if (a > -0.00001f && a < 0.0001f) {
			return false;
		}

		float f = 1.f / a;

		Vector3f s = pos - v0;
		float u = f * s.dot(h);

		if (u < 0.f || u > 1.f) {
			return false;
		}

		Vector3f q = s.cross(e1);
		float v = f * dir.dot(q);

		if (v < 0.f || u + v > 1.f) {
			return false;
		}

		f *= e2.dot(q);

		if (f > 0.00001) {
			intersectPos->set(pos + dir * f);
			normal->set(e1.cross(e2).normalized());

			return true;
		}

		return false;
	}

	static FORCEINLINE bool intersectTriangle(const Vector3f& pos, const Vector3f& dir,
			const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, float& f,
			Vector3f* normal) {
		Vector3f e1 = v1 - v0;
		Vector3f e2 = v2 - v0;
		
		Vector3f h = dir.cross(e2);
		float a = e1.dot(h);

		if (a > -0.00001f && a < 0.0001f) {
			return false;
		}

		f = 1.f / a;

		Vector3f s = pos - v0;
		float u = f * s.dot(h);

		if (u < 0.f || u > 1.f) {
			return false;
		}

		Vector3f q = s.cross(e1);
		float v = f * dir.dot(q);

		if (v < 0.f || u + v > 1.f) {
			return false;
		}

		f *= e2.dot(q);

		if (f > 0.00001) {
			normal->set(e1.cross(e2).normalized());

			return true;
		}

		return false;
	}
}
