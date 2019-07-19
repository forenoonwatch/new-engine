#include "indexed-model.hpp"

#include <cfloat>

#include "math/intersects.hpp"

void IndexedModel::initStaticMesh() {
	allocateElement(3); // Positions
	allocateElement(2); // TexCoords
	allocateElement(3); // Normals
	allocateElement(3); // Tangents

	setInstancedElementStartIndex(4); // Begin instanced data
	//allocateElement(16); // Model view projection matrix
	//allocateElement(16); // Model matrix
	allocateElement(2 * 16); // MVP, normal transform
}

void IndexedModel::initSkinnedMesh() {
	allocateElement(3); // Positions
	allocateElement(2); // TexCoords
	allocateElement(3); // Normals
	allocateElement(3); // Tangents

	allocateElement(3); // Joint Indices
	allocateElement(3); // Joint Weights

	setInstancedElementStartIndex(6); // Begin instanced data
	//allocateElement(16); // Transform matrix
	allocateElement(2 * 16); // MVP, normal transform
}

void IndexedModel::initQuad() {
	allocateElement(4); // <vertX, vertY, baseUVX, baseUVY>
	setInstancedElementStartIndex(1);
	allocateElement(4); // <posX, posY, uvX, uvY>
	allocateElement(4); // <sizeX, sizeY, uvSizeX, uvSizeY>
	
	addElement4f(0, 0.f, 1.f, 0.f, 0.f); // 00 : 0
	addElement4f(0, 0.f, 0.f, 0.f, 1.f); // 01: 1
	addElement4f(0, 1.f, 0.f, 1.f, 1.f); // 11: 2
	addElement4f(0, 1.f, 1.f, 1.f, 0.f); // 10: 3

	addIndices3i(0, 2, 1);
	addIndices3i(0, 3, 2);
}

void IndexedModel::initCube() {
	allocateElement(3); // Positions
	setInstancedElementStartIndex(1);
	allocateElement(16); // Model view projection matrix
}

void IndexedModel::addElement1f(uint32 elementIndex, float e0) {
	assertCheck(elementIndex < elementSizes.size());

	elements[elementIndex].push_back(e0);
}

void IndexedModel::addElement2f(uint32 elementIndex, float e0, float e1) {
	assertCheck(elementIndex < elementSizes.size());

	elements[elementIndex].push_back(e0);
	elements[elementIndex].push_back(e1);
}

void IndexedModel::addElement3f(uint32 elementIndex, float e0, float e1, float e2) {
	assertCheck(elementIndex < elementSizes.size());

	elements[elementIndex].push_back(e0);
	elements[elementIndex].push_back(e1);
	elements[elementIndex].push_back(e2);
}

void IndexedModel::addElement4f(uint32 elementIndex, float e0, float e1, float e2, float e3) {
	assertCheck(elementIndex < elementSizes.size());

	elements[elementIndex].push_back(e0);
	elements[elementIndex].push_back(e1);
	elements[elementIndex].push_back(e2);
	elements[elementIndex].push_back(e3);
}

void IndexedModel::setElement4f(uint32 elementIndex, uint32 arrayIndex,
		float e0, float e1, float e2, float e3) {
	assertCheck(elementIndex < elementSizes.size());

	arrayIndex *= elementSizes[elementIndex];

	elements[elementIndex][arrayIndex] = e0;
	elements[elementIndex][arrayIndex + 1] = e1;
	elements[elementIndex][arrayIndex + 2] = e2;
	elements[elementIndex][arrayIndex + 3] = e3;
}

void IndexedModel::addIndices1i(uint32 i0) {
	indices.push_back(i0);
}

void IndexedModel::addIndices2i(uint32 i0, uint32 i1) {
	indices.push_back(i0);
	indices.push_back(i1);
}

void IndexedModel::addIndices3i(uint32 i0, uint32 i1, uint32 i2) {
	indices.push_back(i0);
	indices.push_back(i1);
	indices.push_back(i2);
}

void IndexedModel::addIndices4i(uint32 i0, uint32 i1, uint32 i2, uint32 i3) {
	indices.push_back(i0);
	indices.push_back(i1);
	indices.push_back(i2);
	indices.push_back(i3);
}

uint32 IndexedModel::getNumIndices() const {
	return indices.size();
}

void IndexedModel::allocateElement(uint32 elementSize) {
	elementSizes.push_back(elementSize);
	elements.push_back(Array<float>());
}

void IndexedModel::setInstancedElementStartIndex(uint32 elementIndex) {
	instancedElementsStartIndex = elementIndex;
}

//void IndexedModel::clear()
//{
//	instancedElementsStartIndex = -1;
//	indices.clear();
//	elementSizes.clear();
//	elements.clear();
//}

uint32 IndexedModel::createVertexArray(RenderDevice& device,
		enum RenderDevice::BufferUsage usage) const {
	uint32 numVertexComponents = elementSizes.size();
	uint32 numInstanceComponents = instancedElementsStartIndex == ((uint32)-1) ?
		0 : (numVertexComponents - instancedElementsStartIndex);
	numVertexComponents -= numInstanceComponents;

	Array<const float*> vertexDataArray;

	for (uint32 i = 0; i < numVertexComponents; ++i) {
		vertexDataArray.push_back(&(elements[i][0]));
	}

	const float** vertexData = &vertexDataArray[0];
	const uint32* vertexElementSizes = &elementSizes[0];
	
	uint32 numVertices = elements[0].size()/vertexElementSizes[0];
	uint32 numIndices = indices.size();

	return device.createVertexArray(vertexData, vertexElementSizes,
			numVertexComponents, numInstanceComponents, numVertices, &indices[0],
			numIndices, usage);
}

void IndexedModel::calcNormals() {
	for (uint32 i = 0; i < indices.size(); i += 3) {
		uint32 i0 = indices[i];
		uint32 i1 = indices[i + 1];
		uint32 i2 = indices[i + 2];

		Vector3f v1 = Vector3f(&elements[0][3 * i1]) - Vector3f(&elements[0][3 * i0]);
		Vector3f v2 = Vector3f(&elements[0][3 * i2]) - Vector3f(&elements[0][3 * i0]);

		Vector3f normal = v1.cross(v2).normalized(); // TODO: see if I really need to normalize this

		if (elements[2].size() <= 3 * i0) {
			addElement3f(2, normal[0], normal[1], normal[2]);
		}
		else {
			Vector3f v = Vector3f(&elements[2][3 * i0]) + normal;

			elements[2][3 * i0] = v[0];
			elements[2][3 * i0 + 1] = v[1];
			elements[2][3 * i0 + 2] = v[2];
		}

		if (elements[2].size() <= 3 * i1) {
			addElement3f(2, normal[0], normal[1], normal[2]);
		}
		else {
			Vector3f v = Vector3f(&elements[2][3 * i1]) + normal;

			elements[2][3 * i1] = v[0];
			elements[2][3 * i1 + 1] = v[1];
			elements[2][3 * i1 + 2] = v[2];
		}

		if (elements[2].size() <= 3 * i2) {
			addElement3f(2, normal[0], normal[1], normal[2]);
		}
		else {
			Vector3f v = Vector3f(&elements[2][3 * i2]) + normal;

			elements[2][3 * i2] = v[0];
			elements[2][3 * i2 + 1] = v[1];
			elements[2][3 * i2 + 2] = v[2];
		}
	}

	for (uint32 i = 0; i < elements[2].size(); i += 3) {
		Vector3f v(&elements[2][i]);
		v = v.normalized();

		elements[2][i] = v[0];
		elements[2][i + 1] = v[1];
		elements[2][i + 2] = v[2];
	}
}

void IndexedModel::calcTangents() {
	for (uint32 i = 0; i < indices.size(); i += 3) {
		uint32 i0 = indices[i];
		uint32 i1 = indices[i + 1];
		uint32 i2 = indices[i + 2];

		Vector3f edge1 = Vector3f(&elements[0][3 * i1]) - Vector3f(&elements[0][3 * i0]);
		Vector3f edge2 = Vector3f(&elements[0][3 * i2]) - Vector3f(&elements[0][3 * i0]);

		float deltaU1 = elements[1][2 * i1] - elements[1][2 * i0];
		float deltaV1 = elements[1][2 * i1 + 1] - elements[1][2 * i0 + 1];
		float deltaU2 = elements[1][2 * i2] - elements[1][2 * i0];
		float deltaV2 = elements[1][2 * i2 + 1] - elements[1][2 * i0 + 1];

		float dividend = (deltaU1 * deltaV2 - deltaU2 * deltaV1);
		float f = dividend == 0 ? 0.f : 1.f / dividend;

		Vector3f tangent(f * (deltaV2 * edge1[0] - deltaV1 * edge2[0]),
				f * (deltaV2 * edge1[1] - deltaV1 * edge2[1]),
				f * (deltaV2 * edge1[2] - deltaV1 * edge2[2]));

		if (elements[3].size() <= 3 * i0) {
			addElement3f(3, tangent[0], tangent[1], tangent[2]);
		}
		else {
			Vector3f t = Vector3f(&elements[3][3 * i0]) + tangent;

			elements[3][3 * i0] = t[0];
			elements[3][3 * i0 + 1] = t[1];
			elements[3][3 * i0 + 2] = t[2];
		}

		if (elements[3].size() <= 3 * i1) {
			addElement3f(3, tangent[0], tangent[1], tangent[2]);
		}
		else {
			Vector3f t = Vector3f(&elements[3][3 * i1]) + tangent;

			elements[3][3 * i1] = t[0];
			elements[3][3 * i1 + 1] = t[1];
			elements[3][3 * i1 + 2] = t[2];
		}

		if (elements[3].size() <= 3 * i2) {
			addElement3f(3, tangent[0], tangent[1], tangent[2]);
		}
		else {
			Vector3f t = Vector3f(&elements[3][3 * i2]) + tangent;

			elements[3][3 * i2] = t[0];
			elements[3][3 * i2 + 1] = t[1];
			elements[3][3 * i2 + 2] = t[2];
		}
	}

	for (uint32 i = 0; i < elements[3].size(); i += 3) {
		Vector3f t(&elements[3][i]);
		t = t.normalized();

		elements[3][i] = t[0];
		elements[3][i + 1] = t[1];
		elements[3][i + 2] = t[2];
	}
}

bool IndexedModel::intersectsRay(const Vector3f& pos, const Vector3f& dir,
		Vector3f* intersectPos, Vector3f* normal) const {
	uint32 i0, i1, i2;

	float t;
	float minT = FLT_MAX;
	bool intersected = false;
	
	for (uint32 i = 0; i < indices.size(); i += 3) {
		i0 = indices[i];
		i1 = indices[i + 1];
		i2 = indices[i + 2];

		Vector3f v0 = Vector3f(&elements[0][3 * i0]);
		Vector3f v1 = Vector3f(&elements[0][3 * i1]);
		Vector3f v2 = Vector3f(&elements[0][3 * i2]);

		if (Intersects::intersectTriangle(pos, dir, v0, v1, v2, t, normal) && t < minT) {
			minT = t;
			intersected = true;
		}
	}

	if (intersected) {
		intersectPos->set(pos + dir * minT);
	}

	return intersected;
}
