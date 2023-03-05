#pragma once

namespace Lua {

	template<typename InputStream>
	class StreamReader {
	public:

		StreamReader(InputStream& is) :
			_is(is),
			_buff("")
		{}

		static const char* cb(lua_State *L, void* data, size_t* size)
		{
			StreamReader* reader = reinterpret_cast<StreamReader*>(data);

			reader->_is.read(reader->_buff, sizeof(reader->_buff));
			*size = (size_t)reader->_is.gcount();

			return reader->_buff;
		}

	private:

		char _buff[512];
		InputStream& _is;

	};

}