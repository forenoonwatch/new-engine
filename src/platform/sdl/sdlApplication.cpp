#include "sdlApplication.hpp"
#include "core/common.hpp"
#include <SDL2/SDL.h>

uint32 SDLApplication::numInstances = 0;

SDLApplication* SDLApplication::create() {
	const uint32 flags = SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS;
	uint32 initialized = SDL_WasInit(flags);

	if (initialized != flags &&
			SDL_Init(flags) != 0) {
		DEBUG_LOG("SDLApplication", LOG_ERROR, "SDL_Init: %s", SDL_GetError());
		return nullptr;
	}

	return new SDLApplication();
}

SDLApplication::SDLApplication() {
	numInstances++;
	isAppRunning = true;
}

SDLApplication::~SDLApplication() {
	numInstances--;

	if (numInstances == 0) {
		SDL_Quit();
	}
}

void SDLApplication::processMessages(double delta, IApplicationEventHandler& handler) {
	SDL_Event e;
	(void)delta;

	while (SDL_PollEvent(&e)) {
		switch(e.type) {
			case SDL_KEYDOWN:
				handler.onKeyDown(e.key.keysym.scancode, e.key.repeat != 0);
				break;
			case SDL_KEYUP:
				handler.onKeyUp(e.key.keysym.scancode, e.key.repeat != 0);
				break;
			case SDL_MOUSEBUTTONDOWN:
				handler.onMouseDown(e.button.button, e.button.clicks);
				break;
			case SDL_MOUSEBUTTONUP:
				handler.onMouseUp(e.button.button, e.button.clicks);
				break;
			case SDL_MOUSEMOTION:
				handler.onMouseMove(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
				break;
			case SDL_MOUSEWHEEL:
				handler.onMouseWheelMove(e.wheel.y);
				break;
			case SDL_QUIT:
				isAppRunning = false;
				break;
			default:
				break;
		};
	}
}

bool SDLApplication::isRunning() {
	return isAppRunning;
}

