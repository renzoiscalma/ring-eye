#include "Lua.h"

Lua::Lua () {
	//initialize Lua
	L = lua_open();
	//load Lua base libraries
	luaL_openlibs(L);
	//load the script...this loads the GetNum() function, but
	//doesn't execute it yet.
	if (!ErrorCheck(luaL_dofile(L, "main.lua")))
	{
		//DO NOT CLEAR THE ERROR STRING OFF THE STACK
		//AFTER luaL_dofile() OR AN EXCEPTION GETS THROWN

		std::cout << "ERROR processing script.lua\n\n";
	}
};

//error handling function for Lua calls
//ErrorCheck() will print out some error info
//DO NOT CLEAR THE ERROR STRING OFF THE STACK
bool Lua::ErrorCheck(int errorNum)
{
	if (errorNum)
	{
		//There was an error!
		std::cout << "ERROR!\n";
		std::cout << "There was an error calling Lua.\n";
		switch (errorNum)
		{
		case LUA_ERRRUN:
			std::cout << "There was a runtime error.\n";
			break;

		case LUA_ERRMEM:
			std::cout << "There was a memory allocation error (this should be unusual!).\n";
			break;

		case LUA_ERRERR:
			std::cout << "There was a error-handler error (this should be unusual!).\n";
			break;

		case LUA_ERRSYNTAX:
			std::cout << "There was a syntax error.\n";
			break;

		default:
			std::cout << "There was an unknown error.\n";
		}

		std::cout << "Error string from Lua:\n";

		//get error string off the stack
		std::string errorString = lua_tostring(L, -1);
		std::cout << errorString << "\n";

		return false;
	}

	return true;
};

int Lua::getMaxScore() {
	lua_getglobal(L, "get_max_scores");
	lua_call(L, 0, 1);
	int maxScore = lua_tonumber(L, 1);
	return maxScore;
}

void Lua::saveScore(int score) {
	lua_getglobal(L, "save_scores");
	lua_pushinteger(L, score);
	lua_call(L, 1, 1);
}