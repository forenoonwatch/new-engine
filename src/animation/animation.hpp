#pragma once

#include "containers/string.hpp"
#include "containers/hash-map.hpp"
#include "containers/array.hpp"

#include "math/transform.hpp"

#include <algorithm>

class KeyFrame {
	public:
		inline KeyFrame(float time)
			: time(time) {}

		//inline KeyFrame(const KeyFrame& keyFrame)
		//	: time(keyFrame.time)
		//	, jointTransforms(std::move(keyFrame.jointTransforms)) {}

		inline float getTime() const { return time; }

		inline void addJointTransform(const String& name, const Transform& transform) {
			jointTransforms[name] = transform;
		}

		inline const Transform& getJointTransform(const String& name) { return jointTransforms[name]; }
	private:
		float time;
		HashMap<String, Transform> jointTransforms;
};

class Animation {
	public:
		inline Animation(const String& name)
			: name(name) {}

		//inline Animation(const Animation& animation)
		//	: name(animation.name)
		//	, keyFrames(std::move(animation.keyFrames)) {}

		void addKeyFrame(const KeyFrame& keyFrame);

		inline KeyFrame& getKeyFrame(uint32 i) { return keyFrames[i]; }
		inline const KeyFrame& getKeyFrame(uint32 i) const { return keyFrames[i]; }
		inline uint32 getNumFrames() const { return keyFrames.size(); }

		inline const String& getName() const { return name; }
	private:
		String name;
		Array<KeyFrame> keyFrames;
};
