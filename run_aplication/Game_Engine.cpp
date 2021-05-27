#include "Game_Engine.h"

//press one button -> first press = action 1, sec press = action 2 and it keep looping
int GameEngine::oneButton(int input, int* switch_flashLight, int*loop_input) {
	if (input || *loop_input) {
		*loop_input = 1;
		if (!input && !*switch_flashLight) {	//-> reversed, so it can change
			*loop_input = 0;
			*switch_flashLight = 1;
			return 1;
		}
		else if (!input && *switch_flashLight) {	//-> reversed, so it can change
			*loop_input = 0;
			*switch_flashLight = 0;
			return 0;
		}
	}

	if (*switch_flashLight) return 1;
	return 0;
}

//FPS
void GameEngine::calculateFPS() {
	static int lastSecond = 0, frames = 0;
	int currSecond = 0;
	currSecond = glfwGetTime();

	if (currSecond > lastSecond) {
		lastSecond = currSecond;
		int fps = frames;
		std::cout << fps << std::endl;
		frames = 0;
	}

	frames = frames + 1;
}

//Gravity Implementation
void GameEngine::gravityImp(float basePosition, float gravityPower, float* velocity, float* positionY, float time) {
	//note : (1)the ideal gravity is speed = 10.f and,
	//gravity power = 3.5f
	//(2)speed = 5.f, gravity power = 2.f
	float gravity = -9.8f;
	if (gravityPower < 0.f)
		gravityPower = 1.f / -gravityPower;

	//calculate gravity
	*positionY += *velocity * time;
	*velocity += gravity * time * gravityPower;
	if (*positionY < basePosition && gravityPower != 0.f) {
		*positionY = basePosition;
		if (*velocity < -1.f)
			*velocity = -1.f;
		this->jump_one = 0;
	}
	else if (gravityPower != 0.f) this->jump_one = 1;
	else if (gravityPower == 0.f) *velocity = 0.f;
}

//Draw Setup	(Default)
void GameEngine::drawSetup(Shader* shader) {
	//setup check
	if (cameraPos != glm::vec3(NULL) && cameraFront != glm::vec3(NULL)) {
		//set material
		shader->setFloat("material.shininess", this->shininess);
		//Data
		glBindBuffer(GL_UNIFORM_BUFFER, UBO[1]);

		glBufferSubData(GL_UNIFORM_BUFFER, offsets[0], sizeof(glm::vec3), glm::value_ptr(cameraPos));

		glBufferSubData(GL_UNIFORM_BUFFER, offsets[1], sizeof(glm::vec3), glm::value_ptr(lightPos));
		glBufferSubData(GL_UNIFORM_BUFFER, offsets[2], sizeof(glm::vec3), glm::value_ptr(lightColor));

		glBufferSubData(GL_UNIFORM_BUFFER, offsets[3], sizeof(glm::vec3), glm::value_ptr(ambientLight));
		glBufferSubData(GL_UNIFORM_BUFFER, offsets[4], sizeof(glm::vec3), glm::value_ptr(diffuseLight));
		glBufferSubData(GL_UNIFORM_BUFFER, offsets[5], sizeof(glm::vec3), glm::value_ptr(specularLight));

		glBufferSubData(GL_UNIFORM_BUFFER, offsets[6], sizeof(float), &attenuation.x);
		glBufferSubData(GL_UNIFORM_BUFFER, offsets[7], sizeof(float), &attenuation.y);
		glBufferSubData(GL_UNIFORM_BUFFER, offsets[8], sizeof(float), &attenuation.z);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//model, camera, projection
		glm::mat4 projection, camera, model;
		projection = glm::mat4(1.f);
		camera = glm::mat4(1.f);
		model = glm::mat4(1.f);

		projection = this->op_projection;
		camera = this->op_camera;
		model = this->op_model;

		glm::mat4 PCM = projection * camera * model;
		glm::mat4 PC = projection * camera;
		shader->setMat4("model", model);
		shader->setMat4("camera", camera);
		shader->setMat4("projection", projection);
		shader->setMat4("PC", PC);
		shader->setMat4("PCM", PCM);
		//Matrices
		glBindBuffer(GL_UNIFORM_BUFFER, UBO[0]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(PC));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//reset transformation
		this->op_projection = glm::perspective(glm::radians(45.f),
								perspectiveSetup(),
								0.1f, 1000.f);
		this->op_camera = glm::lookAt(this->cameraPos + this->cameraFront, 
									this->cameraFront, 
									this->cameraUp);	
		this->op_model = glm::mat4(1.f);

		//reset light position
		this->lightPos = this->cameraPos;
	}
	else {
		std::cout << "WARNING::GAME_ENGINE.CPP::PLEASE GIVE ";
		std::cout << "CAMERA_POS OR_CAMERA_FRONT VALUES\n";
	}
}

//perspective setup
float GameEngine::perspectiveSetup() {
	glfwGetFramebufferSize(this->window, &this->framebuffer_w, &this->framebuffer_h);
	if (this->framebuffer_w <= 0.f && this->framebuffer_h <= 0.f) {
		this->framebuffer_w = 640;
		this->framebuffer_h = 480;
	}
	float aspect = float(this->framebuffer_w) / this->framebuffer_h;

	return aspect;
}