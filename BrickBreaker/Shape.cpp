#include "Shape.h"
#include "shaderHelpers.h"
#include <iostream>
#include <vector>
#include "GameData.h"
#include <glm\glm.hpp>
#include "Console.h"

/*
Constructor. Loads an obj TODO texture combo
*/
Shape::Shape(const char * name,  GLuint pProgram, GLenum pMode, bool flat)
{
	_flat = flat;
	mode = pMode;
	programIndex = pProgram;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	loadModel(name, &vertices, &uvs, &normals, &indices); //TODO normals and uvs

	//for draw call
	numVert = indices.size();

	_vertices = vertices;
	
	GLuint indexBuffer;
	//set up arrays
	GLuint uvIndex;
	GLuint normalIndex;	
	


	glGenVertexArrays(1, &vaoArray);
	glBindVertexArray(vaoArray);
	glGenBuffers(1, &vboArray);
	glBindBuffer(GL_ARRAY_BUFFER, vboArray);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &uvIndex);
	glBindBuffer(GL_ARRAY_BUFFER, uvIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &normalIndex);
	glBindBuffer(GL_ARRAY_BUFFER, normalIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

/*
Vertex getter for collider gen.
*/
std::vector<glm::vec3> * Shape::vertices()
{
	return &_vertices;
}

/*
Draws the shape
*/
void Shape::Draw(const glm::mat4 &world)
{
	if (_flat) {
		setShaderValue(programIndex, "bright", 1);
	}
	else {
		setShaderValue(programIndex, "bright", 0);
	}
	setShaderMatrix(programIndex, "worldMatrix", world);
	
	glBindVertexArray(vaoArray);
	glDrawElements(mode, numVert, GL_UNSIGNED_INT, 0);
}


/*
Deletes the gl buffers created in the constructor
*/
Shape::~Shape()
{
	glDeleteVertexArrays(1, &vaoArray);
	glDeleteBuffers(1, &vboArray);
}
