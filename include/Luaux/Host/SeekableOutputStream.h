#pragma once

#include "StreamBase.h"
#include "Output.h"
#include "SeekableOutput.h"

namespace Luaux {
	namespace Host {

		template<typename Char, typename Size, typename Pos>
		class SeekableOutputStream :
			public StreamBase,
			public SeekableStreamBase,
			public Output<Char, Size>,
			public SeekableOutput<Char, Size, Pos> {

		public:

			SeekableOutputStream(Engine::SeekableOutputStreamIfce& outputStream) :
				StreamBase(outputStream),
				SeekableStreamBase(outputStream),
				Output<Char, Size>(outputStream),
				SeekableOutput<Char, Size, Pos>(outputStream)
			{}

			virtual void seteof() { _eof = true; }
			virtual void setfail() { _fail = true; }

		};

	}
}