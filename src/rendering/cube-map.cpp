#include "cube-map.hpp"

CubeMap::CubeMap(RenderDevice& device, const Array<String>& fileNames)
		: device(device) {
	assertCheck(fileNames.size() == 6);

	textureID = device.createCubeMap();

	for (uint32 i = 0; i < 6; ++i) {
		ArrayBitmap bmp;
		bmp.load(fileNames[i]);

		device.addTextureToCubeMap(textureID, RenderDevice::CUBE_MAP_POSITIVE_X + i,
				bmp.getWidth(), bmp.getHeight(), bmp.getPixelArray());
	}
}

CubeMap::CubeMap(RenderDevice& device, ArrayBitmap* bitmaps)
		: device(device) {
	textureID = device.createCubeMap();

	for (uint32 i = 0; i < 6; ++i) {
		device.addTextureToCubeMap(textureID, RenderDevice::CUBE_MAP_POSITIVE_X + i,
				bitmaps[i].getWidth(), bitmaps[i].getHeight(), bitmaps[i].getPixelArray());
	}
}

CubeMap::~CubeMap() {
	textureID = device.releaseTexture2D(textureID);
}
