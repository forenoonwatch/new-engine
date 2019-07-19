#pragma once

#include "render-device.hpp"

class UniformBuffer {
	public:
		inline UniformBuffer(RenderDevice& deviceIn, uintptr dataSize,
				enum RenderDevice::BufferUsage usage, const void* data = nullptr)
			: device(&deviceIn)
			, deviceId(device->createUniformBuffer(data, dataSize, usage))
			, size(dataSize) {}

		inline void update(const void* data, uintptr dataSize);
		inline void update(const void* data) { update(data, size); }
		inline void update(const void* data, uintptr offset, uintptr dataSize);
		
		inline uint32 getId();

		inline ~UniformBuffer() {
			deviceId = device->releaseUniformBuffer(deviceId);
		}
	private:
		RenderDevice* device;
		uint32 deviceId;
		uintptr size;

		NULL_COPY_AND_ASSIGN(UniformBuffer);
};

inline uint32 UniformBuffer::getId() {
	return deviceId;
}

inline void UniformBuffer::update(const void* data, uintptr dataSize) {
	device->updateUniformBuffer(deviceId, data, dataSize);
}

inline void UniformBuffer::update(const void* data, uintptr offset,
		uintptr dataSize) {
	device->updateUniformBuffer(deviceId, data, offset, dataSize);
}
