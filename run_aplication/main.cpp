#include "libs.h"
#include "Shader.h"
#include "Model.h"
#include "Animator.h"
#include "Game_Engine.h"
#include "DrawMutant.h"

int main() {
	//Create Window    =====
	createWindow(&window);

	//Shader    =====
	Shader defaultShader("df_vertex.glsl", "df_frag.glsl");
	Shader lightShader("lg_vertex.glsl", "lg_frag.glsl");
	Shader animateShader("an_vertex.glsl", "an_frag.glsl");
	Shader cubemapShader("cm_vertex.glsl", "cm_frag.glsl");

	//Model    =====
	//Arena
	Model arenaModel("gladiatorArena/gladiatorArena.obj");

	//Fireball and its handle
	Model fireballModel("MonkeSun/MonkeSun.obj");
	Model handleModel("fireballHandle/fireballHandle.obj");

	//Running Mutant
	const char* filename = "mutantRunning/mutantRunning.dae";
	Model runningModel(filename);
	Animation runningAnimation(filename, &runningModel);
	Animator runningAnimator(&runningAnimation);

	//Standing Mutant
	filename = "mutantStanding/mutantStanding.dae";
	Model standingModel(filename);
	Animation standingAnimation(filename, &standingModel);
	Animator standingAnimator(&standingAnimation);

	//Jumping Mutant
	filename = "mutantJumping/mutantJumping.dae";
	Model jumpingModel(filename);
	Animation jumpingAnimation(filename, &jumpingModel);
	Animator jumpingAnimator(&jumpingAnimation);
	float jumpingLastFrame = jumpingAnimator.getLastFrame();

	//Game Engine    =====
	GameEngine mainGame(window);

	//VAO VBO
	unsigned int cmVAO, cmVBO;
	funcVBO(&cmVBO, sizeof(skyboxVertices), skyboxVertices);
	funcVAO(&cmVAO);

	//Cube map
	unsigned int cmTexture;
	cmTexture = cubeMap_setup();

	//Uniform Buffer Object    =====
	//binding point 0
	defaultShader.bindUniform("Matrices", 0);

	//binding point 1
	defaultShader.bindUniform("Data", 1);
	animateShader.bindUniform("Data", 1);

	//UBO setup    =====
	mainGame.setUBO(&defaultShader);

	//DEFINE RENDER    =====
	#define ___clearColor glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	//Main Loop    =====
	while (!glfwWindowShouldClose(window)) {
		//FPS
		mainGame.calculateFPS();

		//Draw Setup
		//Properties
		glm::vec3 mutantPosition = glm::vec3(mutantPos.x, 0.f, mutantPos.z);
		mainGame.cameraPos = cameraPos;
		mainGame.cameraFront = glm::vec3(mutantPos.x, mutantPos.y + 1.5f, mutantPos.z);

		//RENDER ========================================================================== RENDER
		___clearColor;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//DRAW ==================================
		int forward = glfwGetKey(window, GLFW_KEY_W);
		int left = glfwGetKey(window, GLFW_KEY_A);
		int backward = glfwGetKey(window, GLFW_KEY_S);
		int right = glfwGetKey(window, GLFW_KEY_D);
		int jump = glfwGetKey(window, GLFW_KEY_SPACE);
		glm::mat4 rotate(1.f);
									//DRAW STANDING MUTANT
		if (!forward && !left && !backward && !right && !jumpOne) {
			if(forwardTrue)	//-> pressing W
				rotate = glm::rotate(mainGame.op_model,
					glm::radians(-mutantYaw) + glm::radians(-120.f), glm::vec3(0.f, 1.f, 0.f));
			else if (backwardTrue)//-> pressing S
				rotate = glm::rotate(mainGame.op_model,
					glm::radians(-mutantYaw) + glm::radians(70.f), glm::vec3(0.f, 1.f, 0.f));
			else if (rightTrue) //-> pressing D
				rotate = glm::rotate(mainGame.op_model,
					glm::radians(-mutantYaw) + glm::radians(150.f), glm::vec3(0.f, 1.f, 0.f));
			else //-> pressing A
				rotate = glm::rotate(mainGame.op_model,
					glm::radians(-mutantYaw) + glm::radians(-30.f), glm::vec3(0.f, 1.f, 0.f));

			drawMutant(animateShader,
				standingAnimator,
				&mainGame,
				standingModel,
				mutantPosition, rotate);
		}

									//DRAW JUMPING MUTANT

		//JUMP
		if (!forward && !left && !backward && !right && jump || 
			!forward && !left && !backward && !right && jumpOne) {
			if (forwardTrue)	//-> pressing W
				rotate = glm::rotate(mainGame.op_model, 
					glm::radians(-mutantYaw) + glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
			else if (backwardTrue)//-> pressing S
				rotate = glm::rotate(mainGame.op_model, 
					glm::radians(-mutantYaw) + glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
			else if (rightTrue) //-> pressing D
				rotate = glm::rotate(mainGame.op_model, 
					glm::radians(-mutantYaw) + glm::radians(189.f), glm::vec3(0.f, 1.f, 0.f));
			else //-> pressing A
				rotate = glm::rotate(mainGame.op_model, 
					glm::radians(-mutantYaw), glm::vec3(0.f, 1.f, 0.f));

			jumpOne = 1;
			drawMutant(animateShader,
				jumpingAnimator,
				&mainGame,
				jumpingModel,
				mutantPosition, rotate);
		}

		//W - A JUMP
		else if (forward && left && jump || jumpOne && forwardTrue && leftTrue) {
			rotate = glm::rotate(mainGame.op_model, 
				glm::radians(-yaw) + glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;
			jumpOne = 1;

			forwardTrue = true;
			leftTrue = true;
			rightTrue = false;
			backwardTrue = false;

			drawMutant(animateShader,
				jumpingAnimator,
				&mainGame,
				jumpingModel,
				mutantPos, rotate);
		}
		//A - S JUMP
		else if (left && backward && jump || jumpOne && leftTrue && backwardTrue) {
			rotate = glm::rotate(mainGame.op_model, 
				glm::radians(-yaw) + glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;
			jumpOne = 1;

			leftTrue = true;
			backwardTrue = true;
			rightTrue = false;
			forwardTrue = false;

			drawMutant(animateShader,
				jumpingAnimator,
				&mainGame,
				jumpingModel,
				mutantPos, rotate);
		}
		//S - D JUMP
		else if (backward && right && jump || jumpOne && backwardTrue && rightTrue) {
			rotate = glm::rotate(mainGame.op_model,
				glm::radians(-yaw) + glm::radians(145.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;
			jumpOne = 1;

			backwardTrue = true;
			rightTrue = true;
			forwardTrue = false;
			leftTrue = false;

			drawMutant(animateShader,
				jumpingAnimator,
				&mainGame,
				jumpingModel,
				mutantPos, rotate);
		}
		//D - W JUMP
		else if (right && forward && jump || jumpOne && rightTrue && forwardTrue) {
			rotate = glm::rotate(mainGame.op_model,
				glm::radians(-yaw) + glm::radians(-145.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;
			jumpOne = 1;

			rightTrue = true;
			forwardTrue = true;
			backwardTrue = false;
			leftTrue = false;

			drawMutant(animateShader,
				jumpingAnimator,
				&mainGame,
				jumpingModel,
				mutantPos, rotate);
		}

		//W JUMP
		else if (forward && jump || jumpOne && forwardTrue) {
			rotate = glm::rotate(mainGame.op_model, 
				glm::radians(-yaw) + glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;
			jumpOne = 1;

			forwardTrue = true;
			backwardTrue = false;
			rightTrue = false;
			leftTrue = false;

			drawMutant(animateShader,
				jumpingAnimator,
				&mainGame,
				jumpingModel,
				mutantPos, rotate);
		}
		//A JUMP
		else if (left && jump || jumpOne && leftTrue) {
			rotate = glm::rotate(mainGame.op_model, glm::radians(-yaw), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;
			jumpOne = 1;

			forwardTrue = false;
			backwardTrue = false;
			rightTrue = false;
			leftTrue = true;

			drawMutant(animateShader,
				jumpingAnimator,
				&mainGame,
				jumpingModel,
				mutantPos, rotate);
		}
		//S JUMP
		else if (backward && jump || jumpOne && backwardTrue) {
			rotate = glm::rotate(mainGame.op_model,
				glm::radians(-yaw) + glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;
			jumpOne = 1;

			forwardTrue = false;
			backwardTrue = true;
			rightTrue = false;
			leftTrue = false;

			drawMutant(animateShader,
				jumpingAnimator,
				&mainGame,
				jumpingModel,
				mutantPos, rotate);
		}
		//D JUMP
		else if (right && jump || jumpOne && rightTrue) {
			rotate = glm::rotate(mainGame.op_model,
				glm::radians(-yaw) + glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;
			jumpOne = 1;

			forwardTrue = false;
			backwardTrue = false;
			rightTrue = true;
			leftTrue = false;

			drawMutant(animateShader,
				jumpingAnimator,
				&mainGame,
				jumpingModel,
				mutantPos, rotate);
		}

									//DRAW RUNNING MUTANT

		//W - A
		else if (forward && left) {
			rotate = glm::rotate(mainGame.op_model,
				glm::radians(-yaw) + glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;

			forwardTrue = true;
			leftTrue = true;
			rightTrue = false;
			backwardTrue = false;

			drawMutant(animateShader,
				runningAnimator,
				&mainGame,
				runningModel,
				mutantPos, rotate);
		}
		//A - S
		else if (left && backward) {
			rotate = glm::rotate(mainGame.op_model,
				glm::radians(-yaw) + glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;

			leftTrue = true;
			backwardTrue = true;
			rightTrue = false;
			forwardTrue = false;

			drawMutant(animateShader,
				runningAnimator,
				&mainGame,
				runningModel,
				mutantPos, rotate);
		}
		//S - D
		else if (backward && right) {
			rotate = glm::rotate(mainGame.op_model,
				glm::radians(-yaw) + glm::radians(145.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;

			backwardTrue = true;
			rightTrue = true;
			forwardTrue = false;
			leftTrue = false;

			drawMutant(animateShader,
				runningAnimator,
				&mainGame,
				runningModel,
				mutantPos, rotate);
		}
		//D - W
		else if (right && forward) {
			rotate = glm::rotate(mainGame.op_model,
				glm::radians(-yaw) + glm::radians(-145.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;

			rightTrue = true;
			forwardTrue = true;
			backwardTrue = false;
			leftTrue = false;

			drawMutant(animateShader,
				runningAnimator,
				&mainGame,
				runningModel,
				mutantPos, rotate);
		}

		//W
		else if (forward) {
			rotate = glm::rotate(mainGame.op_model, 
				glm::radians(-yaw) + glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;

			forwardTrue = true;
			backwardTrue = false;
			rightTrue = false;
			leftTrue = false;

			drawMutant(animateShader,
				runningAnimator,
				&mainGame,
				runningModel,
				mutantPos, rotate);
		}
		//A
		else if (left) {
			rotate = glm::rotate(mainGame.op_model, glm::radians(-yaw), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;

			leftTrue = true;
			forwardTrue = false;
			backwardTrue = false;
			rightTrue = false;

			drawMutant(animateShader, 
				runningAnimator, 
				&mainGame, 
				runningModel, 
				mutantPos, rotate);
		}
		//S
		else if (backward) {
			rotate = glm::rotate(mainGame.op_model, 
				glm::radians(-yaw) + glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;

			backwardTrue = true;
			rightTrue = false;
			forwardTrue = false;
			leftTrue = false;

			drawMutant(animateShader,
				runningAnimator,
				&mainGame,
				runningModel,
				mutantPos, rotate);
		}
		//D
		else if (right) {
			rotate = glm::rotate(mainGame.op_model, 
				glm::radians(-yaw) + glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
			mutantYaw = yaw;

			rightTrue = true;
			backwardTrue = false;
			forwardTrue = false;
			leftTrue = false;

			drawMutant(animateShader,
				runningAnimator,
				&mainGame,
				runningModel,
				mutantPos, rotate);
		}

		//DRAW ARENA
		defaultShader.useProgram();
		mainGame.shininess = 16.f;
		mainGame.attenuation = glm::vec3(1.0, 0.14, 0.07);
		mainGame.lightPos = glm::vec3(2.f, 5.f, 2.f);
		mainGame.op_model = glm::scale(mainGame.op_model, glm::vec3(2.f));
		mainGame.drawSetup(&defaultShader);
		arenaModel.Draw(defaultShader);

		//DRAW FIREBALL'S HANDLE
		defaultShader.useProgram();
		mainGame.shininess = 16.f;
		mainGame.attenuation = glm::vec3(1.0, 0.14, 0.07);
		mainGame.lightPos = glm::vec3(2.f, 5.f, 2.f);
		mainGame.op_model = glm::translate(mainGame.op_model, glm::vec3(2.f, 0.f, 2.f));
		mainGame.drawSetup(&defaultShader);
		handleModel.Draw(defaultShader);

		//DRAW FIREBALL
		lightShader.useProgram();
		mainGame.op_model = glm::translate(mainGame.op_model, glm::vec3(2.f, 0.8f, 2.f));
		mainGame.op_model = glm::scale(mainGame.op_model, glm::vec3(0.9f));
		mainGame.drawSetup(&lightShader);
		fireballModel.Draw(lightShader);

		//CUBE MAP
		cubemapShader.useProgram();
		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(cmVAO);
		mainGame.op_model = glm::translate(mainGame.op_model, cameraPos);
		mainGame.op_model = glm::scale(mainGame.op_model, glm::vec3(500.f));
		mainGame.drawSetup(&cubemapShader);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cmTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		//INPUT
		input(&mainGame);

		//Animation Update
		if (!forward && !left && !backward && !right || 
			forward && left && backward && right)
			standingAnimator.updateAnimation(deltaTime);	//update standing animation
		else if (forward || left || backward || right)
			runningAnimator.updateAnimation(deltaTime);	//update running animation
		if (jumpOne == 1)
			jumpingAnimator.updateAnimation(deltaTime * 1.5f); //update jumping while running animation
		if (jumpingAnimator.getDuration() >= jumpingLastFrame)
			jumpOne = 0;

		//DELTA TIME
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//SWAP BUFFERS
		glfwSwapBuffers(window);
		glFlush();
		glfwSwapInterval(0);
	}

	//Delete
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


/*  ===== Function =====  */
//Create Window
void createWindow(GLFWwindow** window) {
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	*window = glfwCreateWindow(window_w, window_h, "test", NULL, NULL);

	//Frame Buffer
	glfwSetFramebufferSizeCallback(*window, frameBuffer);
	//Mouse
	glfwSetCursorPosCallback(*window, camera_callBack);
	//Scroll
	glfwSetScrollCallback(*window, scroll_callBack);

	glfwMakeContextCurrent(*window);

	glewExperimental = GL_TRUE;
	glewInit();

	//OpenGL Setting
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Depth
	glEnable(GL_DEPTH_TEST);

	//Blend
	glEnable(GL_BLEND);

	//Cull face
	glEnable(GL_CULL_FACE);
}

//Frame Buffer
void frameBuffer(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

//VAO, VBO, FBO - RBO
template<class T>
void funcVBO(unsigned int* VBO, int size, T* vert) {
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vert, GL_STATIC_DRAW);
}
void funcVAO(unsigned int* VAO) {
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
}
//with MSAA or Anti Aliasing	//-> must use window (width and height)
void funcFBO_MSAA(unsigned int* FBO_MSAA, unsigned int* texture) {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	//FBO -> Framebuffer Object
	glGenFramebuffers(1, FBO_MSAA);
	glBindFramebuffer(GL_FRAMEBUFFER, *FBO_MSAA);

	unsigned int textureFBO_MSAA;
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, *texture);

	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, mode->width, mode->height, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, *texture, 0);

	//RBO -> render buffer object
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, mode->width, mode->height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	//Check framebuffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::MAIN.CPP::FRAMEBUFFER_IS_NOT_COMPLETE\n";

	//unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
//without MSAA or Anti Aliasing	-> must use framebuffer
void funcFBO(unsigned int* FBO, unsigned int* texture, bool wMSAA) {
	//Generate FBO
	glGenFramebuffers(1, FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);

	// create a color attachment texture
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebuffer_w, framebuffer_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture, 0);

	//check if normal FBO generated with MSAA FBO
	if (!wMSAA) {
		//RBO -> render buffer object
		unsigned int RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer_w, framebuffer_h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		//unbind RBO
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	//Check framebuffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::MAIN.CPP::FRAMEBUFFER_IS_NOT_COMPLETE\n";

	//unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//Camera call back(Looking Around) -> input for mouse (sort of)
void camera_callBack(GLFWwindow* window, double x_currMouse, double y_currMouse) {
	if (firstMouse) {
		x_lastMouse = x_currMouse;
		y_lastMouse = y_currMouse;
		firstMouse = false;
	}

	float x_mouse = x_currMouse - x_lastMouse;
	float y_mouse = y_lastMouse - y_currMouse;
	x_lastMouse = x_currMouse;
	y_lastMouse = y_currMouse;

	float sensivity = 0.05f;
	x_mouse *= sensivity;
	y_mouse *= sensivity;

	yaw = glm::mod(yaw + x_mouse, 360.f);;
	pitch += y_mouse;

	if (pitch > 89.f) pitch = 89.f;
	if (pitch < -89.f) pitch = -89.f;
}

void scroll_callBack(GLFWwindow* window, double x_scroll, double y_scroll) {
	scroll -= y_scroll;

	if (scroll < 1)
		scroll = 1;
	else if (scroll > 50)
		scroll = 50;
}

//Texture
unsigned int loadTexture(std::string filename) {
	std::string file = "image/" + filename;

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int image_w, image_h;
	unsigned char* image = SOIL_load_image(file.c_str(), &image_w, &image_h, NULL, SOIL_LOAD_RGBA);

	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::MAIN.CPP::FAILED_TO_LOAD_IMAGE\n";
	}

	return texture;
}

//Input
void input(GameEngine* mainGame) {
	//General Input
	glfwPollEvents();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	//Input From Keyboard
	float speed = 5.f;	//speed for flying (it can be also for gravitation stuff)
	float movementSpeed = speed * deltaTime;	//speed for walk
	glm::vec3 cameraRight = glm::cross(-cameraPos, cameraUp);

	static float velocity = 0.f;	//(20 - 88) animation
	if (glfwGetKey(window, GLFW_KEY_SPACE) && mainGame->jump_one == 0)
		velocity = 5.f;
	mainGame->gravityImp(0.f, 2.f, &velocity, &mutantPos.y, deltaTime);	//-> gravity implementation

	//WASD
	if (glfwGetKey(window, GLFW_KEY_W))
		mutantPos += movementSpeed * glm::normalize(glm::cross(cameraUp, cameraRight));
	if (glfwGetKey(window, GLFW_KEY_A))
		mutantPos -= movementSpeed * glm::normalize(cameraRight);
	if (glfwGetKey(window, GLFW_KEY_S))
		mutantPos -= movementSpeed * glm::normalize(glm::cross(cameraUp, cameraRight));
	if (glfwGetKey(window, GLFW_KEY_D))
		mutantPos += movementSpeed * glm::normalize(cameraRight);
	
	//Input for Camera
	glm::vec3 direction;
	float radius = scroll;
	direction.x = (cos(glm::radians(yaw)) * cos(glm::radians(pitch))) * radius;
	direction.y = -sin(glm::radians(pitch)) * radius;
	direction.z = (sin(glm::radians(yaw)) * cos(glm::radians(pitch))) * radius;
	if (direction.y < -1.5f) direction.y = -1.5f;
	cameraPos = direction;
}

//Cube Map
unsigned int cubeMap_setup() {
	//faces
	std::vector<std::string> faces = {
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

	//generate texture
	unsigned int texture_cubeMap;
	glGenTextures(1, &texture_cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubeMap);

	//texture's option
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//attach texture
	int image_w, image_h;
	for (int i = 0; i < faces.size(); i++) {
		std::string filename = "image/stars_milky_way2.jpg";
		unsigned char* image = SOIL_load_image(filename.c_str(), &image_w, &image_h, NULL, SOIL_LOAD_RGBA);

		if (image) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		}
		else std::cout << "ERROR::MAIN.CPP::Cubemap tex failed to load at path:" << faces[i] << std::endl;
		SOIL_free_image_data(image);
	}

	return texture_cubeMap;
}