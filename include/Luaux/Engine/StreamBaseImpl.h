#pragma once

#include "StreamBaseIfce.h"

namespace Luaux {
	namespace Engine {

		template<typename IOStream>
		static void ImplStreamBase(StreamBaseIfce* ifce, IOStream iostream)
		{
			ifce->clear = Lua::MakeFunc(
				[iostream] (Lua::Args args, Lua::Lua lua) mutable
				{
					iostream->clear();
				}
			);
		}

		template<typename IOStream>
		static void ImplSeekableStreamBase(SeekableStreamBaseIfce* ifce, IOStream iostream)
		{
			ifce->begp = SeekOption::begp;;
			ifce->curp = SeekOption::curp;
			ifce->endp = SeekOption::endp;
		}

	}
}