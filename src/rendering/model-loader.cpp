#include "model-loader.hpp"

#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "math/math.hpp"

#define EPSILON		0.0001f
#define WITHIN_EPSILON(n) \
	((n) < EPSILON && (n) > -EPSILON)

#define CONVERT_VEC3(v) \
	Vector3f(v.x, v.y, v.z)
#define CONVERT_QUAT(q) \
	Quaternion(q.x, q.y, q.z, q.w)

#define HASH_TIME(time) \
	(int32)(1e4 * time)

struct VertexJointData {
	uint32 vertexIDs[Rig::MAX_WEIGHTS] = {0};
	float weights[Rig::MAX_WEIGHTS] = {0};
};

static void loadJoints(Rig& rig, const aiMesh* model, Array<VertexJointData>& bones);
static void calcRigHierarchy(Rig& rig, const aiNode* rootNode);

static void genStaticMesh(IndexedModel& newModel, const aiMesh* model);
static void genSkinnedMesh(IndexedModel& newModel, const aiMesh* model, Rig& rig);

static bool loadAnimation(Animation& newAnim, const aiAnimation* anim);

bool ModelLoader::loadModels(const String& fileName,
			Array<IndexedModel>& models, Array<uint32>& modelMaterialIndices,
			Array<MaterialSpec>& materials, Array<Rig>& rigs, Array<Animation>& animations) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName.c_str(), 
											 aiProcess_Triangulate |
											 aiProcess_GenSmoothNormals | 
											 aiProcess_FlipUVs |
											 aiProcess_CalcTangentSpace);
	
	if (!scene) {
		DEBUG_LOG(LOG_TYPE_IO, LOG_ERROR,  "Mesh load failed!: %s",
				fileName.c_str());
		return false;
	}

	Matrix globalInverseTransform = Matrix::transformMatrix(Vector3f(),
			Quaternion(Vector3f(-1.f, 0.f, 0.f), MATH_HALF_PI), Vector3f(1.f, 1.f, 1.f));
	
	for (uint32 j = 0; j < scene->mNumMeshes; ++j) {
		const aiMesh* model = scene->mMeshes[j];
		modelMaterialIndices.push_back(model->mMaterialIndex);

		IndexedModel newModel;

		if (model->HasBones()) {
			rigs.emplace_back(globalInverseTransform, &newModel);
			Rig& rig = rigs.back();

			genSkinnedMesh(newModel, model, rig);
			calcRigHierarchy(rig, scene->mRootNode);
			rig.calcRootJoint();
		}
		else {
			genStaticMesh(newModel, model);
		}
		
		for (uint32 i = 0; i < model->mNumFaces; ++i) {
			const aiFace& face = model->mFaces[i];
			assert(face.mNumIndices == 3);

			newModel.addIndices3i(face.mIndices[0], face.mIndices[1],
					face.mIndices[2]);
		}

		models.push_back(newModel);
	}

	for (uint32 i = 0; i < scene->mNumMaterials; i++) {
		const aiMaterial* material = scene->mMaterials[i];
		MaterialSpec spec;
		
		// Currently only handles diffuse textures.
		aiString texturePath;
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
				material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath)
				!= AI_SUCCESS) {
			String str(texturePath.data);
			spec.textureNames["diffuse"] = str;
		}

		materials.push_back(spec);
	}

	if (scene->HasAnimations()) {
		for (uint32 i = 0; i < scene->mNumAnimations; ++i) {
			animations.emplace_back(String(scene->mAnimations[i]->mName.data));

			if (!loadAnimation(animations.back(), scene->mAnimations[i])) {
				DEBUG_LOG(LOG_TYPE_IO, LOG_ERROR,  "Animation load failed!: %s - %s",
						fileName.c_str(), scene->mAnimations[i]->mName.C_Str());
			}
		}
	}
	
	return true;
}

bool ModelLoader::loadOnlyModels(const String& fileName,
		Array<IndexedModel>& models) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName.c_str(), 
											 aiProcess_Triangulate |
											 aiProcess_GenSmoothNormals | 
											 aiProcess_FlipUVs |
											 aiProcess_CalcTangentSpace);
	
	if (!scene) {
		DEBUG_LOG(LOG_TYPE_IO, LOG_ERROR,  "Mesh load failed!: %s",
				fileName.c_str());
		return false;
	}

	for (uint32 j = 0; j < scene->mNumMeshes; ++j) {
		const aiMesh* model = scene->mMeshes[j];

		IndexedModel newModel;
		genStaticMesh(newModel, model);
		
		for (uint32 i = 0; i < model->mNumFaces; ++i) {
			const aiFace& face = model->mFaces[i];
			assert(face.mNumIndices == 3);

			newModel.addIndices3i(face.mIndices[0], face.mIndices[1],
					face.mIndices[2]);
		}

		models.push_back(newModel);
	}

	return true;
}

static void genStaticMesh(IndexedModel& newModel, const aiMesh* model) {
	newModel.initStaticMesh();

	const aiVector3D aiZeroVector(0.f, 0.f, 0.f);
	for (uint32 i = 0; i < model->mNumVertices; ++i) {
		const aiVector3D pos = model->mVertices[i];
		const aiVector3D normal = model->mNormals[i];
		const aiVector3D texCoord = model->HasTextureCoords(0)
			? model->mTextureCoords[0][i] : aiZeroVector;
		const aiVector3D tangent = model->mTangents[i];

		newModel.addElement3f(0, pos.x, pos.y, pos.z);
		newModel.addElement2f(1, texCoord.x, texCoord.y);
		newModel.addElement3f(2, normal.x, normal.y, normal.z);
		newModel.addElement3f(3, tangent.x, tangent.y, tangent.z);
	}
}

static void genSkinnedMesh(IndexedModel& newModel, const aiMesh* model, Rig& rig) {
	newModel.initSkinnedMesh();

	Array<VertexJointData> joints;
	joints.resize(model->mNumVertices);

	loadJoints(rig, model, joints);

	const aiVector3D aiZeroVector(0.f, 0.f, 0.f);

	for (uint32 i = 0; i < model->mNumVertices; ++i) {
		const aiVector3D pos = model->mVertices[i];
		const aiVector3D normal = model->mNormals[i];
		const aiVector3D texCoord = model->HasTextureCoords(0)
			? model->mTextureCoords[0][i] : aiZeroVector;
		const aiVector3D tangent = model->mTangents[i];

		const uint32* vertexIDs = joints[i].vertexIDs;
		const float* weights = joints[i].weights;

		newModel.addElement3f(0, pos.x, pos.y, pos.z);
		newModel.addElement2f(1, texCoord.x, texCoord.y);
		newModel.addElement3f(2, normal.x, normal.y, normal.z);
		newModel.addElement3f(3, tangent.x, tangent.y, tangent.z);

		newModel.addElement3f(4, vertexIDs[0], vertexIDs[1], vertexIDs[2]);
		newModel.addElement3f(5, weights[0], weights[1], weights[2]);
	}
}

static void addJointData(VertexJointData& vbd, uint32 jointIndex, float weight) {
	for (uint32 i = 0; i < Rig::MAX_WEIGHTS; ++i) {
		if (vbd.weights[i] == 0.f) {
			vbd.vertexIDs[i] = jointIndex;
			vbd.weights[i] = weight;
			return;
		}
	}
}

static void loadJoints(Rig& rig, const aiMesh* model, Array<VertexJointData>& joints) {
	for (uint32 i = 0; i < model->mNumBones; ++i) {
		uint32 jointIndex = 0;
		String jointName(model->mBones[i]->mName.data);

		if (rig.hasJoint(jointName)) {
			jointIndex = rig.getJointIndex(jointName);
		}
		else {
			jointIndex = rig.getNumJoints();
			rig.addJoint(jointName, Matrix(model->mBones[i]->mOffsetMatrix[0]));
		}

		for (uint32 j = 0; j < model->mBones[i]->mNumWeights; ++j) {
			const aiVertexWeight& vertexWeight = model->mBones[i]->mWeights[j];
			addJointData(joints[vertexWeight.mVertexId], jointIndex, vertexWeight.mWeight);
		}
	}
}

static void calcRigHierarchy(Rig& rig, const aiNode* rootNode) {
	const String parentName(rootNode->mName.data);
	const bool isParent = rig.hasJoint(parentName);

	for (uint32 i = 0; i < rootNode->mNumChildren; ++i) {
		if (isParent) {
			const String jointName(rootNode->mChildren[i]->mName.data);

			if (rig.hasJoint(jointName)) {
				rig.setJointParent(parentName, jointName);
			}
		}

		calcRigHierarchy(rig, rootNode->mChildren[i]);
	}
}

static bool loadAnimation(Animation& newAnim, const aiAnimation* anim) {
	HashMap<int32, KeyFrame> keyFrames;
	float time;

	for (uint32 i = 0; i < anim->mNumChannels; ++i) {
		const aiNodeAnim* channel = anim->mChannels[i];
		const String jointName(channel->mNodeName.data);

		if (channel->mNumPositionKeys != channel->mNumRotationKeys
				|| channel->mNumScalingKeys != channel->mNumRotationKeys
				|| channel->mNumPositionKeys != channel->mNumScalingKeys) {
			DEBUG_LOG(LOG_ERROR, LOG_TYPE_IO, "Animation load error - non-equivalent amount of keys");
			return false;
		}

		for (uint32 j = 0; j < channel->mNumRotationKeys; ++j) {
			time = channel->mRotationKeys[j].mTime;

			if (!WITHIN_EPSILON(time - channel->mPositionKeys[j].mTime)
					|| !WITHIN_EPSILON(time - channel->mScalingKeys[j].mTime)) {
				return false;
			}

			time /= anim->mTicksPerSecond;

			auto frame = keyFrames.find(HASH_TIME(time));

			if (frame == std::end(keyFrames)) {
				KeyFrame keyFrame(time);
				keyFrame.addJointTransform(jointName, Transform(
							CONVERT_VEC3(channel->mPositionKeys[j].mValue),
							CONVERT_QUAT(channel->mRotationKeys[j].mValue),
							CONVERT_VEC3(channel->mScalingKeys[j].mValue)));

				keyFrames.emplace(HASH_TIME(time), keyFrame);
			}
			else {
				frame->second.addJointTransform(jointName, Transform(
							CONVERT_VEC3(channel->mPositionKeys[j].mValue),
							CONVERT_QUAT(channel->mRotationKeys[j].mValue),
							CONVERT_VEC3(channel->mScalingKeys[j].mValue)));
			}
		}
	}

	for (auto it = std::begin(keyFrames), end = std::end(keyFrames); it != end; ++it) {
		newAnim.addKeyFrame(it->second);
	}

	 return true;
}
