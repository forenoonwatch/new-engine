#pragma once

#include "render-device.hpp"

class FeedbackBuffer {
	public:
		inline FeedbackBuffer(RenderDevice& deviceIn, uintptr bufferSize,
				uintptr* attribSizes, uint32 numAttribs, uint32 outputSize,
				const float* initialData = nullptr, uintptr initialDataSize = 0)
			: device(&deviceIn)
			, deviceID(device->createFeedbackBuffer(initialData, initialDataSize,
						bufferSize, attribSizes, numAttribs, outputSize)) {}

		inline uint32 getID() { return deviceID; }

		inline ~FeedbackBuffer() {
			device->releaseFeedbackBuffer(deviceID);
		}
	private:
		RenderDevice* device;
		uint32 deviceID;
};
