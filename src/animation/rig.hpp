#pragma once

#include "rendering/indexed-model.hpp"
#include "math/matrix.hpp"

struct Joint {
	inline Joint(const Matrix& localTransform, const String& name,
			uint32 index)
		: localTransform(localTransform)
		, name(name)
		, index(index)
		, parent(-1) {}

	Matrix localTransform;
	String name;
	uint32 index;
	uint32 parent;

	Array<uint32> children;
};

class Rig {
	public:
		static constexpr size_t MAX_JOINTS = 20;
		static constexpr size_t MAX_WEIGHTS = 3;

		inline Rig(const Matrix& globalInverseTransform, IndexedModel* model)
				: globalInverseTransform(globalInverseTransform)
				, model(model)
				, rootJoint(0) {}

		void resetJoints();
		void calcRootJoint();

		inline bool hasJoint(const String& name) const { return nameMap.find(name) != std::end(nameMap); }
		inline void addJoint(const String& name, const Matrix& localTransform);
		
		inline void setJointParent(const String& parent, const String& child);
		inline void setFinalTransform(uint32 jointIndex, const Matrix& transform);

		inline uint32 getJointIndex(const String& name) { return nameMap[name]; }
		inline Joint& getJoint(uint32 i) { return joints[i]; }
		
		inline size_t getNumJoints() const { return joints.size(); }
		inline Joint* getRootJoint() { return &joints[rootJoint]; }
		inline const Matrix& getGlobalInverseTransform() const { return globalInverseTransform; }
		inline Matrix* getTransformSet() { return transformSet; }
	private:
		IndexedModel* model;
		Matrix globalInverseTransform; // TODO: see if I really need this
		
		uint32 rootJoint;

		Array<Joint> joints;
		HashMap<String, uint32> nameMap;
		Matrix transformSet[MAX_JOINTS];
};

inline void Rig::setJointParent(const String& parent, const String& child) {
	joints[nameMap[child]].parent = nameMap[parent];
	joints[nameMap[parent]].children.push_back(nameMap[child]);
}

inline void Rig::addJoint(const String& name, const Matrix& localTransform) {
	nameMap[name] = joints.size();
	joints.emplace_back(localTransform, name, joints.size());
}

inline void Rig::setFinalTransform(uint32 jointIndex, const Matrix& transform) {
	transformSet[jointIndex] = transform;
}
