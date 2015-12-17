#pragma once
#include <glew.h>
#include <string>
#include <unordered_map>
#include "Shape.h"

enum GAME_CONSTANT
{
	INT_SCORE,
	INT_LEVEL,

	STRING_LEVEL,

	BOOL_LEVEL_LOSE,
	BOOL_LEVEL_WIN,
	BOOL_GAME_OVER,
	BOOL_GAME_START,
};

enum EVENT
{
	COLLISION,
};

class GameData
{
public:
	static GameData& instance()
	{
		static GameData _instance;
		return _instance;
	}
	void init(GLuint p);

	//global identifers
	int  getUniqueID();

	//game variables
	void setInt(GAME_CONSTANT, int);
	void modifyInt(GAME_CONSTANT, int);
	int getInt(GAME_CONSTANT);

	void setBool(GAME_CONSTANT, bool);
	bool getBool(GAME_CONSTANT);

	//object store
	void loadObject(const char * name, bool skybox = false);
	Shape * getObject(const char * name);
	void loadTexture(const char * name);
	GLuint getTexture(const char * name);
private:
	int uid;
	GLuint programIndex;
	std::unordered_map<std::string, Shape *> _objects;
	std::unordered_map<std::string, GLuint> _textures;

	std::unordered_map<GAME_CONSTANT, int> _ints;
	std::unordered_map<GAME_CONSTANT, bool> _bools;

	GameData();
	GameData(GameData const&) = delete;
	void operator=(GameData const&) = delete;
};

