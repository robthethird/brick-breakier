#include "GameData.h"
#include "Console.h"
#include <SOIL.h>
#include "shaderHelpers.h"

GameData::GameData()
{
	uid = 0;
}

void GameData::init(GLuint p)
{
	programIndex = p;
}

void GameData::loadObject(const char * name, bool flat)
{
	
	Shape * s = new Shape(name, programIndex, GL_TRIANGLES, flat);
	std::string trunc = name;
	trunc = trunc.substr(0, trunc.find("."));
	trunc = trunc.substr(trunc.find("/") + 1);
	Console::Log(trunc.c_str());
	_objects[trunc] = s;
}

Shape * GameData::getObject(const char * name)
{
	return _objects[std::string(name)];
}

void GameData::loadTexture(const char * name)
{

	GLuint texID = SOIL_load_OGL_texture(
		name, SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	std::string trunc = name;
	trunc = trunc.substr(0, trunc.find("."));
	trunc = trunc.substr(trunc.find("/") + 1);
	Console::Log(trunc.c_str());
	_textures[trunc] = texID;
}

GLuint GameData::getTexture(const char * name)
{
	return _textures[std::string(name)];
}

int GameData::getUniqueID()
{
	int tid = uid;
	uid++;
	return tid;
}

void GameData::setInt(GAME_CONSTANT g, int i)
{
	_ints[g] = i;
}

void GameData::modifyInt(GAME_CONSTANT g, int i)
{
	_ints[g] += i;
}

int GameData::getInt(GAME_CONSTANT g)
{
	return _ints[g];
}

void GameData::setBool(GAME_CONSTANT g, bool b)
{
	_bools[g] = b;
}

bool GameData::getBool(GAME_CONSTANT g)
{
	return _bools[g];
}
