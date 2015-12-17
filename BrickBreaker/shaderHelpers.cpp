
#include "shaderHelpers.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

using namespace std;


//for map
struct KeyFuncs
{
	size_t operator()(const glm::vec3& k)const
	{
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
	}

	bool operator()(const glm::vec3& a, const glm::vec3& b)const
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};


typedef unordered_map<glm::vec3, int, KeyFuncs, KeyFuncs> VecMap;

/* Open the specified file in binary format, 
 load its text into a null-terminated array of characters
 and return that array (reference week 10's File IO work). 
 If there is a problem opening the file, return 0.
*/
char* loadTextFile(const char* fileName)
{
	/* open in BINARY MODE */
	ifstream inFile(fileName, ios::binary);
	if (inFile.is_open())
	{
		// Handle file I/O here
		// Seek & get the length
		inFile.seekg(0, ios::end);
		int length = (int)inFile.tellg();

		// Seek back to the beginning
		inFile.seekg(0, ios::beg);

		// Make an array to hold the file
		char* fileContents = new char[length + 1];

		// Read the file & set null pointer
		inFile.read(fileContents, length);
		fileContents[length] = 0;

		// Done with the file
		inFile.close();

		//return array from this function
		return fileContents;
	}
	else { //couldn't open the file
		cout << "Couldn't open " << fileName << endl;
		return 0;
	}
}
GLuint loadShader(const char* fileName, GLenum shaderType)
{
	GLuint shader;
	const char* fileContents = loadTextFile(fileName);
	//double check contents in console
	if (fileContents != 0) {
		
		//Create the shader
		shader = glCreateShader(shaderType);

		/* Set source code. Parameters
			1: Shader object index
			2 : How many strings make up the shader file code
			3 : Address of one or more strings(the code)
			4 : Array of string sizes – 0 means strings are null terminated
		*/
		glShaderSource(shader, 1, &fileContents, 0);

		//compilation may or may not succeed,
		//so we need to get shader info to confirm
		glCompileShader(shader);
		//clean up file contents either way
		delete[] fileContents;

		/* Get compilation results
			Param 1: the shader object index
			Param 2 : Which info to get ? GL_COMPILE_STATUS
			Param 3 : Address of integer(GLint) variable to store result
		*/
		GLint result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (result == GL_TRUE)
		{
			return shader; //YESSSSS
		} else
		{ //BOOOOO
			//Get length of compilation info log array
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
			//Make a char array of that (result) length for log
			char* logArr = new char[result];

			/* Get The Log. Param 1: Shader object index
				Param 2 : Number of characters to retrieve
				Param 3 : Address of int variable where log length will be stored(use zero – we already have length)
				Param 4 : Character array to hold the log data
			*/
			glGetShaderInfoLog(shader, result, 0, logArr);
			//dump log to console
			cout << logArr << endl;
			delete[] logArr; //clean up
			//report failure back
			glDeleteShader(shader);
			return 0;
		}
	} else
	{
		cout << "Couldn't load " << fileName << endl;
		return 0;
	}
}
GLuint loadShaderProgram(const char* vertexFile, const char* fragmentFile)
{
	//Call loadShader( ), passing in the vertex shader file path
	// and GL_VERTEX_SHADER. Save the resulting shader index in a
	// GLuint.  If the result was zero, there was an error, so 
	// also return zero here.
	GLuint vshader = loadShader(vertexFile, GL_VERTEX_SHADER);
	if (vshader == 0) {
		cout << "vshader load failed\n";
		return 0;
	}
	//Call loadShader( ) again, passing in the fragment shader
	// file path and GL_FRAGMENT_SHADER. Save the resulting
	// shader index in another GLuint, and if it was zero also
	// return zero here.
	GLuint fshader = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (fshader == 0) {
		cout << "fshader load failed\n";
		return 0;
	}
	//Create a shader program with glCreateProgram(),
	GLuint sProgram = glCreateProgram();

	//then use glAttachShader() twice to attach both shaders you loaded.
	glAttachShader(sProgram, vshader);
	glAttachShader(sProgram, fshader);

	//Finally, link the program with glLinkProgram().
	glLinkProgram(sProgram);

	//Determine if the program linked correctly by calling
	// glGetProgramiv().If it linked, return the index of the shader program
	GLint result;
	glGetProgramiv(sProgram, GL_LINK_STATUS, &result);
	if (result == GL_TRUE)
	{
		return sProgram; //YESSSSSS
	}
	else
	{ //BOOOOO
		//Get length of compilation info log array
		glGetProgramiv(sProgram, GL_INFO_LOG_LENGTH, &result);
		//Make a char array of that (result) length for log
		char* logArr = new char[result];

		/* Get The Log. Param 1: sProgram object index
		Param 2 : Number of characters to retrieve
		Param 3 : Address of int variable where log length will be stored(use zero – we already have length)
		Param 4 : Character array to hold the log data
		*/
		glGetProgramInfoLog(sProgram, result, 0, logArr);
		//dump log to console
		cout << logArr << endl;
		delete[] logArr; //clean up
		//report failure back
		glDeleteProgram(sProgram);
		return 0;
	}
}



/*
Sends the transformation matrix to the shader program
*/
void setShaderMatrix(GLuint index, const char * name, const glm::mat4 &matToSend)
{
	GLuint uniLoc = glGetUniformLocation(index, name);
	glProgramUniformMatrix4fv(index, uniLoc, 1,GL_FALSE, &matToSend[0][0]);
}

/*
Sends the color to the shader program
*/
void setShaderColor(GLuint pIndex, const char * varName, GLfloat red, GLfloat green, GLfloat blue)
{
	GLuint uniLoc = glGetUniformLocation(pIndex, varName);
	glProgramUniform4f(pIndex, uniLoc, red, green, blue, 1);
}

void setShaderValue(GLuint pIndex, const char * varName, int val)
{
	GLuint uniLoc = glGetUniformLocation(pIndex, varName);
	glProgramUniform1f(pIndex, uniLoc, (GLfloat)val);
}

/*
.obj reader. currently ignores normal and uv information, and assumes no texture
*/
void loadModel(const char* fileName, std::vector<glm::vec3> * vertices, std::vector<glm::vec2> * uvs, std::vector<glm::vec3> * normals, std::vector<unsigned int> * indices)
{
	//ignoring uvs and normals for now
	vector<int> vIndices;
	vector<int> uIndices;
	vector<int> nIndices;
	vector<glm::vec3> tVertices;
	vector<glm::vec2> tUvs;
	vector<glm::vec3> tNormals;

	FILE * inFile;
	fopen_s(&inFile, fileName, "r");
	while (true) {
		char lineType[32];
		int result = fscanf_s(inFile, "%s", lineType, 32);
		if (result == EOF) {
			break; //finished parsing
		}
		//cases
		if (strcmp(lineType, "v") == 0) { //vertex info
			glm::vec3 tVertex;
			fscanf_s(inFile, "%f %f %f\n", &tVertex.x, &tVertex.y, &tVertex.z);
			tVertices.push_back(tVertex);
		}
		else if (strcmp(lineType, "vt") == 0) {
			glm::vec2 tUV;
			fscanf_s(inFile, "%f %f\n", &tUV.x, &tUV.y);
			tUvs.push_back(tUV);
		}
		else if (strcmp(lineType, "vn") == 0) {
			glm::vec3 tNormal;
			fscanf_s(inFile, "%f %f %f\n", &tNormal.x, &tNormal.y, &tNormal.z);
			tNormals.push_back(tNormal);
		}
		else if (strcmp(lineType, "f") == 0) {
			int vertexIndex[3];
			int uvIndex[3];
			int normalIndex[3];
			fscanf_s(inFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
															 &vertexIndex[1], &uvIndex[1], &normalIndex[1], 
															 &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			vIndices.push_back(vertexIndex[0]);
			vIndices.push_back(vertexIndex[1]);
			vIndices.push_back(vertexIndex[2]);

			uIndices.push_back(uvIndex[0]);
			uIndices.push_back(uvIndex[1]);
			uIndices.push_back(uvIndex[2]);

			nIndices.push_back(normalIndex[0]);
			nIndices.push_back(normalIndex[1]);
			nIndices.push_back(normalIndex[2]);
		}
	}
	
	VecMap reindexed;
	//convert tVertices into the right order
	//cout << ".obj vertices:" << endl;

	int currTotalIndex = 0;

	for (unsigned int i = 0; i < vIndices.size(); i++) {
		

		int vIndex = vIndices[i] - 1;
		int uIndex = uIndices[i] - 1;
		int nIndex = nIndices[i] - 1;

		//current unique index combo
		glm::vec3 currCombo = glm::vec3(vIndex, uIndex, nIndex);

		if (reindexed.find(currCombo) == reindexed.end()) {
			// not found
			vertices->push_back(tVertices[vIndex]);
			uvs->push_back(tUvs[uIndex]);
			normals->push_back(tNormals[nIndex]);
			reindexed[currCombo] = currTotalIndex;
			indices->push_back(reindexed[currCombo]);
			currTotalIndex++;
		}
		else {
			// found
			indices->push_back(reindexed[currCombo]);
		}

		//indices->push_back(tIndex);
		//vertices->push_back(tVertices[tIndex]);
		
		//cout << "( " << tVertices[tIndex].x << ", " << tVertices[tIndex].y << ", " << tVertices[tIndex].z << " )" << endl;
	}

}
