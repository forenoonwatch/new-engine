#pragma once

#include "game-render-context.hpp"

class SkinnedMeshComponent : public ECSComponent<SkinnedMeshComponent> {
	public:
		VertexArray* vertexArray = nullptr;
		Material* material = nullptr;
		Rig* rig = nullptr;
};

class SkinnedMeshSystem : public BaseECSSystem {
	public:
		SkinnedMeshSystem(GameRenderContext& context)
				: BaseECSSystem()
				, context(context) {
			addComponentType(TransformComponent::ID);
			addComponentType(SkinnedMeshComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override {
			TransformComponent* transform = (TransformComponent*)components[0];
			SkinnedMeshComponent* mesh = (SkinnedMeshComponent*)components[1];
			
			context.renderSkinnedMesh(*mesh->vertexArray, *mesh->material,
					transform->transform.toMatrix(), *mesh->rig);
		}
	private:
		GameRenderContext& context;
};
