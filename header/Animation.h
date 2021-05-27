#pragma once

/*  ===== Library(Include) =====  */
//Standard Library
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <map>

//GLFW and GLEW Library
#include <glew.h>
#include <glfw3.h>

//GLM Library
#include <glm.hpp>

//Assimp Library
#include <Importer.hpp>
#include <scene.h>

//Own Library
#include "Model.h"
#include "Bone.h"


/*  ===== Animation's Class =====  */
struct AssimpNodeData {
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation {
	private :
		//properties
		float m_duration;
		int m_ticksPerSecond;
		std::vector<Bone> m_bones;
		AssimpNodeData m_rootNode;
		std::map<std::string, BoneInfo> m_boneInfoMap;


		void readMissingBones(const aiAnimation* animation, Model& model);
		//replicates aiNode structure of Assimp and creates structure of AssimpNodeData
		void readDataStruct(AssimpNodeData& dest, const aiNode* src);

	public :
		Animation(std::string path, Model* model);
		~Animation() {}

		inline float getTicksPerSecond() { return this->m_ticksPerSecond; }
		inline float getDuration() { return this->m_duration; }
		inline const AssimpNodeData& getRootNode() { return this->m_rootNode; }
		inline const std::map<std::string, BoneInfo>& getBoneIDMap() {
			return this->m_boneInfoMap;
		}

		//Find Bone
		Bone* findBone(const char* name);
};