#pragma once

#include "StreamBaseIfce.h"

namespace Luaux {
	namespace Engine {

		class OutputIfce {
		public:
			Lua::Function write;
			Lua::Function flush;
		};

		class SeekableOutputIfce {
		public:

			Lua::Function tellp;
			Lua::Function seekp;

		};

		///////////////////////////////////////////////////////

		class OutputStreamIfce :
			public StreamBaseIfce,
			public OutputIfce {
		};

		class SeekableOutputStreamIfce :
			public StreamBaseIfce,
			public SeekableStreamBaseIfce,
			public OutputIfce,
			public SeekableOutputIfce {
		};

	}
}

defhost_struct(
	Luaux::Engine::OutputStreamIfce,
	m.clear,
	m.write,
	m.flush
)

defhost_struct(
	Luaux::Engine::SeekableOutputStreamIfce,
	m.begp,
	m.curp,
	m.endp,
	m.clear,
	m.write,
	m.flush,
	m.tellp,
	m.seekp
)