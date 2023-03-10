#pragma once

#include "../../LuaCpp2/LuaCpp2.h"

namespace Luaux {
	namespace Engine {

		enum class SeekOption {
			begp,
			curp,
			endp
		};

		class StreamBaseIfce {
		public:
			Lua::Function clear;
		};

		class SeekableStreamBaseIfce {
		public:
			SeekOption begp;
			SeekOption curp;
			SeekOption endp;
		};

	}
}

defhost_enum(Luaux::Engine::SeekOption)