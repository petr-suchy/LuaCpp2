#pragma once

#include "IOStreamIfce.h"

namespace Luaux {
	namespace Engine {

		class FileStreamBaseIfce {
		public:
			Lua::Function __close;
			Lua::Function close;
		};

		///////////////////////////////////////////////////////

		class InputFileStreamIfce :
			public SeekableInputStreamIfce,
			public FileStreamBaseIfce {
		};

		class OutputFileStreamIfce :
			public SeekableOutputStreamIfce,
			public FileStreamBaseIfce {
		};

		class FileStreamIfce :
			public SeekableStreamIfce,
			public FileStreamBaseIfce {
		};

	}
}

defhost_struct(
	Luaux::Engine::InputFileStreamIfce,
	m.begp,
	m.curp,
	m.endp,
	m.clear,
	m.read,
	m.tellg,
	m.seekg,
	m.__close,
	m.close
)

defhost_struct(
	Luaux::Engine::OutputFileStreamIfce,
	m.begp,
	m.curp,
	m.endp,
	m.clear,
	m.write,
	m.flush,
	m.tellp,
	m.seekp,
	m.__close,
	m.close
)

defhost_struct(
	Luaux::Engine::FileStreamIfce,
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
	m.__close,
	m.close
)