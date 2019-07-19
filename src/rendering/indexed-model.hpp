#pragma once

#include "render-device.hpp"

#include "math/aabb.hpp"
#include "math/vector.hpp"

class IndexedModel {
	public:
		IndexedModel()
			: instancedElementsStartIndex((uint32)(-1)) {}

		uint32 createVertexArray(RenderDevice& device,
				enum RenderDevice::BufferUsage usage) const;

		void initStaticMesh();
		void initSkinnedMesh();
		void initQuad();
		void initCube();

		void allocateElement(uint32 elementSize);
		void setInstancedElementStartIndex(uint32 elementIndex);

		void addElement1f(uint32 elementIndex, float e0);
		void addElement2f(uint32 elementIndex, float e0, float e1);
		void addElement3f(uint32 elementIndex, float e0, float e1, float e2);
		void addElement4f(uint32 elementIndex, float e0, float e1, float e2, float e3);

		void setElement4f(uint32 elementIndex, uint32 arrayIndex, float e0, float e1, float e2, float e3);

		void addIndices1i(uint32 i0);
		void addIndices2i(uint32 i0, uint32 i1);
		void addIndices3i(uint32 i0, uint32 i1, uint32 i2);
		void addIndices4i(uint32 i0, uint32 i1, uint32 i2, uint32 i3);

		void calcNormals();
		void calcTangents();

		bool intersectsRay(const Vector3f& pos, const Vector3f& dir,
				Vector3f* intersectPos, Vector3f* normal) const;

		inline AABB getAABBForElementArray(uint32 index) {
			return AABB(&elements[index][0], elements[index].size() / elementSizes[index]);
		}

		inline float getElement(uint32 elementIndex, uint32 arrayIndex) const {
			return elements[elementIndex][arrayIndex];
		}

		uint32 getNumIndices() const;
	private:
		Array<uint32> indices;
		Array<uint32> elementSizes;
		Array<Array<float>> elements;

		uint32 instancedElementsStartIndex;
};
