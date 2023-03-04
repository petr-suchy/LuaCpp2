#pragma once

#include "StreamBaseImpl.h"
#include "IStreamIfce.h"

#include <vector>

namespace Luaux {
	namespace Engine {

		template<typename IStream>
		static void ImplInput(InputIfce* ifce, IStream istream)
		{
			ifce->read = Lua::MakeFunc(
				[istream] (Lua::Args args, Lua::Lua lua) mutable
				{
					size_t count;
					args.in() >> count;

					std::vector<char> buff(count);

					istream->read(
						buff.data(),
						buff.size()
					);

					if (istream->fail() && !istream->eof()) {
						throw std::runtime_error(
							"failed to read data from stream"
						);
					}

					size_t gcount = (size_t) istream->gcount();

					if (gcount < 0 || gcount > (std::numeric_limits<size_t>::max)()) {
						throw std::runtime_error(
							"invalid number of characters returned"
						);
					}

					std::string str(
						buff.data(),
						(size_t) gcount
					);

					if (!str.empty()) {
						args.out() << str << istream->eof();
					}
				}
			);
		}

		template<typename IStream>
		static void ImplSeekableInput(SeekableInputIfce* ifce, IStream istream)
		{
			ifce->tellg = Lua::MakeFunc(
				[istream] (Lua::Args args, Lua::Lua lua) mutable
				{
					args.out() << istream->tellg();
				}
			);
			
			ifce->seekg = Lua::MakeFunc(
				[istream] (Lua::Args args, Lua::Lua lua) mutable
				{
					size_t off;
					SeekOption way = SeekOption::begp;

					args.in() >> off;

					if (!args.in().atEnd()) {
						args.in() >> way;
					}

					switch (way) {
						case SeekOption::begp:
							istream->seekg(off, istream->beg);
						break;

						case SeekOption::curp:
							istream->seekg(off, istream->cur);
						break;

						case SeekOption::endp:
							istream->seekg(off, istream->end);
						break;

						default:
							throw std::logic_error("invalid argument");
					}

				}
			);
		}

		/////////////////////////////////////////////////////

		template< typename IStream>
		static void ImplInputStream(InputStreamIfce* ifce, IStream istream)
		{
			ImplStreamBase(ifce, istream);
			ImplInput(ifce, istream);
		}

		template< typename IStream>
		static void ImplSeekableInputStream(SeekableInputStreamIfce* ifce, IStream istream)
		{
			ImplStreamBase(ifce, istream);
			ImplInput(ifce, istream);
			ImplSeekableStreamBase(ifce, istream);
			ImplSeekableInput(ifce, istream);
		}

		///////////////////////////////////////////////////////

		template<typename IStream>
		class InputStreamImpl : public InputStreamIfce {
		public:

			InputStreamImpl(IStream istream)
			{
				ImplInputStream(this, istream);
			}

		};

		template<typename IStream>
		class SeekableInputStreamImpl : public SeekableInputStreamIfce {
		public:

			SeekableInputStreamImpl(IStream istream)
			{
				ImplSeekableInputStream(this, istream);
			}

		};


		/////////////////////////////////////////////

		template<typename IStream>
		static InputStreamIfce MakeInputStream(IStream istream)
		{
			return InputStreamImpl<IStream>(istream);
		}

		template<typename IStream>
		static SeekableInputStreamIfce MakeSeekableInputStream(IStream istream)
		{
			return SeekableInputStreamImpl<IStream>(istream);
		}

	}
}