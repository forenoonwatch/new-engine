#pragma once

#include "util-components.hpp"
#include "game-render-context.hpp"

class ParticleSystem;
class GameRenderContext;

// analogue of the particle data in the particle shader
class Particle {
	public:
		inline Particle()
			: type(0.f)
			, position{0}
			, velocity{0}
			, age(0.f)
			, transScale{0} {}

		static constexpr uintptr TYPE_SIZE = sizeof(float);
		static constexpr uintptr POSITION_SIZE = 3 * sizeof(float);
		static constexpr uintptr VELOCITY_SIZE = 3 * sizeof(float);
		static constexpr uintptr AGE_SIZE = sizeof(float);
		static constexpr uintptr TRANSPARENCY_SIZE = 2 * sizeof(float);
		static constexpr uintptr SCALE_SIZE = 2 * sizeof(float);

		static constexpr uintptr TYPE_OFFSET = 0;
		static constexpr uintptr POSITION_OFFSET = TYPE_OFFSET + TYPE_SIZE;
		static constexpr uintptr VELOCITY_OFFSET = POSITION_OFFSET + POSITION_SIZE;
		static constexpr uintptr AGE_OFFSET = VELOCITY_OFFSET + VELOCITY_SIZE;
		static constexpr uintptr TRANSPARENCY_OFFSET = AGE_OFFSET + AGE_SIZE;
		static constexpr uintptr SCALE_OFFSET = TRANSPARENCY_OFFSET + TRANSPARENCY_SIZE;

		float type;
		float position[3];
		float velocity[3];
		float age;
		float transScale[4];
};

class ParticleEmitter {
	public:
		inline ParticleEmitter(ParticleSystem& particleSystem, uint32 id,
				Texture* texture)
			: particleSystem(&particleSystem)
			, id(id)
			, texture(texture) {}

		/*inline ParticleEmitter(const ParticleEmitter& emitter)
			: particleSystem(emitter.particleSystem)
			, id(emitter.id)
			, texture(emitter.texture) {}

		inline ParticleEmitter(ParticleEmitter&& emitter)
			: particleSystem(emitter.particleSystem)
			, id(emitter.id)
			, texture(emitter.texture) {}*/

		inline void setPosition(float x, float y, float z);
		inline void setPosition(const Vector3f& position);
		
		inline void setVelocity(float x, float y, float z);
		inline void setVelocity(const Vector3f& velocity);

		inline void setTransparency(float start, float end);
		inline void setScale(float start, float end);
	private:
		ParticleSystem* particleSystem;
		uint32 id;
		Texture* texture;

		inline uint32 getOffset() const { return id * sizeof(Particle); }

		friend class ParticleSystem;
};

// contains feedback buffer and additional render data for each particle emitter
class ParticleSystem {
	public:
		inline ParticleSystem(RenderDevice& device, uint32 maxParticles,
				uintptr* attribSizes, uint32 numAttribs)
			: feedbackBuffer(device, maxParticles * sizeof(Particle), attribSizes,
					numAttribs, nullptr, sizeof(Particle)) {}

		inline ParticleEmitter& createEmitter(Texture* texture);
		inline void removeEmitter(ParticleEmitter& emitter);

		inline FeedbackBuffer& getBuffer() { return feedbackBuffer; }
		inline HashMap<Texture*, Array<ParticleEmitter>>& getEmitters() { return emitters; }
	private:
		FeedbackBuffer feedbackBuffer;
		HashMap<Texture*, Array<ParticleEmitter>> emitters;

		NULL_COPY_AND_ASSIGN(ParticleSystem);
};

#include "particles.inl"
