#pragma once

#include "StreamBase.h"
#include "Input.h"
#include "SeekableInput.h"

namespace Luaux {
	namespace Host {


		template<typename Char, typename Size, typename Pos>
		class SeekableInputStream :
			public StreamBase,
			public SeekableStreamBase,
			public Input<Char, Size>,
			public SeekableInput<Char, Size, Pos> {

		public:

			SeekableInputStream(Engine::SeekableInputStreamIfce& inputStream) :
				StreamBase(inputStream),
				SeekableStreamBase(inputStream),
				Input<Char, Size>(inputStream),
				SeekableInput<Char, Size, Pos>(inputStream)
			{}

			virtual void seteof() { _eof = true; }
			virtual void setfail() { _fail = true; }

		};

	}
}