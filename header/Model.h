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
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//SOIL Library
#include <SOIL2.h>

//Assimp Library
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

//Own Library
#include "Shader.h"
#include "Mesh.h"


/*  ===== Model's Class =====  */
struct BoneInfo {
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;
};
namespace assimpHelper {
    //convert assimp's matrix to GLM's matrix
    static inline glm::mat4 ConvertMatrix(const aiMatrix4x4& from) {
        glm::mat4 to;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;

        return to;
    };

    //convert assimp's vector to GLM's vector
    static inline glm::vec3 getGLMvec(aiVector3D& vector) {
        return glm::vec3(vector.x, vector.y, vector.z);
    }

    //convert assimp's quaternion to GLM's quaternion (usually for rotation)
    static inline glm::quat getGLMquat(aiQuaternion& orientation) {
        return glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);
    }
}

//Model
class Model {
    private:
        std::string directory;
        std::map<std::string, BoneInfo> m_boneInfoMap;
        int m_boneCounter = 0;

        //load model
        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string type_name);
        //load animated model
        void setVertexBoneDataDefault(Vertex& vertex);
        void setVertexBoneData(Vertex& vertex, int boneID, float weight);
        void extractBoneWeight(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

    public:
        std::vector<Mesh> meshes;
        void Draw(Shader& shader, int atArray);
        void Draw(Shader& shader);
        void Draw(Shader& shader, unsigned int primcount);
        void meshSetup_Instance(int size, glm::mat4* data);

        Model(std::string path) {
            loadModel(path);
        }
        auto& getBoneInfoMap() {
            return this->m_boneInfoMap;
        }
        int& getBoneCounter() {
            return this->m_boneCounter;
        }
};