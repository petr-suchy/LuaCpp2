#pragma once

#include "StreamBaseImpl.h"
#include "OStreamIfce.h"

namespace Luaux {
	namespace Engine {

		template< typename OStream>
		static void ImplOutput(OutputIfce* ifce, OStream ostream)
		{
			ifce->write = Lua::MakeFunc(
				[ostream] (Lua::Args args, Lua::Lua lua) mutable
				{
					std::string data;
					args.in() >> data;
						
					ostream->write(data.c_str(), data.length());

					if (ostream->fail()) {
						throw std::runtime_error(
							"failed to write data to stream"
						);
					}
				}
			);

			ifce->flush = Lua::MakeFunc(
				[ostream] (Lua::Args args, Lua::Lua lua) mutable
				{
					ostream->flush();
				}
			);
		}

		template<typename OStream>
		static void ImplSeekableOutput(SeekableOutputIfce* ifce, OStream ostream)
		{
			ifce->tellp = Lua::MakeFunc(
				[ostream] (Lua::Args args, Lua::Lua lua) mutable
				{
					args.out() << ostream->tellp();
				}
			);
			
			ifce->seekp = Lua::MakeFunc(
				[ostream] (Lua::Args args, Lua::Lua lua) mutable
				{
					size_t off;
					args.in() >> off;

					SeekOption way = SeekOption::begp;

					if (!args.in().atEnd()) {
						args.in() >> way;
					}

					switch (way) {
						case SeekOption::begp:
							ostream->seekp(off, ostream->beg);
						break;

						case SeekOption::curp:
							ostream->seekp(off, ostream->cur);
						break;

						case SeekOption::endp:
							ostream->seekp(off, ostream->end);
						break;

						default:
							throw std::logic_error("invalid argument");
					}

				}
			);
		}

		/////////////////////////////////////////////////////

		template< typename OStream>
		static void ImplOutputStream(OutputStreamIfce* ifce, OStream ostram)
		{
			ImplStreamBase(ifce, ostram);
			ImplOutput(ifce, ostram);
		}

		template< typename OStream>
		static void ImplSeekableOutputStream(SeekableOutputStreamIfce* ifce, OStream ostram)
		{
			ImplStreamBase(ifce, ostram);
			ImplOutput(ifce, ostram);
			ImplSeekableStreamBase(ifce, ostram);
			ImplSeekableOutput(ifce, ostram);
		}

		///////////////////////////////////////////////////////

		template<typename OStream>
		class OutputStreamImpl : public OutputStreamIfce {
		public:

			OutputStreamImpl(OStream ostram)
			{
				ImplOutputStream(this, ostram);
			}

		};

		template<typename OStream>
		class SeekableOutputStreamImpl : public SeekableOutputStreamIfce {
		public:

			SeekableOutputStreamImpl(OStream ostram)
			{
				ImplSeekableOutputStream(this, ostram);
			}

		};


		/////////////////////////////////////////////

		template<typename OStream>
		static OutputStreamIfce MakeOutputStream(OStream ostram)
		{
			return OutputStreamImpl<OStream>(ostram);
		}

		template<typename OStream>
		static SeekableOutputStreamIfce MakeSeekableOutputStream(OStream ostram)
		{
			return SeekableOutputStreamImpl<OStream>(ostram);
		}

	}
}