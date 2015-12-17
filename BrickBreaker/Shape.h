#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glew.h>
#include <glfw3.h>
#include <vector>

class Shape
{
protected:
	std::vector<glm::vec3> _vertices;

	GLuint vboArray;
	GLuint vaoArray;
	GLsizei numVert;
	GLuint programIndex;
	GLint offsetLoc;
	GLint scaleLoc;
	GLenum mode;

	bool _flat;
public:
	Shape(const char * name,  GLuint pProgram, GLenum pMode, bool flat = false);
	void Draw(const glm::mat4 &world);
	std::vector<glm::vec3> * vertices();
	~Shape();
};

