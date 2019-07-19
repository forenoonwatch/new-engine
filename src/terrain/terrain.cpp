#include "terrain.hpp"

Terrain::Terrain(float cellSize, uint32 cellRadius) {
	initStaticMesh();

	float* heightMap = (float*)Memory::malloc(cellRadius * cellRadius * sizeof(float));

	for (uint32 i = 0; i < cellRadius * cellRadius; ++i) {
		heightMap[i] = Math::randf();
	}



	Memory::free(heightMap);
}
