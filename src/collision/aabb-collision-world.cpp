#include "aabb-collision-world.hpp"

#include <algorithm>

AABBCollisionWorld::AABBCollisionWorld(ECS& ecs)
		: InteractionWorld(ecs)
		, compareAABB(ecs, 0) {
	addComponentID(TransformComponent::ID);
	addComponentID(AABBColliderComponent::ID);
}

void AABBCollisionWorld::onAddComponent(EntityHandle handle, uint32 id) {
	if (id == TransformComponent::ID) {
		if (ecs.getComponent<AABBColliderComponent>(handle) != nullptr) {
			addEntity(handle);
		}
	}
	else if (id == AABBColliderComponent::ID) {
		if (ecs.getComponent<TransformComponent>(handle) != nullptr) {
			addEntity(handle);
		}
	}
	else if (ecs.getComponent<AABBColliderComponent>(handle) != nullptr
			&& ecs.getComponent<TransformComponent>(handle) != nullptr) {
		entitiesToUpdate.push_back(handle);
	}
}

void AABBCollisionWorld::onRemoveComponent(EntityHandle handle, uint32 id) {
	if (id == TransformComponent::ID || id == AABBColliderComponent::ID) {
		entitiesToRemove.push_back(handle);
	}
	else if (ecs.getComponent<AABBColliderComponent>(handle) != nullptr
			&& ecs.getComponent<TransformComponent>(handle) != nullptr) {
		entitiesToUpdate.push_back(handle);
	}
}

void AABBCollisionWorld::processInteractions(float delta) {
	removeAndUpdateEntities();

	for (size_t i = 0; i < entities.size(); ++i) {
		AABBColliderComponent* comp = ecs.getComponent<AABBColliderComponent>(entities[i].handle);
		comp->transformedAABB = comp->aabb.transform(ecs.getComponent<TransformComponent>
				(entities[i].handle)->transform.toMatrix());
	}

	std::sort(std::begin(entities), std::end(entities), compareAABB);

	AABB* aabb;
	AABB* otherAABB;

	Vector3f centerSum, centerSqSum;

	Array<BaseECSComponent*> interactorComponents;
	Array<BaseECSComponent*> interacteeComponents;

	for (size_t i = 0; i < entities.size(); ++i) {
		aabb = &ecs.getComponent<AABBColliderComponent>(entities[i].handle)->transformedAABB;
		Vector3f center = aabb->getCenter();

		centerSum += center;
		centerSqSum += (center * center);

		for (size_t j = i + 1; j < entities.size(); ++j) {
			otherAABB = &ecs.getComponent<AABBColliderComponent>(entities[j].handle)->transformedAABB;

			if (otherAABB->getMinExtents()[compareAABB.axis]
					> aabb->getMaxExtents()[compareAABB.axis]) {
				break;
			}

			if (aabb->intersects(*otherAABB)) {
				size_t torIndex = i;
				size_t teeIndex = j;
				size_t temp;

				// TODO: check if the loop can be l = k + 1 instead of 0
				for (size_t checkNumber = 0; checkNumber < 1; ++checkNumber) {
					for (size_t k = 0; k < entities[torIndex].interactors.size(); ++k) {
						for (size_t l = 0; l < entities[teeIndex].interactees.size(); ++l) {
							if (entities[torIndex].interactors[k] == entities[teeIndex].interactees[l]) {
								Interaction* interaction = interactions[entities[torIndex].interactors[k]];

								interactorComponents.resize(Math::max(interactorComponents.size(),
										interaction->getInteractorComponents().size()));
								interacteeComponents.resize(Math::max(interacteeComponents.size(),
										interaction->getInteracteeComponents().size()));

								for (size_t m = 0; m < interaction->getInteractorComponents().size(); ++m) {
									interactorComponents[m] = ecs.getComponentByType(entities[torIndex].handle,
											interaction->getInteractorComponents()[m]);
								}

								for (size_t m = 0; m < interaction->getInteracteeComponents().size(); ++m) {
									interacteeComponents[m] = ecs.getComponentByType(entities[teeIndex].handle,
											interaction->getInteracteeComponents()[m]);
								}

								AABB overlap = aabb->overlap(*otherAABB);
								Vector3f extents = overlap.getExtents() * 0.5f;
								Vector3f minExtents;
								float depth;

								//Vector3f c = aabb->getCenter() - otherAABB->getCenter();

								if (extents[1] < extents[0]) {
									if (extents[2] < extents[1]) {
										minExtents = Vector3f(0, 0, 1);
										depth = extents[2];
									}
									else {
										minExtents = Vector3f(0, 1, 0);
										depth = extents[1];
									}
								}
								else {
									if (extents[2] < extents[0]) {
										minExtents = Vector3f(0, 0, 1);
										depth = extents[2];
									}
									else {
										minExtents = Vector3f(1, 0, 0);
										depth = extents[0];
									}
								}

								Manifold m((Collider*)&interactorComponents[0],
										(Collider*)&interacteeComponents[0],
										minExtents, depth);
								m.addContactPoint(overlap.getCenter());

								interaction->interact(delta, m, &interactorComponents[0], 
										&interacteeComponents[0]);
							}
						}
					}

					// check if interactee and interactor are in opposite order
					temp = torIndex;
					torIndex = teeIndex;
					teeIndex = temp;
				}
			}
		}
	}

	centerSum /= entities.size();
	centerSqSum /= entities.size();
	Vector3f variance = centerSqSum - (centerSum * centerSum);

	float maxVar = variance[0];
	uint32 maxVarAxis = 0;

	if (variance[1] > maxVar) {
		maxVar = variance[1];
		maxVarAxis = 1;
	}

	if (variance[2] > maxVar) {
		maxVar = variance[2];
		maxVarAxis = 2;
	}

	compareAABB.axis = maxVarAxis;
}
