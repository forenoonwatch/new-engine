#pragma once

#include "scene.hpp"

#include "core/input.hpp"

#include "rendering/model-loader.hpp"

#include "gamecs/renderable-mesh.hpp"
#include "gamecs/skinned-mesh.hpp"
#include "gamecs/movement-control.hpp"
#include "gamecs/motion.hpp"
#include "gamecs/text-renderer.hpp"

#include "animation/animator.hpp"

#include "collision/terrain-collision.hpp"
#include "terrain/water.hpp"

class TestInteraction : public Interaction {
	public:
		TestInteraction() : Interaction() {
			addInteractorComponentType(TransformComponent::ID);
			addInteractorComponentType(AABBColliderComponent::ID);
			addInteractorComponentType(MotionComponent::ID);

			addInteracteeComponentType(TransformComponent::ID);
			addInteracteeComponentType(AABBColliderComponent::ID);
			addInteracteeComponentType(MotionComponent::ID);
		}

		virtual void interact(float delta, const Manifold& m, BaseECSComponent** interactorComponents,
				BaseECSComponent** interacteeComponents) override {
			Transform& transformOR = ((TransformComponent*)interactorComponents[0])->transform;
			Transform& transformEE = ((TransformComponent*)interacteeComponents[0])->transform;

			MotionComponent* motionComponent = ((MotionComponent*)interactorComponents[2]);

			//motionComponent->velocity *= -1.1f;
			//transformOR.setTranslation(transformOR.getTranslation() - (m.getContactVector() * m.getDepth()));
			//transformEE.setTranslation(transformEE.getTranslation() + (m.getContactVector() * m.getDepth()));

			m.applyImpulse((MotionComponent*)interactorComponents[2],
					(MotionComponent*)interacteeComponents[2]);
		}
	private:
		Array<Manifold> manifolds;
};

class SpinSystem : public BaseECSSystem {
	public:
		SpinSystem()
				: BaseECSSystem()
				, counter(0) {
			addComponentType(TransformComponent::ID);
			addComponentType(SkinnedMeshComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override {
			counter += 0.5f * delta;

			Transform& transform = ((TransformComponent*)components[0])->transform;

			transform.setRotation(Quaternion(Vector3f(0.f, 1.f, 0.f), counter));
		}
	private:
		float counter;
};

class FPSUpdateSystem : public BaseECSSystem {
	public:
		FPSUpdateSystem(Game& game)
				: BaseECSSystem()
				, game(game) {
			addComponentType(RenderableText::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override {
			RenderableText* renderableText = (RenderableText*)components[0];

			renderableText->text = "FPS: " + StringUtil::toString(game.getFPS());
		}
	private:
		Game& game;
};

class FollowCameraSystem : public BaseECSSystem {
	public:
		FollowCameraSystem(ECS& ecs, EntityHandle& follow)
				: BaseECSSystem()
				, ecs(ecs)
				, follow(follow)
				, offset(Vector3f(0.f, 2.5f, -10.f)) {
			addComponentType(TransformComponent::ID);
			addComponentType(CameraComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override {
			Transform& transform = ((TransformComponent*)components[0])->transform;
			TransformComponent* followT = ecs.getComponent<TransformComponent>(follow);
			
			transform.setTranslation(followT->transform.getTranslation() + offset);
		}
	private:
		ECS& ecs;
		EntityHandle& follow;
		Vector3f offset;
};

class TestScene : public Scene {
	public:
		virtual int load(Game& game) override {
			game.getAssetManager().load("./res/scenes/test.dat");
			AABB vertexArrayAABB = game.getAssetManager().getModel("MonkeyHead").getAABBForElementArray(0);

			game.getRenderContext().setSkybox(&game.getAssetManager().getCubeMap("Skybox"));

			// End scene creation

			InputControl inputAxisX, inputAxisY, inputAxisZ;

			game.getEventHandler().addKeyControl(Input::KEY_A, inputAxisX, -1.f);
			game.getEventHandler().addKeyControl(Input::KEY_D, inputAxisX, 1.f);
			game.getEventHandler().addKeyControl(Input::KEY_LEFT, inputAxisX, -1.f);
			game.getEventHandler().addKeyControl(Input::KEY_RIGHT, inputAxisX, 1.f);

			game.getEventHandler().addKeyControl(Input::KEY_S, inputAxisZ, -1.f);
			game.getEventHandler().addKeyControl(Input::KEY_W, inputAxisZ, 1.f);
			game.getEventHandler().addKeyControl(Input::KEY_DOWN, inputAxisZ, -1.f);
			game.getEventHandler().addKeyControl(Input::KEY_UP, inputAxisZ, 1.f);

			game.getEventHandler().addKeyControl(Input::KEY_Q, inputAxisY, 1.f);
			game.getEventHandler().addKeyControl(Input::KEY_E, inputAxisY, -1.f);

			// Create components
			AABBColliderComponent colliderComponent;
			colliderComponent.aabb = vertexArrayAABB;

			TerrainColliderComponent terrainColliderComponent;
			terrainColliderComponent.halfHeight = vertexArrayAABB.getExtents()[2];
			terrainColliderComponent.radius = vertexArrayAABB.getExtents()[1];

			TransformComponent transformComponent;
			transformComponent.transform.setTranslation(Vector3f(0, 0, 20));

			MovementControlComponent movementControl;
			movementControl.movementControls.emplace_back(Vector3f(10, 0, 0), &inputAxisX);
			movementControl.movementControls.emplace_back(Vector3f(0, 10, 0), &inputAxisY);
			movementControl.movementControls.emplace_back(Vector3f(0, 0, 10), &inputAxisZ);

			SkinnedMeshComponent skinnedMesh;
			skinnedMesh.vertexArray = &game.getAssetManager().getVertexArray("Cowboy");
			skinnedMesh.material = &game.getAssetManager().getMaterial("Bricks");
			skinnedMesh.rig = &game.getAssetManager().getRig("CowboyRig");

			RenderableMeshComponent renderableMesh;
			renderableMesh.vertexArray = &game.getAssetManager().getVertexArray("MonkeyHead");
			renderableMesh.material = &game.getAssetManager().getMaterial("Bricks");

			RenderableText renderableText;
			renderableText.font = &game.getAssetManager().getFont("LucidaTypewriterRegular24");
			renderableText.text = "FPS: --";
			renderableText.color = Vector3f(1.f, 1.f, 1.f);
			renderableText.x = 30.f;
			renderableText.y = 550.f;

			Animator animatorComponent;
			animatorComponent.currentAnim = &game.getAssetManager().getAnimation("Run");

			MotionComponent motionComponent;

			CameraComponent cameraComponent(game.getRenderContext().getCamera());

			// Create entities
			//game.getECS().makeEntity(transformComponent, movementControl, skinnedMesh, motionComponent, colliderComponent);

			// camera controller
			transformComponent.transform.setTranslation(Vector3f(0.f, 0.f, 0.f));
			game.getECS().makeEntity(transformComponent, cameraComponent);

			// terrain
			RenderableMeshComponent terrainMesh;
			terrainMesh.vertexArray = &game.getAssetManager().getVertexArray("Terrain");
			terrainMesh.material = &game.getAssetManager().getMaterial("Grass");

			transformComponent.transform.setTranslation(Vector3f(0.f, -15.f, 0.f));
			transformComponent.transform.setRotation(Quaternion(Vector3f(-1.f, 0.f, 0.f), 0.5f * 3.141592f));
			transformComponent.transform.setScale(Vector3f(50.f, 50.f, 50.f));

			Transform terrainTransform = transformComponent.transform;

			//game.getECS().makeEntity(transformComponent, terrainMesh);
			
			//Water water;

			RenderableMeshComponent waterMesh;

			transformComponent.transform.setScale(Vector3f(1.f, 1.f, 1.f));

			transformComponent.transform.setRotation(Quaternion(Vector3f(0.f, 1.f, 0.f), -1.f * 3.141592f));
			transformComponent.transform.setTranslation(Vector3f(Math::randf() * 10.f - 5.f,
						Math::randf() * 10.f - 5.f, 20.f));

			game.getECS().makeEntity(transformComponent, skinnedMesh, animatorComponent);

			// Cube 1
			//motionComponent.velocity = Vector3f(0, 0, 0.1f);
			motionComponent.acceleration = Vector3f(0.f, -9.81f, 0.f);
			//transformComponent.transform.setTranslation(Vector3f(-3.f, -9.5f, 15.f));
			//transformComponent.transform.setTranslation(Vector3f(0.f, 0.f, 20.f));
			//transformComponent.transform.setRotation(Quaternion(Vector3f(1.f, 0.f, 0.f), 0.5f * MATH_PI));
			//game.getECS().makeEntity(transformComponent, renderableMesh, motionComponent, colliderComponent,
			//		terrainColliderComponent);
			//motionComponent.velocity = Vector3f();
			//game.getECS().makeEntity(transformComponent, renderableMesh);

			// Movable object
			transformComponent.transform.setTranslation(Vector3f(-7.f, 15.f, 15.f));
			//game.getECS().makeEntity(transformComponent, renderableMesh, motionComponent, movementControl, colliderComponent);
			EntityHandle movableObject = game.getECS().makeEntity(transformComponent, renderableMesh,
					motionComponent, movementControl, terrainColliderComponent, colliderComponent);

			// FPS renderer
			game.getECS().makeEntity(renderableText);

			// Create systems
			MovementControlSystem mcSystem;
			MotionSystem motionSystem;
			AnimatorSystem animatorSystem;
			SpinSystem spinSystem;
			FPSUpdateSystem fpsSystem(game);
			
			TerrainCollisionSystem ptcSystem(game.getAssetManager().getModel("Terrain"), terrainTransform);
			FollowCameraSystem cfSystem(game.getECS(), movableObject);

			CameraSystem cameraSystem;
			RenderableMeshSystem rmSystem(game.getRenderContext());
			SkinnedMeshSystem smSystem(game.getRenderContext());
			TextRenderingSystem frSystem(game.getRenderContext());

			game.getMainSystems().addSystem(mcSystem);
			game.getMainSystems().addSystem(motionSystem);
			game.getMainSystems().addSystem(animatorSystem);
			//mainSystems.addSystem(spinSystem);
			game.getMainSystems().addSystem(fpsSystem);
			game.getMainSystems().addSystem(ptcSystem);

			game.getRenderPipeline().addSystem(cfSystem);
			game.getRenderPipeline().addSystem(cameraSystem);
			game.getRenderPipeline().addSystem(rmSystem);
			game.getRenderPipeline().addSystem(smSystem);
			game.getRenderPipeline().addSystem(frSystem);

			game.startLoop();
		}
};
