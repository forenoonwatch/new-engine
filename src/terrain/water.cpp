#include "water.hpp"

#include "classic-noise.hpp"

#define SIDE_LENGTH_VERTICES 100

Water::Water() {
	allocateElement(2); // positions
	allocateElement(4); // adjacent local-space positions
	allocateElement(2); // tex coords

	setInstancedElementStartIndex(3);
	allocateElement(2 * 16); // MVP matrix, normal transform

	//createOcean(9);
	createSimpleGrid();
	//calcTangents();
}

void Water::createSimpleGrid() {
	for (float y = 0.f; y < SIDE_LENGTH_VERTICES; ++y) {
		for (float x = 0.f; x < SIDE_LENGTH_VERTICES; ++x) {
			addVertex(x / SIDE_LENGTH_VERTICES, y / SIDE_LENGTH_VERTICES);
		}
	}

	for (int32 y = 1; y < SIDE_LENGTH_VERTICES; ++y) {
		for (int32 x = 1; x < SIDE_LENGTH_VERTICES; ++x) {
			int32 i0 = y * SIDE_LENGTH_VERTICES + x;
			int32 i1 = y * SIDE_LENGTH_VERTICES - SIDE_LENGTH_VERTICES + x;
			int32 i2 = y * SIDE_LENGTH_VERTICES + x - 1;
			int32 i3 = y * SIDE_LENGTH_VERTICES - SIDE_LENGTH_VERTICES + x - 1;

			addIndices(i2, i1, i0);
			addIndices(i2, i3, i1);
		}
	}
}

void Water::createOcean(int32 verticesPerEdge) {
	const float edgeScale = 1.f / (float)(verticesPerEdge - 1);
	const int verticesPerInnerEdge = verticesPerEdge - 2;
	const int verticesPerLayer = (verticesPerInnerEdge + verticesPerEdge) << 1;
	
	addVertexSkirt(0.5f, edgeScale);
	addVertexRing(0, edgeScale);
	addVertexRing(1, edgeScale);
	addVertexRing(4, 0.5f * edgeScale);
	addVertexRing(5, 0.5f * edgeScale);
	
	addVertexGrid(12, 0.25f * edgeScale);
	
	int32 ind = calcRingIndices(0, verticesPerEdge, verticesPerEdge + 2);
	calcBlendIndices(verticesPerLayer + ind, verticesPerEdge, verticesPerInnerEdge);
	ind = calcRingIndices(ind, verticesPerInnerEdge, verticesPerEdge);
	calcRingIndices(ind - 2 * verticesPerInnerEdge - 2, verticesPerInnerEdge, verticesPerEdge);
	calcBlendIndices(ind + 2 * verticesPerInnerEdge + 2, verticesPerEdge, verticesPerInnerEdge);
	
	int32 innerInd = ind + verticesPerLayer + verticesPerInnerEdge;
	
	for (int32 i = verticesPerInnerEdge; i > 1; i -= 2) {
		calcRingIndices(innerInd + 1, i, i + 2);
		innerInd += ((i + 1) << 2);
	}
	
	calcCenterIndices(innerInd + 1);
}

void Water::addVertexSkirt(float offset, float scale) {
	// top
	addVertex(-offset, -offset);
	
	for (float i = 0; i <= 1; i += scale) {
		addVertex(i, -offset);
	}
	
	addVertex(1 + offset, -offset);
	
	// bottom
	addVertex(-offset, 1 + offset);
	
	for (float i = 0; i <= 1; i += scale) {
		addVertex(i, 1 + offset);
	}
	
	addVertex(1 + offset, 1 + offset);
	
	// left
	for (float i = 0; i <= 1; i += scale) {
		addVertex(-offset, i);
	}
	
	// right
	for (float i = 0; i <= 1; i += scale) {
		addVertex(1 + offset, i);
	}
}

void Water::addVertexRing(float inset, float scale) {
	// top
	for (float i = inset * scale; i <= 1 - inset * scale; i += scale) {
		addVertex(i, inset * scale);
	}
	
	// bottom
	for (float i = inset * scale; i <= 1 - inset * scale; i += scale) {
		addVertex(i, 1 - inset * scale);
	}
	
	// left
	for (float i = inset * scale + scale; i <= 1 - inset * scale - scale; i += scale) {
		addVertex(inset * scale, i);
	}
	
	// right
	for (float i = inset * scale + scale; i <= 1 - inset * scale - scale; i += scale) {
		addVertex(1 - inset * scale, i);
	}
}

void Water::addVertexGrid(float inset, float scale) {
	for (float i = inset; i * scale < 0.5; ++i) {
		addVertexRing(i, scale);
	}
	
	addVertex(0.5f, 0.5f);
}

int32 Water::calcRingIndices(int32 outerBase, int32 innerRing, int32 outerRing) {
	int32 innerBase = outerBase + 4 * outerRing - 5;
	int32 res = outerBase + (innerRing + outerRing) << 1;
	
	addCornerIndices(innerBase, outerBase, innerRing, outerRing);
	addOffsetIndices(innerBase, outerBase, innerRing, outerRing);
	
	// top side
	for (int32 i = 1; i < outerRing - 2; ++i) {
		addIndices(outerBase + i, outerBase + i + 1, innerBase + i);
		addIndices(innerBase + i + 1, innerBase + i, outerBase + i + 1);
	}
	
	outerBase += outerRing;
	innerBase += innerRing;
	
	// bottom side
	for (int32 i = 1; i < outerRing - 2; ++i) {
		addIndices(innerBase + i, outerBase + i + 1, outerBase + i);
		addIndices(outerBase + i + 1, innerBase + i, innerBase + i + 1);
	}
	
	outerBase += outerRing;
	innerBase += innerRing;
	
	// left side
	for (int32 i = 1; i < outerRing - 4; ++i) {
		addIndices(innerBase + i, outerBase + i + 1, outerBase + i);
		addIndices(outerBase + i + 1, innerBase + i, innerBase + i + 1);
	}
	
	outerBase += outerRing - 2;
	innerBase += innerRing - 2;
	
	// right side
	for (int32 i = 1; i < outerRing - 4; ++i) {
		addIndices(outerBase + i, outerBase + i + 1, innerBase + i);
		addIndices(innerBase + i + 1, innerBase + i, outerBase + i + 1);
	}
	
	return res;
}

void Water::calcBlendIndices(int32 outerBase, int32 innerRing, int32 outerRing) {
	int32 innerBase = outerBase + 4 * outerRing - 5;
	
	addCornerIndices(innerBase, outerBase, 9, 7);
	
	// top side
	for (int32 i = 1; i < outerRing - 2; ++i) {
		// left triangle
		addIndices(innerBase + 2 * i, innerBase + 2 * i - 1, outerBase + i);
		// middle triangle
		addIndices(outerBase + i, outerBase + i + 1, innerBase + 2 * i);
		// right triangle
		addIndices(outerBase + i + 1, innerBase + 2 * i + 1, innerBase + 2 * i);
	}
	
	outerBase += outerRing;
	innerBase += innerRing;
	
	// bottom side
	for (int32 i = 1; i < outerRing - 2; ++i) {
		// left triangle
		addIndices(outerBase + i, innerBase + 2 * i - 1, innerBase + 2 * i);
		// middle triangle
		addIndices(innerBase + 2 * i, outerBase + i + 1, outerBase + i);
		// right triangle
		addIndices(innerBase + 2 * i, innerBase + 2 * i + 1, outerBase + i + 1);
	}
	
	outerBase += outerRing;
	innerBase += innerRing;
	
	// left side
	for (int32 i = 0; i < outerRing - 3; ++i) {
		// left triangle
		if (i > 0) {
			addIndices(outerBase + i, innerBase + 2 * i, innerBase + 2 * i + 1);
		}
		else {
			addIndices(outerBase + i, innerBase - 2 * innerRing + 1, innerBase + 2 * i + 1);
		}
		
		// middle triangle
		addIndices(innerBase + 2 * i + 1, outerBase + i + 1, outerBase + i);
		
		// right triangle
		if (i < outerRing - 4) {
			addIndices(innerBase + 2 * i + 1, innerBase + 2 * i + 2, outerBase + i + 1);
		}
		else {
			addIndices(innerBase + 2 * i + 1, innerBase - innerRing + 1, outerBase + i + 1);
		}
	}
	
	outerBase += outerRing - 2;
	innerBase += innerRing - 2;
	
	// right side
	for (int32 i = 0; i < outerRing - 3; ++i) {
		// left triangle
		if (i > 0) {
			addIndices(innerBase + 2 * i + 1, innerBase + 2 * i, outerBase + i);
		}
		else {
			addIndices(innerBase + 2 * i + 1, innerBase - 2 * innerRing + 2, outerBase + i);
		}
		
		// middle triangle
		addIndices(outerBase + i, outerBase + i + 1, innerBase + 2 * i + 1);
		
		// right triangle
		if (i < outerRing - 4) {
			addIndices(outerBase + i + 1, innerBase + 2 * i + 2, innerBase + 2 * i + 1);
		}
		else {
			addIndices(outerBase + i + 1, innerBase - innerRing + 2, innerBase + 2 * i + 1);
		}
	}
}

void Water::calcCenterIndices(int32 baseOffset) {
	for (int32 i = 0; i < 2; ++i) {
		addIndices(baseOffset + 8, baseOffset + i, baseOffset + i + 1);
		addIndices(baseOffset + i + 4, baseOffset + i + 3, baseOffset + 8);
	}
	
	addIndices(baseOffset + 6, baseOffset, baseOffset + 8);
	addIndices(baseOffset + 3, baseOffset + 6, baseOffset + 8);
	addIndices(baseOffset + 2, baseOffset + 7, baseOffset + 8);
	addIndices(baseOffset + 8, baseOffset + 7, baseOffset + 5);
}

void Water::addCornerIndices(int32 innerBase, int32 outerBase, int32 innerRing,
		int32 outerRing) {
	// top left
	addIndices(outerBase, outerBase + 1, innerBase + 1);
	addIndices(innerBase + 1, outerBase + 2 * outerRing, outerBase);
	
	// top right
	addIndices(outerBase + outerRing - 2, outerBase + outerRing - 1, innerBase + innerRing);
	addIndices(outerBase + 3 * outerRing - 2, innerBase + innerRing, outerBase + outerRing - 1);
	
	// bottom left
	addIndices(outerBase + 3 * outerRing - 3, innerBase + innerRing + 1, outerBase + outerRing + 1);
	addIndices(outerBase + 3 * outerRing - 3, outerBase + outerRing + 1, outerBase + outerRing);
	
	// bottom right
	addIndices(innerBase + 2 * innerRing, outerBase + 4 * outerRing - 5, outerBase + 2 * outerRing - 1);
	addIndices(outerBase + 2 * outerRing - 1, outerBase + 2 * outerRing - 2, innerBase + 2 * innerRing);
}

void Water::addOffsetIndices(int32 innerBase, int32 outerBase, int32 innerRing,
		int32 outerRing) {
	// top left
	addIndices(outerBase + 2 * outerRing, innerBase + 1, outerBase + 2 * outerRing + 1);
	addIndices(innerBase + 1, innerBase + 2 * innerRing + 1, outerBase + 2 * outerRing + 1);
	
	// top right
	addIndices(innerBase + innerRing, outerBase + 3 * outerRing - 2, outerBase + 3 * outerRing - 1);
	addIndices(innerBase + innerRing, outerBase + 3 * outerRing - 1, innerBase + 3 * innerRing - 1);
	
	// bottom left
	addIndices(innerBase + 3 * innerRing - 2, innerBase + innerRing + 1, outerBase + 3 * outerRing - 3);
	addIndices(innerBase + 3 * innerRing - 2, outerBase + 3 * outerRing - 3, outerBase + 3 * outerRing - 4);
	
	// bottom right
	addIndices(outerBase + 4 * outerRing - 5, innerBase + 2 * innerRing, innerBase + 4 * innerRing - 4);
	addIndices(outerBase + 4 * outerRing - 6, outerBase + 4 * outerRing - 5, innerBase + 4 * innerRing - 4);
}

inline void Water::addVertex(float x, float y) {
	//addElement2f(0, (x + 0.5f) / 2.f, (y + 0.5f) / 2.f);
	addElement2f(0, 300 * (x + 0.5f) / 2.f - 150.f, 300 * (y + 0.5f) / 2.f - 150.f);
	addElement4f(1, 0.f, 0.f, 0.f, 0.f);
	addElement2f(2, (x + 0.5f) / 2.f, (y + 0.5f) / 2.f);
	//addElement3f(0, 300 * (x - 0.5f), 0.f, 300 * (y - 0.5f));
	//addElement2f(1, (x + 0.5f) / 2.f, (y + 0.5f) / 2.f); // TODO: add tex coords
	//addElement3f(2, 0, 1, 0);
}

inline void Water::addIndices(uint32 a, uint32 b, uint32 c) {
	IndexedModel::addIndices3i(c, b, a);

	setAdjacent(a, b, c);
	setAdjacent(b, a, c);
	setAdjacent(c, a, b);
}

inline void Water::setAdjacent(uint32 a, uint32 b, uint32 c) {
	float aX = getElement(0, 2 * a), aY = getElement(0, 2 * a + 1);
	float bX = getElement(0, 2 * b), bY = getElement(0, 2 * b + 1);
	float cX = getElement(0, 2 * c), cY = getElement(0, 2 * c + 1);

	setElement4f(1, a, bX - aX, bY - aY, cX - aX, cY - aY);
}
