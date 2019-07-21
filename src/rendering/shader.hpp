#pragma once

#include "uniform-buffer.hpp"
#include "material.hpp"
#include "sampler.hpp"

#include "math/matrix.hpp"

class Shader {
	public:
		inline Shader(RenderDevice& deviceIn, const String& text)
			: device(&deviceIn)
			, deviceID(device->createShaderProgram(text)) {}

		inline void setBufferBlock(const String& name, uint32 block);
		inline void setUniformBuffer(const String& name, UniformBuffer& buffer, uint32 index);

		inline void setSampler(const String& name, Texture& texture, Sampler& sampler,
				uint32 unit, enum RenderDevice::TextureType textureType = RenderDevice::TEXTURE_TYPE_2D);
		inline void setSampler(const String& name, uint32 texture, Sampler& sampler,
				uint32 unit, enum RenderDevice::TextureType textureType = RenderDevice::TEXTURE_TYPE_2D);
		inline void setSampler(const String& name, uint32 texture, uint32 sampler,
				uint32 unit, enum RenderDevice::TextureType textureType = RenderDevice::TEXTURE_TYPE_2D);
		
		inline void setMaterial(Material& material, Sampler& sampler);

		inline uint32 getId();

		inline virtual ~Shader() {
			deviceID = device->releaseShaderProgram(deviceID);
		}
	private:
		RenderDevice* device;
		uint32 deviceID;

		NULL_COPY_AND_ASSIGN(Shader);
};

inline uint32 Shader::getId() {
	return deviceID;
}

inline void Shader::setBufferBlock(const String& name, uint32 block) {
	device->setShaderUniformBlockBinding(deviceID, name, block);
}

inline void Shader::setUniformBuffer(const String& name, UniformBuffer& buffer, uint32 index) {
	device->setShaderUniformBuffer(deviceID, name, buffer.getId(), index);
}

inline void Shader::setSampler(const String& name, Texture& texture,
		Sampler& sampler, uint32 unit, enum RenderDevice::TextureType textureType) {
	device->setShaderSampler(deviceID, name, texture.getId(), sampler.getId(), unit, textureType);
}

inline void Shader::setSampler(const String& name, uint32 texture,
		Sampler& sampler, uint32 unit, enum RenderDevice::TextureType textureType) {
	device->setShaderSampler(deviceID, name, texture, sampler.getId(), unit, textureType);
}

inline void Shader::setSampler(const String& name, uint32 texture, uint32 sampler,
		uint32 unit, enum RenderDevice::TextureType textureType) {
	device->setShaderSampler(deviceID, name, texture, sampler, unit, textureType);
}

inline void Shader::setMaterial(Material& material, Sampler& sampler) {
	device->setShaderSampler(deviceID, "diffuse", material.getDiffuse().getId(),
			sampler.getId(), 0, RenderDevice::TEXTURE_TYPE_2D);
	device->setShaderSampler(deviceID, "normalMap", material.getNormal().getId(),
			sampler.getId(), 1, RenderDevice::TEXTURE_TYPE_2D);
}
