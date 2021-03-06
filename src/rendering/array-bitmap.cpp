#include "array-bitmap.hpp"

#include "core/memory.hpp"
#include "staticLibs/stb_image.h"

ArrayBitmap::ArrayBitmap(int32 widthIn, int32 heightIn)
		: width(widthIn)
		, height(heightIn) {
	assertCheck(width > 0 && height > 0);

	pixels = (int32*)Memory::malloc(getPixelsSize());
}

ArrayBitmap::ArrayBitmap(int32 widthIn, int32 heightIn, int32* pixelsIn)
		: width(widthIn)
		, height(heightIn) {
	assertCheck(width > 0 && height > 0);
	assertCheck(pixelsIn != nullptr);

	uintptr size = getPixelsSize();
	pixels = (int32*)Memory::malloc(size);
	Memory::memcpy(pixels, pixelsIn, size);
}

ArrayBitmap::ArrayBitmap(int32 widthIn, int32 heightIn, int32* pixelsIn, int32 offsetX,
			int32 offsetY, int32 rowOffset)
		: width(widthIn)
		, height(heightIn) {
	assertCheck(width > 0 && height > 0);
	assertCheck(pixelsIn != nullptr);
	assertCheck(offsetX > 0 && offsetY > 0 && rowOffset > 0);

	uintptr size = getPixelsSize();
	pixels = (int32*)Memory::malloc(size);
	int32* pixelsSrc = pixelsIn + offsetY + offsetX * rowOffset;

	for (uintptr i = 0; i < (uintptr)height; ++i, pixels += width, pixelsSrc += rowOffset) {
		Memory::memcpy(pixels, pixelsSrc, (uintptr)width);
	}
}

ArrayBitmap::~ArrayBitmap() {
	pixels = (int32*)Memory::free(pixels);
}

bool ArrayBitmap::load(const String& fileName) {
	int32 texWidth, texHeight, bytesPerPixel;
	uint8* data = stbi_load(fileName.c_str(), &texWidth, &texHeight,
			&bytesPerPixel, 4);

	if (data == nullptr) {
		DEBUG_LOG(LOG_ERROR, LOG_TYPE_IO, "Failed to open file: %s", fileName.c_str());
		return false;
	}

	if (texWidth == width && texHeight == height) {
		Memory::memcpy(pixels, data, getPixelsSize());
	}
	else {
		width = texWidth;
		height = texHeight;
		pixels = (int32*)Memory::free(pixels);
		pixels = (int32*)Memory::malloc(getPixelsSize());
		Memory::memcpy(pixels, data, getPixelsSize());
	}

	stbi_image_free(data);

	return true;
}

bool ArrayBitmap::save(const String& fileName) const {
	// TODO: implement
	return false;
}

void ArrayBitmap::clear(int32 color) {
	Memory::memset(pixels, color, getPixelsSize());
}

void ArrayBitmap::blit(ArrayBitmap& srcAB, int32 x, int32 y) {
	int32* dest = pixels + y * width * sizeof(int32);

	if (x == 0 && srcAB.width == width) {
		Memory::memcpy(dest, srcAB.pixels, srcAB.width * srcAB.height * sizeof(int32));
	}
	else {
		int32 height = srcAB.height;
		int32* src = srcAB.pixels;
		dest += x;

		while (--height) {
			Memory::memcpy(dest, src, srcAB.width * sizeof(int32));
			dest += width;
			src += srcAB.width; 
		}
	}
}
