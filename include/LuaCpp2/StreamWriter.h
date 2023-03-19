#pragma once

#include "State.h"

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
			State state(L);
			auto writer = reinterpret_cast<StreamWriter*>(obj);

			writer->_os.write((const char*) toWrite, len);

			if (writer->_os.fail()) {
				state.pushString("writing to output stream failed");
				return 1;
			}

			return 0;
		}

	private:

		char _buff[512];
		OutputStream& _os;

	};

}