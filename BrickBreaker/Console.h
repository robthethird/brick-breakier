#pragma once
#include <string>

class Console
{
public:
	static void Log(const char* msg);
	static void Log(int msg);
	static void Log(float msg);
	static void Log(std::string msg);
};

