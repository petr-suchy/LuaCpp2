#pragma once

#include "IStreamImpl.h"
#include "OStreamImpl.h"
#include "IOStreamIfce.h"

namespace Luaux {
	namespace Engine {

		template< typename IOStream>
		static void ImplStream(StreamIfce* ifce, IOStream iostream)
		{
			ImplStreamBase(ifce, iostream);
			ImplInput(ifce, iostream);
			ImplOutput(ifce, iostream);
		}

		template< typename IOStream>
		static void ImplSeekableStream(SeekableStreamIfce* ifce, IOStream iostream)
		{
			ImplStreamBase(ifce, iostream);
			ImplInput(ifce, iostream);
			ImplOutput(ifce, iostream);
			ImplSeekableStreamBase(ifce, iostream);
			ImplSeekableInput(ifce, iostream);
			ImplSeekableOutput(ifce, iostream);
		}

		///////////////////////////////////////////////////////

		template<typename IOStream>
		class StreamImpl : public StreamIfce {
		public:

			StreamImpl(IOStream iostream)
			{
				ImplStream(this, iostream);
			}

		};

		template<typename IOStream>
		class SeekableStreamImpl : public SeekableStreamIfce {
		public:

			SeekableStreamImpl(IOStream iostream)
			{
				ImplSeekableStream(this, iostream);
			}

		};


		/////////////////////////////////////////////

		template<typename IOStream>
		static StreamIfce MakeStream(IOStream iostream)
		{
			return StreamImpl<IOStream>(iostream);
		}

		template<typename IOStream>
		static SeekableStreamIfce MakeSeekableStream(IOStream iostream)
		{
			return SeekableStreamImpl<IOStream>(iostream);
		}

	}
}