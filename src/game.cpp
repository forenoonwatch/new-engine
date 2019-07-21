#include "game.hpp"

#include "core/timing.hpp"

#define UNLOCK_FPS true

int Game::loadScene(Scene* scene) {
	return scene->load(*this);
}

void Game::startLoop() {
	//const Color color(0.0f, 0.15f, 0.3f);
	const float frameTime = 1.0 / 60.0;

	uint32 fps = 0;

	double lastTime = Time::getTime();
	double fpsTimeCounter = 0.0;
	double updateTimer = 1.0;

	double currentTime, passedTime;
	bool shouldRender;

	while (app->isRunning()) {
		currentTime = Time::getTime();
		passedTime = currentTime - lastTime;
		lastTime = currentTime;

		fpsTimeCounter += passedTime;
		updateTimer += passedTime;

		if (fpsTimeCounter >= 1.0) {
			//double msPerFrame = 1000.0 / (double)fps;
			
			//DEBUG_LOG("FPS", "NONE", "%f ms (%d fps)", msPerFrame, fps);
			
			setFPS(fps);

			fpsTimeCounter = 0;
			fps = 0;
		}
		
		shouldRender = UNLOCK_FPS;
		
		while (updateTimer >= frameTime) {
			app->processMessages(frameTime, eventHandler);
			interactionWorld->processInteractions(frameTime);
			ecs.updateSystems(mainSystems, frameTime);
			eventHandler.flush();

			updateTimer -= frameTime;
			shouldRender = true;
		}
		
		if (shouldRender) {
			renderContext->clear(true);
			//renderContext->clear(color, true);
			ecs.updateSystems(renderPipeline, frameTime);
			renderContext->flush();
			
			window->present();
			
			++fps;
		}
		else {
			Time::sleep(1);
		}
	}
}
