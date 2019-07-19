#pragma once

#include "uniform-buffer.hpp"
#include "material.hpp"
#include "sampler.hpp"

#include "math/matrix.hpp"

class Shader {
	public:
		inline Shader(RenderDevice& deviceIn, const String& text)
			: device(&deviceIn)
			, deviceId(device->createShaderProgram(text)) {}

		inline void setUniformBuffer(const String& name, UniformBuffer& buffer);

		inline void setSampler(const String& name, Texture& texture, Sampler& sampler,
				uint32 unit, enum RenderDevice::TextureType textureType = RenderDevice::TEXTURE_TYPE_2D);
		inline void setSampler(const String& name, uint32 texture, Sampler& sampler,
				uint32 unit, enum RenderDevice::TextureType textureType = RenderDevice::TEXTURE_TYPE_2D);
		inline void setSampler(const String& name, uint32 texture, uint32 sampler,
				uint32 unit, enum RenderDevice::TextureType textureType = RenderDevice::TEXTURE_TYPE_2D);
		
		inline void setMaterial(Material& material, Sampler& sampler);

		inline uint32 getId();

		inline virtual ~Shader() {
			deviceId = device->releaseShaderProgram(deviceId);
		}
	private:
		RenderDevice* device;
		uint32 deviceId;

		NULL_COPY_AND_ASSIGN(Shader);
};

inline uint32 Shader::getId() {
	return deviceId;
}

inline void Shader::setUniformBuffer(const String& name, UniformBuffer& buffer) {
	device->setShaderUniformBuffer(deviceId, name, buffer.getId());
}

inline void Shader::setSampler(const String& name, Texture& texture,
		Sampler& sampler, uint32 unit, enum RenderDevice::TextureType textureType) {
	device->setShaderSampler(deviceId, name, texture.getId(), sampler.getId(), unit, textureType);
}

inline void Shader::setSampler(const String& name, uint32 texture,
		Sampler& sampler, uint32 unit, enum RenderDevice::TextureType textureType) {
	device->setShaderSampler(deviceId, name, texture, sampler.getId(), unit, textureType);
}

inline void Shader::setSampler(const String& name, uint32 texture, uint32 sampler,
		uint32 unit, enum RenderDevice::TextureType textureType) {
	device->setShaderSampler(deviceId, name, texture, sampler, unit, textureType);
}

inline void Shader::setMaterial(Material& material, Sampler& sampler) {
	device->setShaderSampler(deviceId, "diffuse", material.getDiffuse().getId(),
			sampler.getId(), 0, RenderDevice::TEXTURE_TYPE_2D);
	device->setShaderSampler(deviceId, "normalMap", material.getNormal().getId(),
			sampler.getId(), 1, RenderDevice::TEXTURE_TYPE_2D);
}
