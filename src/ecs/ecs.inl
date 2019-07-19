#include <initializer_list>

namespace {
	template <typename T>
	inline uint32 deduceID(T& obj) {
		return T::ID;
	}

	template <typename T>
	inline BaseECSComponent* deduceComponents(T& obj) {
		return (BaseECSComponent*)&obj;
	}
};

template <class... ComponentTypes>
inline EntityHandle ECS::makeEntity(ComponentTypes... componentsIn) {
	const size_t numComponents = sizeof...(ComponentTypes);
	BaseECSComponent* components[numComponents];
	uint32 componentIDs[numComponents];
	
	std::initializer_list<uint32> ids {deduceID(componentsIn)...};
	std::initializer_list<BaseECSComponent*> comps {deduceComponents(componentsIn)...};
	int i = 0;

	for (auto it = std::begin(ids), end = std::end(ids); it != end; ++it) {
		componentIDs[i++] = *it;
	}

	i = 0;

	for (auto it = std::begin(comps), end = std::end(comps); it != end; ++it) {
		components[i++] = *it;
	}

	return makeEntityFromLists(components, componentIDs, numComponents);
}
