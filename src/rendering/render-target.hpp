#pragma once

#include <stdexcept>

#include "texture.hpp"

class RenderTarget {
	public:
		inline RenderTarget(RenderDevice& deviceIn)
			: device(&deviceIn)
			, deviceId(0) {}
		
		inline RenderTarget(RenderDevice& deviceIn,
				Texture& texture, uint32 width, uint32 height,
				enum RenderDevice::FramebufferAttachment attachment
					= RenderDevice::ATTACHMENT_COLOR,
				uint32 attachmentNumber = 0, uint32 mipLevel = 0)
			: device(&deviceIn)
			, deviceId(device->createRenderTarget(texture.getId(), width, height,
						attachment, attachmentNumber, mipLevel)) {
			checkCompressed(texture);
		}

		inline RenderTarget(RenderDevice& deviceIn,
				Texture& texture,
				enum RenderDevice::FramebufferAttachment attachment
					= RenderDevice::ATTACHMENT_COLOR,
				uint32 attachmentNumber = 0, uint32 mipLevel = 0)
			: device(&deviceIn)
			, deviceId(device->createRenderTarget(texture.getId(),
						texture.getWidth(), texture.getHeight(),
						attachment, attachmentNumber, mipLevel)) {
			checkCompressed(texture);
		}

		inline void checkCompressed(const Texture& texture);

		inline uint32 getId() { return deviceId; }

		inline ~RenderTarget() {
			deviceId = device->releaseRenderTarget(deviceId);
		}
	private:
		RenderDevice* device;
		uint32 deviceId;

		NULL_COPY_AND_ASSIGN(RenderTarget);
};

inline void RenderTarget::checkCompressed(const Texture& texture) {
#ifdef DEBUG
	if (texture.isCompressed()) {
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "Compressed textures cannot be used as render targets!");
		throw std::invalid_argument("Compressed textures cannot be used as render targets!");
	}

	if (texture.hasMipmaps()) {
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_WARNING, "Rendering to a texture with mipmaps will NOT render to all mipmap levels! Unexpected results may occur.");
	}
#endif
}
