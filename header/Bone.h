#pragma once

/*  ===== Library(Include) =====  */
//Standard Library
#include <iostream>
#include <string>
#include <vector>

//GLFW and GLEW Library
#include <glew.h>
#include <glfw3.h>

//GLM Library
#include <glm.hpp>
#include <gtx/quaternion.hpp>

//Assimp Library
#include <scene.h>
#include <matrix4x4.h>

//Own Library
#include "Model.h"


/*  ===== Bone's Class =====  */
struct KeyPosition {
	glm::vec3 position;
	float timeStamp = NULL;
};
struct KeyRotation {
	glm::quat rotation;
	float timeStamp = NULL;
};
struct KeyScale {
	glm::vec3 scale;
	float timeStamp = NULL;
};

//Bone
class Bone {
	private:
		//properties
		std::vector<KeyPosition> m_Positions;
		std::vector<KeyRotation> m_Rotations;
		std::vector<KeyScale> m_Scales;
		int m_numPositions = NULL;
		int m_numRotations = NULL;
		int m_numScalings = NULL;

		glm::mat4 m_localTransform;
		std::string m_name;
		int m_ID;

		//Interpolation each key
		glm::mat4 InterpolatePosition(float animationTime);
		glm::mat4 InterpolateRotation(float animationTime);
		glm::mat4 InterpolateScaling(float animationTime);
		//Get scale factor
		float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

	public :
		Bone(const char* name, int ID, const aiNodeAnim* channel);

		std::string getBoneName()const { return this->m_name; }
		glm::mat4 getLocalTransform() { return this->m_localTransform; }
		int getBoneID() { return this->m_ID; }

		//update bone's data
		void Update(float animationTime);
		//Get each key's index
		int getPositionIndex(float animationTime);
		int getRotationIndex(float animationTime);
		int getScaleIndex(float animationTime);
};