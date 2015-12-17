#include "SoundManager.h"
#include <string>
#include <Windows.h>
#include <mmsystem.h>

void SoundManager::PlayClip(const char * name)
{
	std::string b = "sound/";
	std::string e = ".wav";
	std::string sound = b + name + e;
	PlaySound(sound.c_str(), NULL, SND_ASYNC | SND_FILENAME | SND_NOSTOP);
}