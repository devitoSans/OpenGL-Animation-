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
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//SOIL Library
#include <SOIL2.h>

//Own Library
#include "Shader.h"


/*  ===== GameEngine's Class =====  */
class GameEngine {
	private :
		int framebuffer_w = 0, framebuffer_h = 0;
		GLFWwindow* window;

		std::vector<unsigned int> UBO;
		std::vector<int> offsets;

	public :
		GameEngine(GLFWwindow* window) {
			this->window = window;
		}

		//set offset and generate UBO
		void setUBO(Shader* shader) {
			//set offset
			const char* uniforms[9] = {
				"cameraPos",
				"light.position", "light.color",
				"light.ambient", "light.diffuse", "light.specular",
				"light.constant", "light.linear", "light.quadratic"
			};
			int uniformsSize = sizeof(uniforms) / sizeof(const char*);
			for (int i = 0; i < uniformsSize; i++)
				this->offsets.push_back(shader->getOffSet(uniforms[i]));

			//UBO
			//change this to the size of last uniforms' component.
			int UBOsize = shader->getOffSet(uniforms[uniformsSize - 1]) + sizeof(float);
			this->UBO.push_back(shader->funcUBO(UBOsize, 0));	//matrices
			this->UBO.push_back(shader->funcUBO(UBOsize, 1));	//data
		}

		//one buttuon
		int oneInput = 0;
		int oneButton(int input, int* switch_flashLight, int* loop_input);

		//calculate
		void calculateFPS();

		//gravity implement
		int jump_one = 0;	//must use on space button
		void gravityImp(float basePosition, float gravityPower, float* velocity, float* positionY, float time);

		//perspective setup
		float perspectiveSetup();
		
		//setup draw
		//Camera
		glm::vec3 cameraPos = glm::vec3(NULL);
		glm::vec3 cameraFront = glm::vec3(NULL);
		glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);
		//Lighting
		glm::vec3 attenuation = glm::vec3(1.0, 0.35, 0.44);
		glm::vec3 lightPos = cameraPos;
		glm::vec3 lightColor = glm::vec3(2.46, 1.54, 0.84);
		float shininess = 16.f;
		glm::vec3 ambientLight = glm::vec3(0.1f);
		glm::vec3 diffuseLight = glm::vec3(0.5f);
		glm::vec3 specularLight = glm::vec3(0.5f);
		//Projection, Camera, Model
		glm::mat4 op_projection = glm::mat4(1.f);
		glm::mat4 op_model = glm::mat4(1.f);
		glm::mat4 op_camera = glm::mat4(1.f);

		void drawSetup(Shader* shader);
};