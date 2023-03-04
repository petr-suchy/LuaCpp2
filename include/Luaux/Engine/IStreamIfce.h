#pragma once

#include "StreamBaseIfce.h"

namespace Luaux {
	namespace Engine {

		class InputIfce {
		public:
			Lua::Function read;
		};

		class SeekableInputIfce {
		public:
			Lua::Function tellg;
			Lua::Function seekg;
		};

		///////////////////////////////////////////////////////

		class InputStreamIfce :
			public StreamBaseIfce,
			public InputIfce {
		};

		class SeekableInputStreamIfce :
			public StreamBaseIfce,
			public SeekableStreamBaseIfce,
			public InputIfce,
			public SeekableInputIfce {
		};

	}
}

defhost_struct(
	Luaux::Engine::InputStreamIfce,
	m.clear,
	m.read
)

defhost_struct(
	Luaux::Engine::SeekableInputStreamIfce,
	m.begp,
	m.curp,
	m.endp,
	m.clear,
	m.read,
	m.tellg,
	m.seekg
)