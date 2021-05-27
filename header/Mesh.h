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


#define MAX_BONE_INFLUENCE 4
/*  ===== Mesh's Class =====  */
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    //bone indexes which will influence this vertex
    int m_boneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_weights[MAX_BONE_INFLUENCE];
};
struct Texture {
    unsigned int id = 0;
    std::string type;
    std::string path;
};

//Mesh
class Mesh {
    private:
        //Render Data
        unsigned int VAO, VBO, EBO;

        void mesh_setup();  //-> without instance
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indeces;
        std::vector<Texture> texture;

        Mesh(std::vector<Vertex> outVertices, std::vector<unsigned int> outIndeces, std::vector<Texture> outTexture);
        void Draw(Shader& shader);  //-> without instance
        void Draw(Shader& shader, int primcount);   //with instance
        void mesh_setupInstance(int size, glm::mat4* data);    //with instance
};