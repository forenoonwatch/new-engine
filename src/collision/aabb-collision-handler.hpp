#pragma once

#include <tuple>

#include "collider.hpp"

class AABBCollisionHandler : public Interaction {
	public:
		inline AABBCollisionHandler()
				: Interaction() {
			addInteractorComponentType(TransformComponent::ID);
			addInteractorComponentType(AABBColliderComponent::ID);
			addInteractorComponentType(MotionComponent::ID);

			addInteracteeComponentType(TransformComponent::ID);
			addInteracteeComponentType(AABBColliderComponent::ID);
			addInteracteeComponentType(MotionComponent::ID);
		}

		void correctPositions();

		virtual void interact(float delta, const Manifold& m,
				BaseECSComponent** interactorComponents,
				BaseECSComponent** interacteeComponents) override;
	private:
		struct ManifoldData {
			inline ManifoldData(const Manifold& manifold,
				TransformComponent* tA, TransformComponent* tB)
					: manifold(manifold)
					, tA(tA)
					, tB(tB) {}

			Manifold manifold;
			TransformComponent* tA;
			TransformComponent* tB;
		};

		Array<ManifoldData> manifolds;
};
