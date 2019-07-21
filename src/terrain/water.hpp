#pragma once

#include "rendering/indexed-model.hpp"

class Water : public IndexedModel {
	public:
		Water();

	private:
		void createOcean(int32);
		void createSimpleGrid();

		void addVertexSkirt(float, float);
		void addVertexRing(float, float);
		void addVertexGrid(float, float);

		int32 calcRingIndices(int32, int32, int32);
		void calcBlendIndices(int32, int32, int32);
		void calcCenterIndices(int32);

		void addCornerIndices(int32, int32, int32, int32);
		void addOffsetIndices(int32, int32, int32, int32);

		void addVertex(float, float);
		void addIndices(uint32, uint32, uint32);
		void setAdjacent(uint32, uint32, uint32);
};
