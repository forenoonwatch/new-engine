#pragma once

#include "render-device.hpp"
#include "array-bitmap.hpp"
#include "ddstexture.hpp"
#include "font.hpp"

class Texture {
	public:
		inline Texture(RenderDevice& deviceIn, const ArrayBitmap& texData,
				enum RenderDevice::PixelFormat internalPixelFormat,
				bool generateMipmaps, bool shouldCompress)
			: device(&deviceIn)
			, texId(device->createTexture2D(texData.getWidth(),
					texData.getHeight(), texData.getPixelArray(),
					RenderDevice::FORMAT_RGBA,
					internalPixelFormat, generateMipmaps, shouldCompress))
			, width((uint32)texData.getWidth())
			, height((uint32)texData.getHeight())
			, compressed(shouldCompress)
			, mipmaps(generateMipmaps) {}

		inline Texture(RenderDevice& deviceIn, const Font& font)
			: device(&deviceIn)
			, texId(device->createTexture2D(font.getWidth(), font.getHeight(),
					font.getData(), RenderDevice::FORMAT_R, RenderDevice::FORMAT_R,
					false, false))
			, width((uint32)font.getWidth())
			, height((uint32)font.getHeight())
			, compressed(false)
			, mipmaps(false) {}

		inline Texture(RenderDevice& deviceIn, const DDSTexture& ddsTexture)
			: device(&deviceIn)
			, texId(device->createDDSTexture2D(ddsTexture.getWidth(), ddsTexture.getHeight(),
						ddsTexture.getBuffer(), ddsTexture.getFourCC(),
						ddsTexture.getMipMapCount()))
			, width(ddsTexture.getWidth())
			, height(ddsTexture.getHeight())
			, compressed(true)
			, mipmaps(ddsTexture.getMipMapCount() > 1) {}

		inline Texture(RenderDevice& deviceIn, const String& fileName,
					enum RenderDevice::PixelFormat internalPixelFormat,
					bool generateMipmaps, bool shouldCompress)
				: device(&deviceIn)
				, compressed(shouldCompress)
				, mipmaps(generateMipmaps) {
			ArrayBitmap bmp(fileName);

			texId = device->createTexture2D(bmp.getWidth(), bmp.getHeight(),
					bmp.getPixelArray(), RenderDevice::FORMAT_RGBA, internalPixelFormat,
					generateMipmaps, shouldCompress);
			width = bmp.getWidth();
			height = bmp.getHeight();
		}

		inline Texture(RenderDevice& deviceIn,
				enum RenderDevice::PixelFormat internalPixelFormat,
				uint32 width, uint32 height)
			: device(&deviceIn)
			, texId(device->createTexture2D(width, height, nullptr,
						RenderDevice::FORMAT_RGBA, internalPixelFormat, false, false))
			, width(width)
			, height(height)
			, compressed(false)
			, mipmaps(false) {}
			
		inline ~Texture() {
			texId = device->releaseTexture2D(texId);
		}

		inline uint32 getId();
		inline uint32 getWidth() const;
		inline uint32 getHeight() const;
		inline bool isCompressed() const;
		inline bool hasMipmaps() const;
	private:
		RenderDevice* device;
		uint32 texId;
		uint32 width;
		uint32 height;
		bool compressed;
		bool mipmaps;

		NULL_COPY_AND_ASSIGN(Texture);
};

inline uint32 Texture::getId() {
	return texId;
}

inline uint32 Texture::getWidth() const {
	return width;
}

inline uint32 Texture::getHeight() const {
	return height;
}

inline bool Texture::isCompressed() const {
	return compressed;
}

inline bool Texture::hasMipmaps() const {
	return mipmaps;
}

