#pragma once

/*  ===== Library(Include) =====  */
//Standard Library
#include <iostream>
#include <string>
#include <fstream>
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
#include <gtx/norm.hpp>

//SOIL Library
#include <SOIL2.h>

//Assimp Library
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

//Own Library
#include "Game_Engine.h"


/*  ===== Function =====  */
//Create Window
GLFWwindow* window;
int window_w = 640, window_h = 480;
void createWindow(GLFWwindow** window);

//Frame Buffer
int framebuffer_w = window_w, framebuffer_h = window_h;
void frameBuffer(GLFWwindow* window, int w, int h);

//Camera call back(Looking Around)
void camera_callBack(GLFWwindow* window, double x_currMouse, double y_currMouse);

//Scroll
void scroll_callBack(GLFWwindow* window, double x_scroll, double y_scroll);

//Texture
unsigned int loadTexture(std::string filename);

//Input
void input(GameEngine* game);

//Cube Map
unsigned int cubeMap_setup();

//VAO, VBO, FBO
void funcVAO(unsigned int* VAO);
template<class T>
void funcVBO(unsigned int* VBO, int size, T* vert);
//with MSAA
void funcFBO_MSAA(unsigned int* FBO_MSAA, unsigned int* texture);
//without MSAA
void funcFBO(unsigned int* FBO, unsigned int* texture, bool wMSAA);


/*  ===== Vertex and Index =====  */
float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};


/*  ===== Variables =====  */
//Camera
glm::vec3 cameraPos(15.9f, 4.8f, 0.f);
glm::vec3 cameraFront(-0.7f, -0.6f, 0.f);
glm::vec3 cameraUp(0.f, 1.f, 0.f);

//Camera call back(Looking Around)
float x_lastMouse = 400.f, y_lastMouse = 300.f;
float pitch = 0.f, yaw = -90.f;
bool firstMouse = true;

//Scroll call back
int scroll = 4;
float fov = 45.f;

//Delta Time
float deltaTime = 0.f, lastFrame = 0.f;

//Mutant Movement
bool forwardTrue = 0;
bool backwardTrue = 0;
bool rightTrue = 0;
bool leftTrue = 0;

bool forwardLeftTrue = 0;
bool leftBackwardTrue = 0;
bool backwardRightTrue = 0;
bool rightForwardTrue = 0;

glm::vec3 mutantPos(11.f, 0.f, 0.f);
glm::vec3 mutantFront(0.f, 0.f, 1.f);

float mutantYaw;

//mutant jump
int jumpOne = 0;