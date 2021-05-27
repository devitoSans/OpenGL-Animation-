#include "Animation.h"

//Animation Setup
Animation::Animation(std::string path, Model* model) {
	Assimp::Importer import_;
	std::string filename = "model/" + path;
	const aiScene* scene = import_.ReadFile(filename, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);	//check the scene variable

	auto animation = scene->mAnimations[0];
	this->m_duration = animation->mDuration;
	this->m_ticksPerSecond = animation->mTicksPerSecond;
	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();

	readDataStruct(m_rootNode, scene->mRootNode);
	readMissingBones(animation, *model);
}

//Find Bone
Bone* Animation::findBone(const char* name) {
	auto iter = std::find_if (m_bones.begin(), m_bones.end(), 
		[&](const Bone& bone) {
			return bone.getBoneName() == name;
		}
	);
	if (iter == m_bones.end()) return nullptr;
	else return &(*iter);
}

//reads the missing bones information and stores their information (private)
void Animation::readMissingBones(const aiAnimation* animation, Model& model) {
	int size = animation->mNumChannels;

	auto& boneInfoMap = model.getBoneInfoMap(); //getting m_BoneInfoMap from Model class
	int& boneCount = model.getBoneCounter(); //getting the m_BoneCounter from Model class

	//reading channels(bones engaged in an animation and their keyframes)
	for (int i = 0; i < size; i++) {
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		this->m_bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
	}

	this->m_boneInfoMap = boneInfoMap;
}

//replicates aiNode structure of Assimp and creates structure of AssimpNodeData (private)
void Animation::readDataStruct(AssimpNodeData& dest, const aiNode* src) {
	assert(src);	//check the src variable

	dest.name = src->mName.data;
	dest.transformation = assimpHelper::ConvertMatrix(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++) {
		AssimpNodeData newData;
		readDataStruct(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}
