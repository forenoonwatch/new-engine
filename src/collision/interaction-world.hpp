#pragma once

#include "collider.hpp"

class Manifold;

class Interaction {
	public:
		virtual void interact(float delta, const Manifold& manifold,
				BaseECSComponent** interactorComponents,
				BaseECSComponent** interacteeComponents) = 0;

		inline const Array<uint32>& getInteractorComponents() { return interactorComponentTypes; }
		inline const Array<uint32>& getInteracteeComponents() { return interacteeComponentTypes; }

		virtual ~Interaction() {}
	protected:
		inline void addInteractorComponentType(uint32 id) {
			interactorComponentTypes.push_back(id);
		}

		inline void addInteracteeComponentType(uint32 id) {
			interacteeComponentTypes.push_back(id);
		}
	private:
		Array<uint32> interactorComponentTypes;
		Array<uint32> interacteeComponentTypes;
};

class InteractionWorld : public ECSListener {
	public:
		inline InteractionWorld(ECS& ecs)
				: ECSListener(true, false)
				, ecs(ecs)	{}

		virtual void onMakeEntity(EntityHandle handle) override;
		virtual void onRemoveEntity(EntityHandle handle) override;

		virtual void processInteractions(float delta) = 0;
		
		void addInteraction(Interaction* interaction);
	protected:
		struct EntityInternal {
			EntityInternal(EntityHandle handle)
				: handle(handle) {}

			EntityHandle handle;
			Array<uint32> interactors;
			Array<uint32> interactees;
		};

		Array<EntityInternal> entities;
		Array<EntityHandle> entitiesToRemove;
		Array<EntityHandle> entitiesToUpdate;

		Array<Interaction*> interactions;

		ECS& ecs;

		void removeAndUpdateEntities();
		void addEntity(EntityHandle handle);

		void computeInteractions(EntityInternal& entity, uint32 interactionIndex);
};
