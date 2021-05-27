#include "Bone.h"

//Reads from aiNodeAnim and stores all the datas (keys and its time stamp)
Bone::Bone(const char* name, int ID, const aiNodeAnim* channel) :
	m_name(name), m_ID(ID), m_localTransform(1.f) {

	//position
	m_numPositions = channel->mNumPositionKeys;
	for (int positionIndex = 0; positionIndex < m_numPositions; positionIndex++) {
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		float timeStamp = channel->mPositionKeys[positionIndex].mTime;
		KeyPosition data;
		data.position = assimpHelper::getGLMvec(aiPosition);
		data.timeStamp = timeStamp;
		m_Positions.push_back(data);
	}

	//rotation or orientation
	m_numRotations = channel->mNumRotationKeys;
	for (int rotationIndex = 0; rotationIndex < m_numRotations; rotationIndex++) {
		aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
		KeyRotation data;
		data.rotation = assimpHelper::getGLMquat(aiOrientation);
		data.timeStamp = timeStamp;
		m_Rotations.push_back(data);
	}

	//scale
	m_numScalings = channel->mNumScalingKeys;
	for (int scaleIndex = 0; scaleIndex < m_numScalings; scaleIndex++) {
		aiVector3D scale = channel->mScalingKeys[scaleIndex].mValue;
		float timeStamp = channel->mScalingKeys[scaleIndex].mTime;
		KeyScale data;
		data.scale = assimpHelper::getGLMvec(scale);
		data.timeStamp = timeStamp;
		m_Scales.push_back(data);
	}
}

//Update bone's data every frame
void Bone::Update(float animationTime) {
	glm::mat4 translation = InterpolatePosition(animationTime);
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 scale = InterpolateScaling(animationTime);
	m_localTransform = translation * rotation * scale;
}

//interpolate each key (private)
glm::mat4 Bone::InterpolatePosition(float animationTime) {
	if (1 == m_numPositions)
		return glm::translate(glm::mat4(1.0f), m_Positions[0].position);

	int lastTime = getPositionIndex(animationTime);
	int nextTime = lastTime + 1;
	float scaleFactor = getScaleFactor(m_Positions[lastTime].timeStamp, m_Positions[nextTime].timeStamp, animationTime);
	glm::vec3 finalPosition = glm::mix(m_Positions[lastTime].position, m_Positions[nextTime].position, scaleFactor);

	return glm::translate(glm::mat4(1.0f), finalPosition);
}
glm::mat4 Bone::InterpolateRotation(float animationTime) {
	if (1 == m_numRotations) {
		auto rotation = glm::normalize(m_Rotations[0].rotation);
		return glm::toMat4(rotation);
	}

	int lastTime = getRotationIndex(animationTime);
	int nextTime = lastTime + 1;
	float scaleFactor = getScaleFactor(m_Rotations[lastTime].timeStamp, m_Rotations[nextTime].timeStamp, animationTime);
	glm::quat finalRotation = glm::slerp(m_Rotations[lastTime].rotation, m_Rotations[nextTime].rotation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);

	return glm::toMat4(finalRotation);

}
glm::mat4 Bone::InterpolateScaling(float animationTime) {
	if (1 == m_numScalings)
		return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);

	int lastTime = getScaleIndex(animationTime);
	int nextTime = lastTime + 1;
	float scaleFactor = getScaleFactor(m_Scales[lastTime].timeStamp, m_Scales[nextTime].timeStamp, animationTime);
	glm::vec3 finalScale = glm::mix(m_Scales[lastTime].scale, m_Scales[nextTime].scale, scaleFactor);

	return glm::scale(glm::mat4(1.0f), finalScale);
}

//Get scale factor (private)
float Bone::getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) {
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

//Get every key's index (every key's last time stamp)
int Bone::getPositionIndex(float animationTime) {
	for (int index = 0; index < m_numPositions - 1; ++index) {
		if (animationTime < m_Positions[index + 1].timeStamp)
			return index;
	}
	assert(0);
}
int Bone::getRotationIndex(float animationTime) {
	for (int index = 0; index < m_numRotations - 1; ++index) {
		if (animationTime < m_Rotations[index + 1].timeStamp)
			return index;
	}
	assert(0);
}
int Bone::getScaleIndex(float animationTime) {
	for (int index = 0; index < m_numScalings - 1; ++index) {
		if (animationTime < m_Scales[index + 1].timeStamp)
			return index;
	}
	assert(0);
}