#pragma once

extern "C" {
#include "lualib.h"
}

namespace Lua {

	template<typename OutputStream>
	class StreamWriter {
	public:

		StreamWriter(OutputStream& os) :
			_os(os)
		{}

		static int cb(
			Library::State*L,
			const void* toWrite,
			size_t len,
			void* obj
		)
		{
			auto writer = reinterpret_cast<StreamWriter*>(obj);

			writer->_os.write((const char*) toWrite, len);

			if (writer->_os.fail()) {
				lua_pushstring(L, "writing to output stream failed");
				return LUA_ERRRUN;
			}

			return LUA_OK;
		}

	private:

		char _buff[512];
		OutputStream& _os;

	};

}