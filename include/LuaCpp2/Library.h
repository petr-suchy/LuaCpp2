#pragma once

namespace Lua {

	class Library {
	public:

		typedef void State;
		typedef long long Integer;
		typedef double Number;

		enum class Type {
			Boolean, CFunction, Function, Integer, LightUserdata,
			Nil, Number, String, Table, Thread, Userdata
		};

		struct Version {
			int Major;
			int Minor;
			int Release;
		};

		// Function prototype for a registered function.
		typedef int (*CFunction) (State* L);

		// Function prototype that reads blocks while loading chunks.
		typedef const char* (*Reader) (State* L, void* ud, size_t* sz);

		// Function prototype that writes blocks while dumping chunks.
		typedef int (*Writer) (State* L, const void* p, size_t sz, void* ud);

		// Function prototype that is called when a new state is created.
		typedef int (*Initializer) (State* L);

		// Function prototype that is called just before a state is deleted.
		typedef void (*Deleter) (State* L);

		static Library& inst();

		/* Version */

		static const int InterfaceVersionMajor = 1;
		static const int InterfaceVersionMinor = 0;

		// Gets the version of the interface.
		virtual const Version& getifcever() = 0;

		// Gets the version of the implementation.
		virtual const Version& getimplver() = 0;

		/* State */

		// Creates a new Lua state.
		virtual State* newstate() = 0;

		// Increments the internal reference counter of the given state
		// and returns that state.
		virtual State* lockstate(State* L) = 0;

		// Returns the number of objects that share the given state.
		virtual int usecount(State* L) = 0;

		// Destroys all objects in the given Lua state.
		virtual void close(State* L) = 0;

		// Opens all standard Lua libraries into the given state.
		virtual void openlibs(State* L) = 0;

		// Sets the given function to initialize new states.
		virtual void setstateinit(Initializer initializer) = 0;

		// Gets the function that is called when a new state is created.
		virtual Initializer getstateinit() = 0;

		// Sets the given function to delete states.
		virtual void setstatedelete(Deleter deleter) = 0;

		// Gets the function that is called just before a state is deleted.
		virtual Deleter getstatedelete() = 0;

		/* Stack */

		// Returns the index of the top element in the stack.
		virtual int gettop(State* L) = 0;

		// Sets the stack top to the given index.
		virtual void settop(State* L, int idx) = 0;

		// Ensures that there are extra free stack slots in the stack.
		virtual int checkstack(State* L, int n) = 0;

		/* Manipulation of elements */

		// Pops the given number of elements from the stack.
		virtual void pop(State* L, int n) = 0;

		// Pushes a copy of the element at the given index onto the stack.
		virtual void pushvalue(State* L, int idx) = 0;

		// Removes the element at the given index, shifting down the elements
		// above this index to fill the gap.
		virtual void remove(State* L, int idx) = 0;

		// Moves the top element into the given index, shifting up the elements
		// above this index to open space.
		virtual void insert(State* L, int idx) = 0;

		// Moves the top element into the given index without shifting any element.
		virtual void replace(State* L, int idx) = 0;

		/* Pushing elements */

		// Pushes a boolean value onto the stack.
		virtual void pushboolean(State* L, int b) = 0;

		// Pushes a closure onto the stack.
		virtual void pushcclosure(State* L, CFunction fn, int n) = 0;

		// Pushes a function onto the stack.
		virtual void pushcfunction(State* L, CFunction fn) = 0;

		// Pushes a integer value onto the stack.
		virtual void pushinteger(State* L, Integer n) = 0;

		// Pushes a light userdata onto the stack.
		virtual void pushlightuserdata(State* L, void* p) = 0;

		// Pushes a nil value onto the stack.
		virtual void pushnil(State* L) = 0;

		// Pushes a floating-point number onto the stack.
		virtual void pushnumber(State* L, Number n) = 0;

		// Pushes a null-terminated string onto the stack.
		virtual void pushstring(State* L, const char* s) = 0;

		// Pushes a string of the given length onto the stack.
		virtual void pushlstring(State* L, const char* s, size_t len) = 0;

		/* Testing elements */

		// Returns the type of the value in the given acceptable index,
		// or TNONE for a non-valid index
		virtual int type(State* L, int idx) = 0;

		// Returns the name of the given type.
		virtual const char* typetoname(State* L, int tp) = 0;

		// Returns the name of the given type.
		virtual const char* typetoname(Type tp) = 0;

		// Returns 1 if the value at the given acceptable index has type boolean, and 0 otherwise.
		virtual int isboolean(State* L, int idx) = 0;

		// Returns 1 if the value at the given acceptable index is a C function, and 0 otherwise.
		virtual int iscfunction(State* L, int idx) = 0;

		// Returns 1 if the value at the given acceptable index is a function (either C or Lua),
		// and 0 otherwise.
		virtual int isfunction(State* L, int idx) = 0;

		// Returns 1 if the value at the given acceptable index is a integer, and 0 otherwise.
		virtual int isinteger(State* L, int idx) = 0;

		// Returns 1 if the value at the given acceptable index is a light userdata,
		// and 0 otherwise.
		virtual int islightuserdata(State* L, int idx) = 0;

		// Returns 1 if the value at the given acceptable index is nil, and 0 otherwise.
		virtual int isnil(State* L, int idx) = 0;

		// Returns 1 if the value at the given acceptable index is a number or a string convertible
		// to a number, and 0 otherwise.
		virtual int isnumber(State* L, int idx) = 0;

		// Returns 1 if the value at the given acceptable index is a string, and 0 otherwise.
		virtual int isstring(State* L, int idx) = 0;

		// Returns 1 if the value at the given acceptable index is a table, and 0 otherwise.
		virtual int istable(State* L, int idx) = 0;

		// Returns 1 if the value at the given acceptable index is a thread, and 0 otherwise.
		virtual int isthread(State* L, int idx) = 0;

		// Returns 1 if the value at the given acceptable index is a userdata (either full or light),
		// and 0 otherwise.
		virtual int isuserdata(State* L, int idx) = 0;

		// Returns 1 if the value at the given acceptable index is of the specified type,
		// and 0 otherwise.
		virtual int isvalue(State* L, int idx, Type tp) = 0;

		/* Querying elements */

		// Converts the value at the given acceptable index to a boolean value.
		virtual int toboolean(State* L, int idx) = 0;

		// Converts the value at the given acceptable index to an integer value.
		virtual Integer tointeger(State* L, int idx) = 0;

		// Converts the value at the given acceptable index to a floating-point value.
		virtual Number tonumber(State* L, int idx) = 0;

		// Converts the value at the given acceptable index to a string.
		virtual const char* tostring(State* L, int idx) = 0;

		// Converts the value at the given acceptable index to a string.
		virtual const char* tolstring(State* L, int idx, size_t* len) = 0;

		// Returns a pointer to the userdata at the given acceptable index.
		virtual void* touserdata(State* L, int idx) = 0;

		// Returns the pseudo-index of the given upvalue.
		virtual int upvalueindex(int n) = 0;

		/* Tables */

		// Creates a new empty table and pushes it onto the stack.
		virtual void createtable(State* L, int narr, int nrec) = 0;

		// Does the equivalent to t[k] = v, where t is the value at the given valid index,
		// v is the value at the top of the stack, and k is the value just below the top.
		virtual void settable(State* L, int idx) = 0;

		// Pushes onto the stack the value t[k], where t is the value at the given valid index
		// and k is the value at the top of the stack.
		virtual void gettable(State* L, int idx) = 0;

		// Does the equivalent to t[k] = v, where t is the value at the given valid index
		// and v is the value at the top of the stack.
		virtual void setfield(State* L, int idx, const char* k) = 0;

		// Pushes onto the stack the value t[k], where t is the value at the given valid index.
		virtual void getfield(State* L, int idx, const char* k) = 0;

		// Pops a table from the stack and sets it as the new metatable
		// for the value at the given acceptable index.
		virtual void setmetatable(State* L, int idx) = 0;

		// Pushes onto the stack the metatable of the value at the given acceptable index,
		// or returns 0 if the value has no metatable.
		virtual int getmetatable(State* L, int idx) = 0;

		// Pops a key from the stack, and pushes a key-value pair from the table at the given index
		// (the "next" pair after the given key).
		virtual int next(State* L, int idx) = 0;

		/* Global variables */

		// Pops a value from the stack and sets it as the new value of global name.
		virtual void setglobal(State* L, const char* name) = 0;

		// Pushes onto the stack the value of the global name.
		virtual void getglobal(State* L, const char* name) = 0;

		/* Calling functions */

		// Calls a function in protected mode.
		virtual int pcall(State* L, int nargs, int nresults, int errfunc) = 0;

		// Calls a function in protected mode.
		virtual int pcall(State* L, int nargs, int nresults) = 0;

		// Calls a function in protected mode.
		virtual int pcall(State* L, int nargs) = 0;

		/* Load and dump chunks */

		// Loads a chunk.
		virtual int load(
			State* L, Reader reader, void* ud,
			const char* chunkname, const char* mode
		) = 0;

		// Dumps a function as a binary chunk.
		virtual int dump(State* L, Writer writer, void* ud, int strip) = 0;

		/* Userdata */

		// Allocates a new block of memory with the given size, pushes onto the stack
		// a new full userdata with the block address, and returns this address.
		virtual void* newuserdata(State* L, size_t size) = 0;

		/* References */

		// Creates and returns a reference to the value at the top of the stack
		// and pops the value.
		virtual int ref(State* L) = 0;

		// Pushes the value associated with the given reference onto the stack.
		virtual void getref(State* L, int ref) = 0;

		// Releases the given reference and its associated value.
		virtual void unref(State* L, int ref) = 0;

		// Returns 1 if the given reference is nil, and 0 otherwise.
		virtual int isrefnil(int ref) = 0;

		/* Errors */

		// Generates an error, using the value at the top of the stack as the error object.
		virtual int error(State* L) = 0;

		// Converts an error to a string.
		virtual size_t toerrorstring(State* L, int status, char* buff, size_t size) = 0;

	};

}