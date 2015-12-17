#include "GameManager.h"
#include "GameData.h"
#include "Console.h"
#include "ScreenSplash.h"
#include "ScreenHUD.h"

GameManager::GameManager(GLFWwindow *pPointer, GLuint pProgram)
{
	windowPtr = pPointer;
	program = pProgram;
	GameData::instance().init(program);
	load();
	//GameData::instance().setInt(GAME_CONSTANT::INT_LEVEL, 2);
	//_world = new GameWorld(pPointer, pProgram);
	_ui = new ScreenSplash(windowPtr, program);


}

void GameManager::keyPress(GLFWwindow* windowPtr, int key, int scancode, int action, int mods)
{
	_ui->keyPress(windowPtr, key, scancode, action, mods);
	if (_world != nullptr)
		_world->keyPress(windowPtr, key, scancode, action, mods);
}

void GameManager::mouseMove(GLFWwindow* windowPtr, double x, double y) {
	_ui->mouseMove(windowPtr, x, y);
	if (_world != nullptr)
		_world->mouseMove(windowPtr, x, y);
}

void GameManager::mouseClick(GLFWwindow * windowPtr, int button, int action, int mods) {
	_ui->mouseClick(windowPtr, button, action, mods);
	if (_world != nullptr)
		_world->mouseClick(windowPtr, button, action, mods);
	
}

void GameManager::load() {
	Console::Log("Loading textures");
	//GameData::instance().loadTexture("textures/bricktex.png");
	GameData::instance().loadTexture("textures/metal.jpg");
	GameData::instance().loadTexture("textures/brick1.png");
	GameData::instance().loadTexture("textures/brick2.png");
	GameData::instance().loadTexture("textures/brick3.png");
	GameData::instance().loadTexture("textures/stone.jpg");
	GameData::instance().loadTexture("textures/title.png");
	GameData::instance().loadTexture("textures/ui.png");
	GameData::instance().loadTexture("textures/sky.png");

	Console::Log("All textures loaded");

	Console::Log("Loading objects");
	GameData::instance().loadObject("objects/cube.obj");
	GameData::instance().loadObject("objects/sphere.obj");
	GameData::instance().loadObject("objects/ui_quad.obj", true);
	GameData::instance().loadObject("objects/skybox.obj", true);
	//fill with objs


	Console::Log("All objects loaded");
}

void GameManager::setDefaults()
{
	GameData::instance().setInt(GAME_CONSTANT::INT_SCORE, 0);
	GameData::instance().setBool(GAME_CONSTANT::BOOL_GAME_START, false);
	GameData::instance().setBool(GAME_CONSTANT::BOOL_GAME_OVER, false);
	GameData::instance().setBool(GAME_CONSTANT::BOOL_LEVEL_LOSE, false);
	GameData::instance().setBool(GAME_CONSTANT::BOOL_LEVEL_WIN, false);
}

void GameManager::endLevel()
{
	std::string score = "Level Score: " + std::to_string(GameData::instance().getInt(GAME_CONSTANT::INT_SCORE));
	Console::Log(score);
	delete _world;
	setDefaults();
	_world = new GameWorld(windowPtr, program);
}

void GameManager::endGame()
{
	Console::Log("End Game");
	std::string score = "Level Score: " + std::to_string(GameData::instance().getInt(GAME_CONSTANT::INT_SCORE));
	Console::Log(score);
	setDefaults();
	GameData::instance().setInt(GAME_CONSTANT::INT_LEVEL, 0);
	delete _world;
	_world = nullptr;
	delete _ui;
	_ui = new ScreenSplash(windowPtr, program);
}

void GameManager::startGame()
{
	Console::Log("Start Game");
	setDefaults();
	_world = new GameWorld(windowPtr, program);
	delete _ui;
	_ui = new ScreenHUD(windowPtr, program);
}

void GameManager::run() {




	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	//glfwSetCursorPos(windowPtr, 400, 300);


	float previousTime = float(glfwGetTime());
	while (!glfwWindowShouldClose(windowPtr)) {
		float currentTime = float(glfwGetTime());
		float dt = currentTime - previousTime;
		previousTime = currentTime;

		if (_world != nullptr)
			_world->update(dt);
		_ui->update(dt);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (_world != nullptr)
			_world->draw();
		_ui->draw();

		glfwSwapBuffers(windowPtr);

		glfwPollEvents();

		//flow check
		if (GameData::instance().getBool(GAME_CONSTANT::BOOL_LEVEL_LOSE)) {
			Console::Log("Level lose!");
			endLevel();
		}
		else if (GameData::instance().getBool(GAME_CONSTANT::BOOL_LEVEL_WIN)) {
			GameData::instance().modifyInt(GAME_CONSTANT::INT_LEVEL, 1);
			Console::Log("Level win!");
			endLevel();
		}
		else if (GameData::instance().getBool(GAME_CONSTANT::BOOL_GAME_OVER)) {
			endGame();
		} 
		else if (GameData::instance().getBool(GAME_CONSTANT::BOOL_GAME_START)) {
			startGame();
		}
	}

	glfwTerminate();
}


GameManager::~GameManager()
{
}
