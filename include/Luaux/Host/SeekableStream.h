#pragma once

#include "Host/StreamBase.h"
#include "Host/Input.h"
#include "Host/Output.h"
#include "Host/SeekableInput.h"
#include "Host/SeekableOutput.h"
#include "Engine/IOStreamIfce.h"

namespace Luaux {
	namespace Host {

		template<typename Char, typename Size, typename Pos>
		class SeekableStream :
			public StreamBase,
			public SeekableStreamBase,
			public Input<Char, Size>,
			public SeekableInput<Char, Size, Pos>,
			public Output<Char, Size>,
			public SeekableOutput<Char, Size, Pos> {

		public:

			SeekableStream(Engine::SeekableStreamIfce& seekableStream) :
				StreamBase(seekableStream),
				SeekableStreamBase(seekableStream),
				Input<Char, Size>(seekableStream),
				SeekableInput<Char, Size, Pos>(seekableStream),
				Output<Char, Size>(seekableStream),
				SeekableOutput<Char, Size, Pos>(seekableStream)
			{}

			virtual void seteof() { _eof = true; }
			virtual void setfail() { _fail = true; }

		};

	}
}