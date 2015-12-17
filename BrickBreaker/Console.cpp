#include "Console.h"
#include <iostream>

void Console::Log(const char* msg)
{
	std::cout << "Log: " << msg << std::endl;
}

void Console::Log(int msg)
{
	std::cout << "Log: " << msg << std::endl;
}

void Console::Log(float msg)
{
	std::cout << "Log: " << msg << std::endl;
}

void Console::Log(std::string msg)
{
	std::cout << "Log: " << msg << std::endl;
}
