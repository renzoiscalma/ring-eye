#pragma once
#include <lua.hpp>
#include "Blit3D.h"

class Lua
{
public:
	lua_State* L;
	Lua();
	bool ErrorCheck(int errroNum);

	int getMaxScore();
	void saveScore(int score); // appends the recent score to the file
};

