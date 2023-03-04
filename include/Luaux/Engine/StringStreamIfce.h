#pragma once

#include "IOStreamIfce.h"

namespace Luaux {
	namespace Engine {

		class StringStreamBaseIfce {
		public:
			Lua::Function str;
		};

		///////////////////////////////////////////////////////

		class InputStringStreamIfce :
			public SeekableInputStreamIfce,
			public StringStreamBaseIfce {
		};

		class OutputStringStreamIfce :
			public SeekableOutputStreamIfce,
			public StringStreamBaseIfce {
		};

		class StringStreamIfce :
			public SeekableStreamIfce,
			public StringStreamBaseIfce {
		};

	}
}

defhost_struct(
	Luaux::Engine::InputStringStreamIfce,
	m.begp,
	m.curp,
	m.endp,
	m.clear,
	m.read,
	m.tellg,
	m.seekg,
	m.str
)

defhost_struct(
	Luaux::Engine::OutputStringStreamIfce,
	m.begp,
	m.curp,
	m.endp,
	m.clear,
	m.write,
	m.flush,
	m.tellp,
	m.seekp,
	m.str
)

defhost_struct(
	Luaux::Engine::StringStreamIfce,
	m.begp,
	m.curp,
	m.endp,
	m.clear,
	m.read,
	m.tellg,
	m.seekg,
	m.write,
	m.flush,
	m.tellp,
	m.seekp,
	m.str
)