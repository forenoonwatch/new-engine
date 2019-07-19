#pragma once

#include "array-bitmap.hpp"
#include "render-device.hpp"

class CubeMap {
	public:
		CubeMap(RenderDevice& device, const Array<String>& fileNames);
		CubeMap(RenderDevice& device, ArrayBitmap* bitmaps);

		inline uint32 getId();

		~CubeMap();
	private:
		RenderDevice& device;
		uint32 textureID;
};

inline uint32 CubeMap::getId() {
	return textureID;
}
