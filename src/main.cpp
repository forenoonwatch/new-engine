#include <iostream>

#include "tests.hpp"
#include "game.hpp"
#include "test-scene-2.hpp"

// NOTE: Profiling reveals that in the current instanced rendering system:
// - Updating the buffer takes more time than
// - Calculating the transforms which takes more time than
// - Performing the instanced draw
static int runApp(Application* app) {
	Tests::runTests();

	Window window(*app, 800, 600, "ECS Based Game Engine");
	RenderDevice device(window);
	
	Matrix perspective(Matrix::perspective(Math::toRadians(70.0f/2.0f),
				4.0f/3.0f, 0.1f, 1000.0f));

	RenderDevice::DrawParams drawParams;
	drawParams.primitiveType = RenderDevice::PRIMITIVE_TRIANGLES;
	drawParams.faceCulling = RenderDevice::FACE_CULL_BACK;
	drawParams.shouldWriteDepth = true;
	drawParams.depthFunc = RenderDevice::DRAW_FUNC_LEQUAL;
//	drawParams.sourceBlend = RenderDevice::BLEND_FUNC_ONE;
//	drawParams.destBlend = RenderDevice::BLEND_FUNC_ONE;

	AssetManager assetManager(device);
	RenderTarget target(device);
	GameRenderContext gameRenderContext(device, target, drawParams,
			assetManager, perspective);

	Game game(app, &window, &gameRenderContext, &device, &assetManager);

	TestScene2 testScene;

	int res = game.loadScene((Scene*)&testScene);
	//game.startLoop();

	return res;
}

#ifdef main
#undef main
#endif
int main(int argc, char** argv) {
	Application* app = Application::create();
	int result = runApp(app);
	delete app;

	return result;
}

