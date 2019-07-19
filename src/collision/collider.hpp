#pragma once

#include "gamecs/motion.hpp"
#include "interaction-world.hpp"

#include "rendering/indexed-model.hpp"

class Collider {
	public:
	private:
};

class Manifold {
	public:
		inline Manifold(Collider* a, Collider* b,
				const Vector3f& contactVector, float depth)
			: a(a)
			, b(b)
			, contactVector(contactVector)
			, depth(depth) {}

		void applyImpulse(MotionComponent* mA, MotionComponent* mB) const;
		void correctPositions(Transform& tA, Transform& tB) const;

		inline const Collider* getA() const { return a; }
		inline const Collider* getB() const { return b; }

		inline const Vector3f& getContactVector() const { return contactVector; }
		inline float getDepth() const { return depth; }

		inline void addContactPoint(const Vector3f& point) { contacts.push_back(point); }
		inline Array<Vector3f>& getContacts() { return contacts; }
	private:
		Collider* a;
		Collider* b;

		Vector3f contactVector;
		float depth;

		Array<Vector3f> contacts;
};

class AABBColliderComponent : public ECSComponent<AABBColliderComponent>, public Collider {
	public:
		AABB aabb;
		AABB transformedAABB;
};

class MeshColliderComponent : public ECSComponent<MeshColliderComponent>, public Collider {
	public:
		IndexedModel* mesh;
};
