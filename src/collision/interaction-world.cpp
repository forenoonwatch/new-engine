#include "interaction-world.hpp"

void InteractionWorld::onMakeEntity(EntityHandle handle) {
	addEntity(handle);
}

void InteractionWorld::onRemoveEntity(EntityHandle handle) {
	entitiesToRemove.push_back(handle);
}

void InteractionWorld::addInteraction(Interaction* interaction) {
	interactions.push_back(interaction);
	const size_t index = interactions.size() - 1;
	
	for (int i = 0; i < entities.size(); ++i) {
		computeInteractions(entities[i], index);
	}
}

void InteractionWorld::removeAndUpdateEntities() {
	if (entitiesToRemove.size() == 0 && entitiesToUpdate.size() == 0) {
		return;
	}

	bool didRemove;

	for (size_t i = 0; i < entities.size(); ++i) {
		// remove entities
		do {
			didRemove = false;

			for (size_t j = 0; j < entitiesToRemove.size(); ++j) {
				if (entities[i].handle == entitiesToRemove[j]) {
					entities.swap_remove(i);
					entitiesToRemove.swap_remove(j);

					didRemove = true;
					break;
				}
			}

			if (entitiesToRemove.size() == 0 && entitiesToUpdate.size() == 0) {
				return;
			}
		}
		while (didRemove);

		// update entities
		
		for (size_t j = 0; j < entitiesToUpdate.size(); ++j) {
			if (entities[i].handle == entitiesToUpdate[j]) {
				entities[i].interactors.clear();
				entities[i].interactees.clear();

				for (size_t k = 0; k < interactions.size(); ++k) {
					computeInteractions(entities[i], k);
				}

				entitiesToUpdate.swap_remove(j);
			}
		}
	}

	entitiesToRemove.clear();
	entitiesToUpdate.clear();
}

void InteractionWorld::addEntity(EntityHandle handle) {
	entities.emplace_back(handle);

	for (int i = 0; i < interactions.size(); ++i) {
		computeInteractions(entities.back(), i);
	}
}

void InteractionWorld::computeInteractions(EntityInternal& entity,
		uint32 interactionIndex) {
	Interaction* interaction = interactions[interactionIndex];
	const Array<uint32>& interactorComponents = interaction->getInteractorComponents();
	const Array<uint32>& interacteeComponents = interaction->getInteracteeComponents();

	bool isInteractor = true, isInteractee = true;

	for (size_t i = 0; i < interactorComponents.size(); ++i) {
		if (ecs.getComponentByType(entity.handle, interactorComponents[i]) == nullptr) {
			isInteractor = false;
			break;
		}
	}

	for (size_t i = 0; i < interacteeComponents.size(); ++i) {
		if (ecs.getComponentByType(entity.handle, interacteeComponents[i]) == nullptr) {
			isInteractee = false;
			break;
		}
	}

	if (isInteractor) {
		entity.interactors.push_back(interactionIndex);
	}
	
	if (isInteractee) {
		entity.interactees.push_back(interactionIndex);
	}
}
