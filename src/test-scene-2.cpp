#include "test-scene-2.hpp"

#include "core/input.hpp"

#include "gamecs/renderable-mesh.hpp"
#include "gamecs/movement-control.hpp"

#include "terrain/water.hpp"

void SpinSystem::updateComponents(float delta, BaseECSComponent** components) {
	counter += 0.5f * delta;

	Transform& transform = ((TransformComponent*)components[0])->transform;

	transform.setTranslation(Vector3f(rc.getCamera().position[0], 0, rc.getCamera().position[2]));

	//DEBUG_LOG_TEMP("%.2f", counter);
	//transform.setRotation(Quaternion(Vector3f(1.f, 0.f, 0.f), counter));
	//transform.setRotation(Quaternion(Vector3f(1.f, 0.f, 0.f), 1.65f));
}

int TestScene2::load(Game& game) {
	game.getAssetManager().load("./res/scenes/test.dat");
	
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

	MovementControlComponent movementControl;
	movementControl.movementControls.emplace_back(Vector3f(100, 0, 0), &inputAxisX);
	movementControl.movementControls.emplace_back(Vector3f(0, 100, 0), &inputAxisY);
	movementControl.movementControls.emplace_back(Vector3f(0, 0, 100), &inputAxisZ);

	Water water;
	VertexArray va(game.getRenderDevice(), water, RenderDevice::USAGE_STATIC_DRAW);

	RenderableMeshComponent renderableMesh;
	renderableMesh.vertexArray = &game.getAssetManager().getVertexArray("MonkeyHead");
	renderableMesh.material = &game.getAssetManager().getMaterial("Water");

	TransformComponent transformComponent;
	transformComponent.transform.setTranslation(Vector3f(0, 0, 0));
	//transformComponent.transform.setScale(Vector3f(30, 30, 30));
	//transformComponent.transform.setRotation(Quaternion(Vector3f(1.f, 0.f, 0.f), 1.65f));

	game.getECS().makeEntity(transformComponent, renderableMesh);

	CameraComponent cameraComponent(game.getRenderContext().getCamera());
	transformComponent.transform.setTranslation(Vector3f(0.f, 20.f, 0.f));
	transformComponent.transform.setRotation(Quaternion(Vector3f(1.f, 0.f, 0.f), 0.4f));

	MotionComponent motionComponent;

	game.getECS().makeEntity(transformComponent, cameraComponent, motionComponent, movementControl);

	RenderableMeshSystem rmSystem(game.getRenderContext());
	CameraSystem cameraSystem;
	SpinSystem ss(game.getRenderContext());
	MovementControlSystem mcSystem;
	MotionSystem motionSystem;

	//game.getMainSystems().addSystem(ss);
	game.getMainSystems().addSystem(mcSystem);
	game.getMainSystems().addSystem(motionSystem);

	game.getRenderPipeline().addSystem(rmSystem);
	game.getRenderPipeline().addSystem(cameraSystem);

	game.startLoop();
}
