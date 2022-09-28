#pragma once

#include <string>

extern "C" {
#include "lualib.h"
}

namespace Lua {

	static std::string TypeToName(int type)
	{
		std::string name;

		switch (type) {
			case LUA_TNIL: name = "nil"; break;
			case LUA_TBOOLEAN: name = "boolean"; break;
			case LUA_TLIGHTUSERDATA: name = "lightuserdata"; break;
			case LUA_TNUMBER: name = "number"; break;
			case LUA_TSTRING: name = "string"; break;
			case LUA_TTABLE: name = "table"; break;
			case LUA_TFUNCTION: name = "function"; break;
			case LUA_TUSERDATA: name = "userdata"; break;
			case LUA_TTHREAD: name = "thread"; break;
			default: name = "unknown"; break;
		}

		return name;
	}

}