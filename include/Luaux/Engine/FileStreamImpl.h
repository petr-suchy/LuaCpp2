#pragma once

#include "IOStreamImpl.h"
#include "FileStreamIfce.h"

namespace Luaux {
	namespace Engine {

		template<typename FStream>
		static void ImplFileStreamBase(FileStreamBaseIfce* ifce, FStream fstream)
		{
			ifce->__close = Lua::MakeFunc(
				[fstream] (Lua::Args args, Lua::Lua lua) mutable
				{
					fstream->close();
				}
			);

			ifce->close = Lua::MakeFunc(
				[fstream] (Lua::Args args, Lua::Lua lua) mutable
				{
					fstream->close();
				}
			);
		}

		///////////////////////////////////////////////////////

		template<typename IFStream>
		class InputFileStreamImpl : public InputFileStreamIfce {
		public:

			InputFileStreamImpl(IFStream ifstream)
			{
				ImplSeekableInputStream(this, ifstream);
				ImplFileStreamBase(this, ifstream);
			}

		};

		template<typename OFStream>
		class OutputFileStreamImpl : public OutputFileStreamIfce {
		public:

			OutputFileStreamImpl(OFStream ofstream)
			{
				ImplSeekableOutputStream(this, ofstream);
				ImplFileStreamBase(this, ofstream);
			}

		};

		template<typename FStream>
		class FileStreamImpl : public FileStreamIfce {
		public:

			FileStreamImpl(FStream fstream)
			{
				ImplSeekableStream(this, fstream);
				ImplFileStreamBase(this, fstream);
			}

		};

		///////////////////////////////////////////////////////

		template<typename IFStream>
		static InputFileStreamIfce MakeInputFileStream(IFStream ifstream)
		{
			return InputFileStreamImpl<IFStream>(ifstream);
		}

		template<typename OFStream>
		static OutputFileStreamIfce MakeOutputFileStream(OFStream ofstream)
		{
			return OutputFileStreamImpl<OFStream>(ofstream);
		}

		template<typename FStream>
		static FileStreamIfce MakeFileStream(FStream fstream)
		{
			return FileStreamImpl<FStream>(fstream);
		}

	}
}