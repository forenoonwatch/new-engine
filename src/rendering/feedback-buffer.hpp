#pragma once

#include "render-device.hpp"

class FeedbackBuffer {
	public:
		inline FeedbackBuffer(RenderDevice& deviceIn, uintptr bufferSize,
				uintptr* attribSizes, uint32 numAttribs,
				const float* initialData = nullptr, uintptr initialDataSize = 0)
			: device(&deviceIn)
			, deviceID(device->createFeedbackBuffer(initialData, initialDataSize,
						bufferSize, attribSizes, numAttribs)) {}

		inline void update(const void* data, uintptr dataSize);
		inline void update(const void* data, uintptr offset, uintptr dataSize);

		inline uint32 getID() { return deviceID; }

		inline ~FeedbackBuffer() {
			device->releaseFeedbackBuffer(deviceID);
		}
	private:
		RenderDevice* device;
		uint32 deviceID;

		NULL_COPY_AND_ASSIGN(FeedbackBuffer);
};

inline void FeedbackBuffer::update(const void* data, uintptr dataSize) {
	device->writeFeedbackBufferData(deviceID, data, dataSize);
}

inline void FeedbackBuffer::update(const void* data,
		uintptr offset, uintptr dataSize) {
	device->writeFeedbackBufferData(deviceID, data, offset, dataSize);
}
