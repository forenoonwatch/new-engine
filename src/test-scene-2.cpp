#include "test-scene-2.hpp"

#include "core/timing.hpp"
#include "core/input.hpp"

#include "gamecs/renderable-mesh.hpp"
#include "gamecs/movement-control.hpp"
#include "gamecs/orbit-camera.hpp"

//#include "animation/animator.hpp"
//#include "gamecs/skinned-mesh.hpp"

#include "gamecs/text-renderer.hpp"

#include "terrain/water.hpp"

#include "classic-noise.hpp"

#define AMPLITUDE 3.f
#define TIME_SCALE 0.2f
#define SPACE_SCALE 0.1f
#define SHIP_LENGTH 2.f
#define SHIP_WIDTH 1.f

void SpinSystem::updateComponents(float delta, BaseECSComponent** components) {
	counter += 0.5f * delta;

	Transform& transform = ((TransformComponent*)components[0])->transform;

	transform.setTranslation(Vector3f(rc.getCamera().position[0], 0, rc.getCamera().position[2]));

	//DEBUG_LOG_TEMP("%.2f", counter);
	//transform.setRotation(Quaternion(Vector3f(1.f, 0.f, 0.f), counter));
	//transform.setRotation(Quaternion(Vector3f(1.f, 0.f, 0.f), 1.65f));
}

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

class FloatSystem : public BaseECSSystem {
	public:
		FloatSystem()
				: BaseECSSystem() {
			addComponentType(TransformComponent::ID);
			addComponentType(RenderableMeshComponent::ID);
			addComponentType(ParticleEmitterComponent::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override {
			Transform& transform = ((TransformComponent*)components[0])->transform;
			ParticleEmitterComponent* pec = (ParticleEmitterComponent*)components[2];

			Matrix m = transform.toMatrix();

			Vector3f fwd = transform.getTranslation() + (m[2] * Vector::load1f(SHIP_LENGTH));
			Vector3f bak = transform.getTranslation() + (m[2] * Vector::load1f(-SHIP_LENGTH));
			Vector3f rht = transform.getTranslation() + (m[0] * Vector::load1f(SHIP_WIDTH));
			Vector3f lft = transform.getTranslation() + (m[0] * Vector::load1f(-SHIP_WIDTH));

			fwd = Vector3f(fwd[0], height(fwd[0], fwd[2]), fwd[2]);
			bak = Vector3f(bak[0], height(bak[0], bak[2]), bak[2]);
			lft = Vector3f(lft[0], height(lft[0], lft[2]), lft[2]);
			rht = Vector3f(rht[0], height(rht[0], rht[2]), rht[2]);

			rht = (rht - lft).normalized();
			fwd = (fwd - bak).normalized();

			Vector3f up = fwd.cross(rht).normalized();
			Vector3f pos = transform.getTranslation();

			transform.setTranslation(Vector3f(pos[0], height(pos[0], pos[2]), pos[2])
					+ Vector3f(0.f, 0.f, delta));
			transform.setRotation(transform.getRotation().slerp(Quaternion::fromAxes(rht, up, fwd), 0.2));
			//pec->emitter->setPosition(pos[0], 0.f, pos[2]);

			Vector3f emitPos = pos + (fwd * -2.1f) + (up * -0.1f) + (rht * 0.5f);
			pec->emitter->setPosition(emitPos);
			//pec->emitter->setVelocity(fwd * 5.f);
		}
	private:
		inline float height(float x, float y) const {
			float t = TIME_SCALE * (float)Time::getTime();
			x *= SPACE_SCALE;
			y *= SPACE_SCALE;

			//return 0;
			return AMPLITUDE * cnoise(x + t, y + t)
					* cnoise(-0.75 * x + 0.5 * t, -0.75 * y + 0.5 * t);
		}
};

int TestScene2::load(Game& game) {
	game.getAssetManager().load("./res/scenes/test.dat");
	
	game.getRenderContext().setSkybox(&game.getAssetManager().getCubeMap("Skybox"));

	InputControl inputAxisX, inputAxisY, inputAxisZ;

	//game.getEventHandler().addKeyControl(Input::KEY_A, inputAxisX, -1.f);
	//game.getEventHandler().addKeyControl(Input::KEY_D, inputAxisX, 1.f);
	//game.getEventHandler().addKeyControl(Input::KEY_LEFT, inputAxisX, -1.f);
	//game.getEventHandler().addKeyControl(Input::KEY_RIGHT, inputAxisX, 1.f);

	game.getEventHandler().addMouseMoveControl(0, inputAxisX, 0.5f);
	game.getEventHandler().addMouseMoveControl(1, inputAxisZ, 0.1f);
	game.getEventHandler().addMouseWheelControl(inputAxisY, 1.f);

	//game.getEventHandler().addKeyControl(Input::KEY_S, inputAxisZ, -1.f);
	//game.getEventHandler().addKeyControl(Input::KEY_W, inputAxisZ, 1.f);
	//game.getEventHandler().addKeyControl(Input::KEY_DOWN, inputAxisZ, -1.f);
	//game.getEventHandler().addKeyControl(Input::KEY_UP, inputAxisZ, 1.f);

	//game.getEventHandler().addKeyControl(Input::KEY_Q, inputAxisY, 1.f);
	//game.getEventHandler().addKeyControl(Input::KEY_E, inputAxisY, -1.f);

	MovementControlComponent movementControl;
	movementControl.movementControls.emplace_back(Vector3f(100, 0, 0), &inputAxisX);
	movementControl.movementControls.emplace_back(Vector3f(0, 100, 0), &inputAxisY);
	movementControl.movementControls.emplace_back(Vector3f(0, 0, 100), &inputAxisZ);

	Water water;
	VertexArray oceanVA(game.getRenderDevice(), water, RenderDevice::USAGE_STATIC_DRAW);

	game.getRenderContext().setOcean(&oceanVA, &game.getAssetManager().getTexture("WaterNormal"),
			&game.getAssetManager().getTexture("WaterDUDV"));

	RenderableMeshComponent renderableMesh;
	renderableMesh.vertexArray = &game.getAssetManager().getVertexArray("Hull");
	renderableMesh.material = &game.getAssetManager().getMaterial("Wood");

	TransformComponent transformComponent;
	transformComponent.transform.setTranslation(Vector3f(0, 0, 10));
	transformComponent.transform.setRotation(Quaternion(Vector3f(0.f, 1.f, 0.f), 2.27f));
	//transformComponent.transform.setScale(Vector3f(30, 30, 30));
	//transformComponent.transform.setRotation(Quaternion(Vector3f(1.f, 0.f, 0.f), 1.65f));
	//

	//ParticleEmitter pe(game.getRenderDevice(), 200, attribSizes, 5);
	ParticleEmitter& pe = game.getRenderContext().getParticleSystem()
			.createEmitter(&game.getAssetManager().getTexture("Smoke"));
	pe.setPosition(1.f, 0.f, 10.f);
	//pe.setVelocity(0.f, 2.5f, 0.f);
	pe.setVelocity(0.f, 0.f, 0.f);
	pe.setTransparency(1.f, 0.25f);
	pe.setScale(0.f, 0.3f);

	//ParticleEmitterComponent pec;
	//pec.emitter = &pe;
	//pec.shader = &game.getAssetManager().getShader("particle-shader");
	//pec.texture = &game.getAssetManager().getTexture("Smoke");

	EntityHandle ship = game.getECS().makeEntity(transformComponent, renderableMesh, pec);

	CameraComponent cameraComponent(game.getRenderContext().getCamera());
	transformComponent.transform.setTranslation(Vector3f(0.f, 0.f, 0.f));
	transformComponent.transform.setRotation(Quaternion(Vector3f(1.f, 0.f, 0.f), 0.3f));

	MotionComponent motionComponent;
	EntityPointerComponent epc;
	epc.entityPtr = &ship;

	CameraControlComponent ccc;
	ccc.movementControls.emplace_back(Vector3f(-0.05f, 0, 0), &inputAxisZ);
	ccc.movementControls.emplace_back(Vector3f(0, 0.05f, 0), &inputAxisX);
	ccc.movementControls.emplace_back(Vector3f(0, 0, -1), &inputAxisY);
	ccc.offsets = Vector3f(0.f, 0.f, 4.f);

	game.getECS().makeEntity(transformComponent, cameraComponent,
			/*motionComponent,*/ movementControl, epc, ccc);

	/*SkinnedMeshComponent skinnedMesh;
	skinnedMesh.vertexArray = &game.getAssetManager().getVertexArray("Cowboy");
	skinnedMesh.material = &game.getAssetManager().getMaterial("Bricks");
	skinnedMesh.rig = &game.getAssetManager().getRig("CowboyRig");
	
	Animator animatorComponent;
	animatorComponent.currentAnim = &game.getAssetManager().getAnimation("Run");
	
	transformComponent.transform.setTranslation(Vector3f());
	transformComponent.transform.setRotation(Quaternion(0, 0, 0, 1));

	game.getECS().makeEntity(transformComponent, skinnedMesh, animatorComponent);*/

	/*Particle baseParticle;
	baseParticle.position[0] = 1.f;
	baseParticle.position[2] = 10.f;
	baseParticle.velocity[1] = 2.5f;
	baseParticle.transScale[0] = 1.f;
	baseParticle.transScale[1] = 0.f;
	baseParticle.transScale[2] = 0.f;
	baseParticle.transScale[3] = 0.5f;*/

	//FeedbackBuffer fb(game.getRenderDevice(), 200 * sizeof(Particle),
	//		attribSizes, 5, 3, (const float*)&baseParticle, sizeof(Particle));

	//game.getECS().makeEntity(pec);

	RenderableText renderableText;
	renderableText.font = &game.getAssetManager().getFont("LucidaTypewriterRegular24");
	renderableText.text = "FPS: --";
	renderableText.color = Vector3f(1.f, 1.f, 1.f);
	renderableText.x = 30.f;
	renderableText.y = 550.f;

	game.getECS().makeEntity(renderableText);

	CameraSystem cameraSystem;
	SpinSystem ss(game.getRenderContext());
	MovementControlSystem mcSystem;
	MotionSystem motionSystem;
	FloatSystem fs;
	OrbitCameraSystem cfs(game.getECS(), game.getEventHandler());
	//AnimatorSystem animatorSystem;
	FPSUpdateSystem fpsSystem(game);

	RenderableMeshSystem rmSystem(game.getRenderContext());
	//SkinnedMeshSystem smSystem(game.getRenderContext());
	TextRenderingSystem frSystem(game.getRenderContext());
	//ParticleRenderSystem prSystem(game.getRenderContext());

	//game.getMainSystems().addSystem(ss);
	game.getMainSystems().addSystem(fs);
	//game.getMainSystems().addSystem(mcSystem);
	//game.getMainSystems().addSystem(motionSystem);
	game.getMainSystems().addSystem(cfs);
	game.getMainSystems().addSystem(fpsSystem);
	//game.getMainSystems().addSystem(animatorSystem);

	game.getRenderPipeline().addSystem(cameraSystem);
	game.getRenderPipeline().addSystem(rmSystem);
	//game.getRenderPipeline().addSystem(prSystem);
	//game.getRenderPipeline().addSystem(smSystem);
	//game.getRenderPipeline().addSystem(frSystem);

	game.startLoop();
	return 0;
}
