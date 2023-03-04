#pragma once

#include "StreamBase.h"
#include "Input.h"

namespace Luaux {
	namespace Host {

		template<typename Char, typename Size>
		class InputStream :
			public StreamBase,
			public Input<Char, Size> {

		public:

			InputStream(Engine::InputStreamIfce& inputStream) :
				StreamBase(inputStream),
				Input<Char, Size>(inputStream)
			{}

			virtual void seteof() { _eof = true; }
			virtual void setfail() { _fail = true; }

		};

	}
}