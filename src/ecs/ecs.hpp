#pragma once

#include "ecs-system.hpp"
#include "containers/hash-map.hpp"

typedef Array<Pair<uint32, uint32>> EntityComponents; // TODO: make a better name for this
//typedef std::pair<uint32, Entity> RawEntity;

/**
 * The kind of object that EntityHandle (a void* typedef) points to internally
 */
struct RawEntity { // TODO: rename this to EntityInternal or something better
	uint32 index; // TODO: tentatively, the index of this entity in ECS.entities 
	EntityComponents entityComponents; // TODO: better convey what this actually is
	// TODO: tentatively, this is an array of pair<component ID, index in memory of the component>
	// TODO: ensure an Array<pair> is the ideal data structure
};

class ECS;

class ECSListener {
	public:
		inline ECSListener(bool notifyOnComps = false, bool notifyOnEntities = false)
			: notifyOnAllComponentOperations(notifyOnComps)
			, notifyOnAllEntityOperations(notifyOnEntities) {}

		virtual void onMakeEntity(EntityHandle handle) = 0;
		virtual void onRemoveEntity(EntityHandle handle) = 0;

		virtual void onAddComponent(EntityHandle handle, uint32 id) = 0;
		virtual void onRemoveComponent(EntityHandle handle, uint32 id) = 0;

		inline const Array<uint32>& getComponentIDs() { return componentIDs; }

		virtual ~ECSListener() {}
	protected:
		inline void setNotificationSettings(bool notifyOnComps, bool notifyOnEntities) {
			notifyOnAllComponentOperations = notifyOnComps;
			notifyOnAllEntityOperations = notifyOnEntities;
		}

		inline void addComponentID(uint32 id) { componentIDs.push_back(id); }
	private:
		Array<uint32> componentIDs;

		bool notifyOnAllComponentOperations;
		bool notifyOnAllEntityOperations;

		friend class ECS;
};

class ECS {
	public:
		ECS() {}

		// ECSListener methods
		inline void addListener(ECSListener* listener) { listeners.push_back(listener); }

		// Entity methods
		EntityHandle makeEntityFromLists(BaseECSComponent** components,
				const uint32* componentIDs, size_t numComponents);
		
		template <class... ComponentTypes>
		EntityHandle makeEntity(ComponentTypes... componentsIn);

		void removeEntity(EntityHandle handle);

		// Component methods
		template <class Component>
		inline void addComponent(EntityHandle handle, Component* component) {
			addComponentInternal(handle, handleToEntityComponents(handle), Component::ID, component);

			for (uint32 i = 0; i < listeners.size(); ++i) {
				const Array<uint32>& componentIDs = listeners[i]->getComponentIDs();

				if (listeners[i]->notifyOnAllComponentOperations) {
					listeners[i]->onAddComponent(handle, Component::ID);
				}
				else {
					for (uint32 j = 0; j < componentIDs.size(); ++j) {
						if (componentIDs[j] == Component::ID) {
							listeners[i]->onAddComponent(handle, Component::ID);
							break;
						}
					}
				}
			}
		}
		
		template <class Component>
		inline bool removeComponent(EntityHandle handle) {
			for (uint32 i = 0; i < listeners.size(); ++i) {
				const Array<uint32>& componentIDs = listeners[i]->getComponentIDs();

				if (listeners[i]->notifyOnAllComponentOperations) {
					listeners[i]->onRemoveComponent(handle, Component::ID);
				}
				else {
					for (uint32 j = 0; j < componentIDs.size(); ++j) {
						if (componentIDs[j] == Component::ID) {
							listeners[i]->onRemoveComponent(handle, Component::ID);
							break;
						}
					}
				}
			}

			return removeComponentInternal(handle, Component::ID);
		}

		template <class Component>
		inline Component* getComponent(EntityHandle handle) {
			return (Component*)getComponentInternal(handleToEntityComponents(handle), 
					components[Component::ID], Component::ID);
		}

		// TODO: getComponentInternal O(n) complexity
		inline BaseECSComponent* getComponentByType(EntityHandle entity, uint32 componentID) {
			return getComponentInternal(handleToEntityComponents(entity),
					components[componentID], componentID);
		}
		
		// System methods
		void updateSystems(ECSSystemList& systems, float delta);

		~ECS();
	private:
		HashMap<uint32, Array<uint8>> components;
		Array<RawEntity*> entities;
		Array<ECSListener*> listeners;

		inline RawEntity* handleToRawType(EntityHandle handle) {
			return (RawEntity*)handle;
		}

		inline uint32 handleToEntityIndex(EntityHandle handle) {
			return ((RawEntity*)handle)->index;
		}

		inline EntityComponents& handleToEntityComponents(EntityHandle handle) {
			return ((RawEntity*)handle)->entityComponents;
		}

		void addComponentInternal(EntityHandle, EntityComponents&, uint32, BaseECSComponent*);
		BaseECSComponent* getComponentInternal(EntityComponents&, Array<uint8>&, uint32);
		bool removeComponentInternal(EntityHandle, uint32);

		void deleteComponent(uint32, uint32);

		void updateSystemWithMultipleComponents(uint32 index, ECSSystemList& systems, float delta,
				const Array<uint32>& componentTypes, Array<BaseECSComponent*>& componentParam,
				Array<Array<uint8>*>& componentArrays);
		uint32 findLeastCommonComponent(const Array<uint32>& componentTypes,
				const Array<uint32>& componentFlags);

		NULL_COPY_AND_ASSIGN(ECS);
};

#include "ecs.inl"
