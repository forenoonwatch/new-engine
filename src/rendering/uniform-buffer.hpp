#pragma once

#include "render-device.hpp"

class UniformBuffer {
	public:
		inline UniformBuffer(RenderDevice& deviceIn, uintptr dataSize,
				enum RenderDevice::BufferUsage usage, const void* data = nullptr)
			: device(&deviceIn)
			, deviceID(device->createUniformBuffer(data, dataSize, usage))
			, size(dataSize) {}

		inline void update(const void* data, uintptr dataSize);
		inline void update(const void* data) { update(data, size); }
		inline void update(const void* data, uintptr offset, uintptr dataSize);
		
		inline uint32 getId();

		inline ~UniformBuffer() {
			deviceID = device->releaseUniformBuffer(deviceID);
		}
	private:
		RenderDevice* device;
		uint32 deviceID;
		uintptr size;

		NULL_COPY_AND_ASSIGN(UniformBuffer);
};

inline uint32 UniformBuffer::getId() {
	return deviceID;
}

inline void UniformBuffer::update(const void* data, uintptr dataSize) {
	device->updateUniformBuffer(deviceID, data, dataSize);
}

inline void UniformBuffer::update(const void* data, uintptr offset,
		uintptr dataSize) {
	device->updateUniformBuffer(deviceID, data, offset, dataSize);
}
