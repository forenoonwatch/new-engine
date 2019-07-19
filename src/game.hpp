#pragma once

#include "core/application.hpp"
#include "core/window.hpp"

#include "scene.hpp"

#include "gamecs/game-render-context.hpp"
#include "game-event-handler.hpp"
//#include "interaction-world.hpp"
#include "asset-manager.hpp"

#include "collision/aabb-collision-world.hpp"
#include "collision/aabb-collision-handler.hpp"

#include <memory>

class Scene;

class Game {
	public:
		Game(Application* app, Window* window, GameRenderContext* renderContext,
			RenderDevice* device, AssetManager* assetManager)
				: app(app)
				, window(window)
				, renderContext(renderContext)
				, assetManager(assetManager)
				, renderDevice(device)
				, interactionWorld(std::make_unique<AABBCollisionWorld>(ecs)) {
			ecs.addListener(interactionWorld.get());
			interactionWorld->addInteraction(&collisionHandler);
		}

		int loadScene(Scene* scene);

		inline GameEventHandler& getEventHandler() { return eventHandler; }
		inline GameRenderContext& getRenderContext() { return *renderContext; }
		inline AssetManager& getAssetManager() { return *assetManager; }
		inline ECS& getECS() { return ecs; }
		inline RenderDevice& getRenderDevice() { return *renderDevice; }

		inline ECSSystemList& getMainSystems() { return mainSystems; }
		inline ECSSystemList& getRenderPipeline() { return renderPipeline; }

		inline void setFPS(uint32 fps) { this->fps = fps; }
		inline uint32 getFPS() const { return fps; }

		void startLoop();
	private:
		Application* app;
		Window* window;
		GameRenderContext* renderContext;
		AssetManager* assetManager;
		RenderDevice* renderDevice;

		GameEventHandler eventHandler;

		ECS ecs;
		ECSSystemList mainSystems;
		ECSSystemList renderPipeline;

		std::unique_ptr<InteractionWorld> interactionWorld;
		AABBCollisionHandler collisionHandler;

		uint32 fps;
};
