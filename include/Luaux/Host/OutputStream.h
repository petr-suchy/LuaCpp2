#pragma once

#include "StreamBase.h"
#include "Output.h"

namespace Luaux {
	namespace Host {

		template<typename Char, typename Size>
		class OutputStream :
			public StreamBase,
			public Output<Char, Size> {

		public:

			OutputStream(Engine::OutputStreamIfce& outputStream) :
				StreamBase(outputStream),
				Output<Char, Size>(outputStream)
			{}

			virtual void seteof() { _eof = true; }
			virtual void setfail() { _fail = true; }

		};

	}
}