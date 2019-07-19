#pragma once

#include <tuple>

#include "core/common.hpp"
#include "containers/array.hpp"

// TODO: improve memory

class BaseECSComponent;

typedef void* EntityHandle;
typedef uint32 (*ECSComponentCreateFunction)(Array<uint8>& memory, EntityHandle entity, BaseECSComponent* comp);
typedef void (*ECSComponentFreeFunction)(BaseECSComponent* comp);

#define NULL_ENTITY_HANDLE nullptr

class BaseECSComponent {
	public:
		static uint32 registerComponentType(ECSComponentCreateFunction createfn,
				ECSComponentFreeFunction freefn, size_t size);

		inline static ECSComponentCreateFunction getTypeCreateFunction(uint32 id) {
			return std::get<0>((*componentTypes)[id]);
		}

		inline static ECSComponentFreeFunction getTypeFreeFunction(uint32 id) {
			return std::get<1>((*componentTypes)[id]);
		}

		inline static size_t getTypeSize(uint32 id) {
			return std::get<2>((*componentTypes)[id]);
		}

		inline static bool isTypeValid(uint32 id) {
			return id < componentTypes->size();
		}

		EntityHandle entity = NULL_ENTITY_HANDLE;
	private: // TODO: replace the tuple with a dedicated class
		static Array<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>* componentTypes;
};

template <typename T>
class ECSComponent: public BaseECSComponent {
	public:
		static const uint32 ID;
		static const size_t SIZE;

		static const ECSComponentCreateFunction CREATE_FUNCTION;
		static const ECSComponentFreeFunction FREE_FUNCTION;
};

template<typename Component>
uint32 ECSComponentCreate(Array<uint8>& memory, EntityHandle entity, BaseECSComponent* comp) {
	uint32 index = memory.size();
	memory.resize(index + Component::SIZE);
	Component* component = new(&memory[index]) Component(*((Component*)comp));
	component->entity = entity;

	return index;
}

template <typename Component>
void ECSComponentFree(BaseECSComponent* comp) {
	Component* component = (Component*)comp;
	component->~Component();
}

template <typename T>
const uint32 ECSComponent<T>::ID(BaseECSComponent::registerComponentType(ECSComponentCreate<T>,
			ECSComponentFree<T>, sizeof(T)));

template <typename T>
const size_t ECSComponent<T>::SIZE(sizeof(T));

template <typename T>
const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);

template <typename T>
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);

class TestComponent: public ECSComponent<TestComponent> {
	public:
		float x;
		float y;
};
