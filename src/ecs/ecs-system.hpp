#pragma once

#include "ecs-component.hpp"

class BaseECSSystem {
	public:
		enum {
			FLAG_OPTIONAL = 0x1,
		};

		inline BaseECSSystem() {}

		virtual void updateComponents(float delta, BaseECSComponent** components) {} // TODO: pure virtual?

		bool isValid() const;

		inline const Array<uint32>& getComponentTypes() const { return componentTypes; }
		inline const Array<uint32>& getComponentFlags() const { return componentFlags; }
	protected:
		inline void addComponentType(uint32 componentType, uint32 componentFlag = 0) {
			componentTypes.push_back(componentType);
			componentFlags.push_back(componentFlag);
		}
	private:
		Array<uint32> componentTypes;
		Array<uint32> componentFlags;
};

class ECSSystemList {
	public:
		inline bool addSystem(BaseECSSystem& system) {
			if (!system.isValid()) {
				return false;
			}

			systems.push_back(&system);
			return true;
		}

		bool removeSystem(BaseECSSystem& system);

		inline BaseECSSystem* operator[](uint32 index) { return systems[index]; }

		inline size_t size() { return systems.size(); }
	private:
		Array<BaseECSSystem*> systems;
};
