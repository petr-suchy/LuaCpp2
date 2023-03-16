#pragma once

#include "Library.h"
#include "lua.hpp"

namespace Lua {

    template<typename NewState>
    class LinkedLibrary : public Library {
    public:

        /* State manipulation */

        virtual State* newstate()
        {
            return NewState{}();
        }

        virtual void close(State* L)
        {
            lua_close(L);
        }

        /* Stack */

        virtual int gettop(State* L)
        {
            return lua_gettop(L);
        }

        virtual void settop(State* L, int idx)
        {
            lua_settop(L, idx);
        }

        virtual int checkstack(State* L, int n)
        {
            return lua_checkstack(L, n);
        }

		/* Manipulation of elements */

		virtual void pop(State* L, int n)
		{
			lua_pop(L, n);
		}

		virtual void pushvalue(State* L, int idx)
		{
			lua_pushvalue(L, idx);
		}

		virtual void remove(State* L, int idx)
		{
			lua_remove(L, idx);
		}

		virtual void insert(State* L, int idx)
		{
			lua_insert(L, idx);
		}

		virtual void replace(State* L, int idx)
		{
			lua_replace(L, idx);
		}

		/* Pushing elements */

		virtual void pushboolean(State* L, int b)
		{
			lua_pushboolean(L, b);
		}

		virtual void pushcclosure(State* L, CFunction fn, int n)
		{
			lua_pushcclosure(L, fn, n);
		}

		virtual void pushcfunction(State* L, CFunction fn)
		{
			lua_pushcfunction(L, fn);
		}

		virtual void pushinteger(State* L, Integer n)
		{
			lua_pushinteger(L, n);
		}

		virtual void pushnil(State* L)
		{
			lua_pushnil(L);
		}

		virtual void pushnumber(State* L, Number n)
		{
			lua_pushnumber(L, n);
		}

		virtual void pushstring(State* L, const char* s)
		{
			lua_pushstring(L, s);
		}

		virtual void pushlstring(State* L, const char* s, size_t len)
		{
			lua_pushlstring(L, s, len);
		}

		/* Testing elements */

		virtual int type(State* L, int idx)
		{
			return lua_type(L, idx);
		}

		virtual int isboolean(State* L, int idx)
		{
			return lua_isboolean(L, idx);
		}

		virtual int iscfunction(State* L, int idx)
		{
			return lua_iscfunction(L, idx);
		}

		virtual int isfunction(State* L, int idx)
		{
			return lua_isfunction(L, idx);
		}

		virtual int isinteger(State* L, int idx)
		{
			return lua_isinteger(L, idx);
		}

		virtual int islightuserdata(State* L, int idx)
		{
			return lua_islightuserdata(L, idx);
		}

		virtual int isnil(State* L, int idx)
		{
			return lua_isnil(L, idx);
		}

		virtual int isnumber(State* L, int idx)
		{
			return lua_isnumber(L, idx);
		}

		virtual int isstring(State* L, int idx)
		{
			return lua_isstring(L, idx);
		}

		virtual int istable(State* L, int idx)
		{
			return lua_istable(L, idx);
		}

		virtual int isthread(State* L, int idx)
		{
			return lua_isthread(L, idx);
		}

		virtual int isuserdata(State* L, int idx)
		{
			return lua_isuserdata(L, idx);
		}

		/* Querying elements */

		virtual int toboolean(State* L, int idx)
		{
			return lua_toboolean(L, idx);
		}

		virtual Integer tointeger(State* L, int idx)
		{
			return lua_tointeger(L, idx);
		}

		virtual Number tonumber(State* L, int idx)
		{
			return lua_tonumber(L, idx);
		}

		virtual const char* tostring(State* L, int idx)
		{
			return lua_tostring(L, idx);
		}

		virtual const char* tolstring(State* L, int idx, size_t* len)
		{
			return lua_tolstring(L, idx, len);
		}

		virtual int upvalueindex(int n)
		{
			return lua_upvalueindex(n);
		}

		/* Tables */

		virtual void createtable(State* L, int narr, int nrec)
		{
			lua_createtable(L, narr, nrec);
		}

		virtual void settable(State* L, int idx)
		{
			lua_settable(L, idx);
		}

		virtual void gettable(State* L, int idx)
		{
			lua_gettable(L, idx);
		}

		virtual void setfield(State* L, int idx, const char* k)
		{
			lua_setfield(L, idx, k);
		}

		virtual void getfield(State* L, int idx, const char* k)
		{
			lua_getfield(L, idx, k);
		}

		virtual void setmetatable(State* L, int idx)
		{
			lua_setmetatable(L, idx);
		}

		virtual int getmetatable(State* L, int idx)
		{
			return lua_getmetatable(L, idx);
		}

		virtual int next(State* L, int idx)
		{
			return lua_next(L, idx);
		}

		/* Global variables */

		virtual void setglobal(State* L, const char* name)
		{
			lua_setglobal(L, name);
		}

		virtual void getglobal(State* L, const char* name)
		{
			lua_getglobal(L, name);
		}

		/* Calling functions */

		virtual int pcall(State* L, int nargs, int nresults, int errfunc)
		{
			return lua_pcall(L, nargs, nresults, errfunc);
		}

		virtual int pcall(State* L, int nargs, int nresults)
		{
			return lua_pcall(L, nargs, nresults, 0);
		}

		virtual int pcall(State* L, int nargs)
		{
			return lua_pcall(L, nargs, LUA_MULTRET, 0);
		}

		/* Load and dump chunks */

		virtual int load(
			State* L, Reader reader, void* ud,
			const char* chunkname, const char* mode
		)
		{
			return lua_load(L, reader, ud, chunkname, mode);
		}

		virtual int dump(State* L, Writer writer, void* ud, int strip)
		{
			return lua_dump(L, writer, ud, strip);
		}

    };

}