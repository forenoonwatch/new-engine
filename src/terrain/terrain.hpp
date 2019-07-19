#pragma once

#include "rendering/indexed-model.hpp"

class Terrain : public IndexedModel {
	public:
		Terrain(float cellSize, uint32 cellRadius);
	private:
};
