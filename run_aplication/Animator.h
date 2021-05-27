#pragma once

/*  ===== Library(Include) =====  */
//Standard Library
#include <iostream>
#include <string>
#include <vector>
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
#include "Bone.h"
#include "Animation.h"


/*  ===== Animator's Class =====  */
class Animator {
	private :
		std::vector<glm::mat4> m_finalMatrices;
		Animation* m_currentAnimation;
		float m_currentTime;

		//Get last frame function's properties
		float lastFrame_temp = 0.f;
		float deltaTime_temp = 0.f;

		float lastSec = 0.f;
		float currentTime_ = 0.f;

	public :
		Animator(Animation* animation);

		//Get Final Matrices
		std::vector<glm::mat4> getFinalMatrices() { return m_finalMatrices; }

		//Get Duration
		float getDuration() { return m_currentTime; }

		//Updating the animation
		void updateAnimation(float time);

		//Calculating all transformation and store it to m_finalMatrices
		void calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

		//Play the Animation
		float getLastFrame();
};