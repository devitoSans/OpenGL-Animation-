#include "Animator.h"

//reset m_currentTime and init m_finalMatrices
Animator::Animator(Animation* animation) {
	m_currentTime = 0.f;
	m_currentAnimation = animation;
	m_finalMatrices.reserve(100);
	for (int i = 0; i < 100; i++)
		m_finalMatrices.push_back(glm::mat4(1.f));
}

//Updating the animation
void Animator::updateAnimation(float time) {
	if (m_currentAnimation) {
		m_currentTime += m_currentAnimation->getTicksPerSecond() * time;
		m_currentTime = fmod(m_currentTime, m_currentAnimation->getDuration());
		calculateBoneTransform(&m_currentAnimation->getRootNode(), glm::mat4(1.0f));
	}
}

//Calculating all transformation and store it to m_finalMatrices
void Animator::calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform) {
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	Bone* Bone = m_currentAnimation->findBone(nodeName.c_str());

	if (Bone) {
		Bone->Update(m_currentTime);
		nodeTransform = Bone->getLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = m_currentAnimation->getBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		m_finalMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
		calculateBoneTransform(&node->children[i], globalTransformation);
}

//Get Last Frame
float Animator::getLastFrame() {
	while (true) {
		currentTime_ += m_currentAnimation->getTicksPerSecond() * deltaTime_temp;
		currentTime_ = fmod(currentTime_, m_currentAnimation->getDuration());

		float currentFrame_temp = glfwGetTime();
		deltaTime_temp = currentFrame_temp - lastFrame_temp;
		lastFrame_temp = currentFrame_temp;

		if (currentTime_ < lastSec)
			break;
		lastSec = currentTime_;
	}
	return lastSec - 0.01f;
}