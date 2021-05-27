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

//SOIL Library
#include <SOIL2.h>

//Assimp Library
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>


/*  ===== Class =====  */
//Shader
class Shader {
    private:
        //Shader
        std::string vertexSource, geometrySource, fragmentSource;
        //Read shader's file
        std::string readFile(char& type) {
            std::ifstream file;
            std::string src, srcTemp;

            if (type == 'v')    //-> vertex shader
                file.open(vertexSource);
            else if(type == 'g')    //-> geometry shader
                file.open(geometrySource); 
            else     //-> fragment shader
                file.open(fragmentSource);

            if (file.is_open())
                while (!file.eof()) {
                    std::getline(file, srcTemp);
                    src.append("\n" + srcTemp);
                }
            else std::cout << "ERROR::MAIN.CPP::CANNOT_OPEN_FILE_TYPE: " << type << std::endl;

            return src;
        }
        //compile shader
        unsigned int compileShader(char type) {
            std::string src;
            const char* srcChar;
            unsigned int shader;
            int success;
            char infoLog[512];

            src = readFile(type);
            srcChar = src.c_str();

            if (type == 'v')    //-> vertex shader
                shader = glCreateShader(GL_VERTEX_SHADER);
            else if (type == 'g')   //-> geometry shader
                shader = glCreateShader(GL_GEOMETRY_SHADER);
            else     //-> fragment shader
                shader = glCreateShader(GL_FRAGMENT_SHADER);

            glShaderSource(shader, 1, &srcChar, NULL);
            glCompileShader(shader);

            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                std::cout << infoLog << std::endl;
                std::cout << "ERROR::MAIN.CPP::FAILED_TO_COMPILE_SHADER_TYPE: " << type << std::endl;
            }

            return shader;
        }

    public:
        //Core Program
        unsigned int core_program;

        //Shader
        //without geometry shader ===
        Shader(std::string vertexSrc, std::string fragmentSrc) {
            vertexSource = vertexSrc;
            fragmentSource = fragmentSrc;

            unsigned int vertexShader = compileShader('v');
            unsigned int fragmentShader = compileShader('f');

            core_program = glCreateProgram();
            glAttachShader(core_program, vertexShader);
            glAttachShader(core_program, fragmentShader);

            glLinkProgram(core_program);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }
        //with geometry shader ===
        Shader(std::string vertexSrc, std::string fragmentSrc, std::string geometrySrc) {
            vertexSource = vertexSrc;
            geometrySource = geometrySrc;
            fragmentSource = fragmentSrc;

            unsigned int vertexShader = compileShader('v');
            unsigned int geometryShader = compileShader('g');
            unsigned int fragmentShader = compileShader('f');

            core_program = glCreateProgram();
            glAttachShader(core_program, vertexShader);
            glAttachShader(core_program, geometryShader);
            glAttachShader(core_program, fragmentShader);

            glLinkProgram(core_program);

            glDeleteShader(vertexShader);
            glDeleteShader(geometryShader);
            glDeleteShader(fragmentShader);
        }

        //Bind Uniform Buffer Object To a Point
        void bindUniform(const char* name, int number) {
            unsigned int uniform = glGetUniformBlockIndex(this->core_program, name);
            glUniformBlockBinding(this->core_program, uniform, number);
        }

        //set offset
        int getOffSet(const char* name) {
            unsigned int indices;
            int offsets;
            glGetUniformIndices(this->core_program, 1, &name, &indices);
            glGetActiveUniformsiv(this->core_program, 1, &indices, GL_UNIFORM_OFFSET, &offsets);

            return offsets;
        }

        //UBO
        unsigned int funcUBO(int size, int number) {
            unsigned int UBO;
            glGenBuffers(1, &UBO);

            //the last offset plus the size of last uniform
            
            glBindBuffer(GL_UNIFORM_BUFFER, UBO);	//-> data
            glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            glBindBufferRange(GL_UNIFORM_BUFFER, number, UBO, 0, size);

            return UBO;
        }

        //Use Program
        void useProgram() {
            glUseProgram(core_program);
        }
        // utility uniform functions 
         // ------------------------------------------------------------------------
        void setBool(const std::string& name, bool value) {
            glUniform1i(glGetUniformLocation(core_program, name.c_str()), (int)value);
        }
        // ------------------------------------------------------------------------
        void setInt(const std::string& name, int value) {
            glUniform1i(glGetUniformLocation(core_program, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setFloat(const std::string& name, float value) {
            glUniform1f(glGetUniformLocation(core_program, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setVec2(const std::string& name, const glm::vec2& value) {
            glUniform2fv(glGetUniformLocation(core_program, name.c_str()), 1, &value[0]);
        }
        void setVec2(const std::string& name, float x, float y) {
            glUniform2f(glGetUniformLocation(core_program, name.c_str()), x, y);
        }
        // ------------------------------------------------------------------------
        void setVec3(const std::string& name, const glm::vec3& value) {
            glUniform3fv(glGetUniformLocation(core_program, name.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string& name, float x, float y, float z) {
            glUniform3f(glGetUniformLocation(core_program, name.c_str()), x, y, z);
        }
        // ------------------------------------------------------------------------
        void setVec4(const std::string& name, const glm::vec4& value) {
            glUniform4fv(glGetUniformLocation(core_program, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string& name, float x, float y, float z, float w) {
            glUniform4f(glGetUniformLocation(core_program, name.c_str()), x, y, z, w);
        }
        // ------------------------------------------------------------------------
        void setMat2(const std::string& name, const glm::mat2& mat) {
            glUniformMatrix2fv(glGetUniformLocation(core_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(const std::string& name, const glm::mat3& mat) {
            glUniformMatrix3fv(glGetUniformLocation(core_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat4(const std::string& name, const glm::mat4& mat) {
            glUniformMatrix4fv(glGetUniformLocation(core_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
};