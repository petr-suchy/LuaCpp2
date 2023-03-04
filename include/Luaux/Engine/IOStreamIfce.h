#pragma once

#include "IStreamIfce.h"
#include "OStreamIfce.h"

namespace Luaux {
	namespace Engine {

		class StreamIfce :
			public StreamBaseIfce,
			public InputIfce,
			public OutputIfce {
		};

		class SeekableStreamIfce :
			public StreamBaseIfce,
			public SeekableStreamBaseIfce,
			public InputIfce,
			public SeekableInputIfce,
			public OutputIfce,
			public SeekableOutputIfce {
		};

	}
}

defhost_struct(
	Luaux::Engine::StreamIfce,
	m.clear,
	m.read,
	m.write,
	m.flush
)

defhost_struct(
	Luaux::Engine::SeekableStreamIfce,
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
	m.seekp
)