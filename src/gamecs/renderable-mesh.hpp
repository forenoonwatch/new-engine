#pragma once

#include "game-render-context.hpp"

class RenderableMeshComponent : public ECSComponent<RenderableMeshComponent> {
	public:
		VertexArray* vertexArray = nullptr; // this is a mesh
		Material* material = nullptr;
};

class RenderableMeshSystem : public BaseECSSystem {
	public:
		RenderableMeshSystem(GameRenderContext& context)
				: BaseECSSystem()
				, context(context) {
			addComponentType(TransformComponent::ID);
			addComponentType(RenderableMeshComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override {
			TransformComponent* transform = (TransformComponent*)components[0];
			RenderableMeshComponent* mesh = (RenderableMeshComponent*)components[1];

			context.renderMesh(*mesh->vertexArray, *mesh->material,
					transform->transform.toMatrix());
		}
	private:
		GameRenderContext& context;
};

