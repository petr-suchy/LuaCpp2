#pragma once

#include "IOStreamImpl.h"
#include "StringStreamIfce.h"

namespace Luaux {
	namespace Engine {

		template<typename SStream>
		static void ImplStringStreamBase(StringStreamBaseIfce* ifce, SStream sstream)
		{
			ifce->str = Lua::MakeFunc(
				[sstream] (Lua::Args args, Lua::Lua lua) mutable
				{
					if (!args.in().atEnd()) {

						std::string str;
						args.in() >> str;

						sstream->str(str);

						return;
					}
					
					args.out() << sstream->str();
				}
			);

		}

		///////////////////////////////////////////////////////

		template<typename ISStream>
		class InputStringStreamImpl : public InputStringStreamIfce {
		public:

			InputStringStreamImpl(ISStream isstream)
			{
				ImplSeekableInputStream(this, isstream);
				ImplStringStreamBase(this, isstream);
			}

		};

		template<typename OSStream>
		class OutputStringStreamImpl : public OutputStringStreamIfce {
		public:

			OutputStringStreamImpl(OSStream osstream)
			{
				ImplSeekableOutputStream(this, osstream);
				ImplStringStreamBase(this, osstream);
			}

		};

		template<typename SStream>
		class StringStreamImpl : public StringStreamIfce {
		public:

			StringStreamImpl(SStream sstream)
			{
				ImplSeekableStream(this, sstream);
				ImplStringStreamBase(this, sstream);
			}

		};

		///////////////////////////////////////////////////////

		template<typename ISStream>
		static InputStringStreamIfce MakeInputStringStream(ISStream isstream)
		{
			return InputStringStreamImpl<ISStream>(isstream);
		}

		template<typename OSStream>
		static OutputStringStreamIfce MakeOutputStringStream(OSStream osstream)
		{
			return OutputStringStreamImpl<OSStream>(osstream);
		}

		template<typename SStream>
		static StringStreamIfce MakeStringStream(SStream sstream)
		{
			return StringStreamImpl<SStream>(sstream);
		}

	}
}