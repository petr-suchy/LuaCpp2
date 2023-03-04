#pragma once

#include "StreamBase.h"
#include "Input.h"
#include "Output.h"
#include "../Engine/IOStreamIfce.h"

namespace Luaux {
	namespace Host {

		template<typename Char, typename Size>
		class Stream :
			public StreamBase,
			public Input<Char, Size>,
			public Output<Char, Size> {

		public:

			Stream(Engine::StreamIfce& stream) :
				StreamBase(stream),
				Input<Char, Size>(stream),
				Output<Char, Size>(stream)
			{}

			virtual void seteof() { _eof = true; }
			virtual void setfail() { _fail = true; }

		};

	}
}