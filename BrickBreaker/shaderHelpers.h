#pragma once
/* DSA 1 - 
Shader Helpers.h */
#include <glew.h>
#include <glfw3.h>
#include <vector>
#include <glm/glm.hpp>


char* loadTextFile(const char* fileName);
GLuint loadShader(const char* fileName, GLenum shaderType);
GLuint loadShaderProgram(const char* vertexFile, const char* fragmentFile);
void setShaderVec2(GLuint index, const char * name, glm::vec2 vecToSend);
void setShaderColor(GLuint pIndex, const char * varName, GLfloat red, GLfloat blue, GLfloat green);
void setShaderValue(GLuint index, const char * name, int val);
void setShaderMatrix(GLuint index, const char * name, const glm::mat4 &matToSend);
void loadModel(const char* fileName, std::vector<glm::vec3> * vertices, std::vector<glm::vec2> * uvs, std::vector<glm::vec3> * normals, std::vector<unsigned int> * indices);