#pragma once

#include "scene.hpp"

#include "gamecs/renderable-mesh.hpp"
#include "terrain/water.hpp"

class SpinSystem : public BaseECSSystem {
	public:
		SpinSystem(GameRenderContext& rc)
				: BaseECSSystem()
				, counter(0)
				, rc(rc) {
			addComponentType(TransformComponent::ID);
			addComponentType(RenderableMeshComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override;
	private:
		float counter;
		GameRenderContext& rc;
};

class TestScene2 : public Scene {
	public:
		virtual int load(Game& game);
};
