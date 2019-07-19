#pragma once

#include "game.hpp"

class Game;

class Scene {
	public:
		virtual int load(Game& game) = 0;
};
