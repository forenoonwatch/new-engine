#include "aabb-collision-handler.hpp"

void AABBCollisionHandler::correctPositions() {
	for (uint32 i = 0; i < manifolds.size(); ++i) {
		manifolds[i].manifold.correctPositions(manifolds[i].tA->transform,
				manifolds[i].tB->transform);
	}

	manifolds.clear();
}

void AABBCollisionHandler::interact(float delta, const Manifold& m,
		BaseECSComponent** interactorComponents,
		BaseECSComponent** interacteeComponents) {
	Transform& transformOR = ((TransformComponent*)interactorComponents[0])->transform;
	Transform& transformEE = ((TransformComponent*)interacteeComponents[0])->transform;

	transformOR.setTranslation(transformOR.getTranslation() - (m.getContactVector() * m.getDepth()));
	transformEE.setTranslation(transformEE.getTranslation() + (m.getContactVector() * m.getDepth()));
	
	//m.applyImpulse((MotionComponent*)interactorComponents[2],
	//				(MotionComponent*)interacteeComponents[2]);

	//manifolds.emplace_back(m, (TransformComponent*)interactorComponents[0],
	//		(TransformComponent*)interacteeComponents[0]);
}
