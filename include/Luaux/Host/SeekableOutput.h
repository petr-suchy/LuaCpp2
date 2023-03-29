#pragma once

#include "../Engine/OStreamIfce.h"

namespace Luaux {
	namespace Host {

		template<typename Char, typename Size, typename Pos>
		class SeekableOutput {
		public:

			SeekableOutput(Engine::SeekableOutputIfce& seekableOutput) :
				_seekableOutput(seekableOutput)
			{}

			// returns the output position indicator
			Pos tellp()
			{
				Pos pos = -1;

				try {

					Lua::AuxiliaryContext lua(_seekableOutput.tellp.getL());

					lua.pcall(_seekableOutput.tellp);
					lua.args().out() >> pos;

				}
				catch (...) {
					// nothing to do here
				}

				return pos;
			}
			
			// sets the output position indicator
			SeekableOutput& seekp(Pos pos)
			{

				try {

					Lua::AuxiliaryContext lua(_seekableOutput.seekp.getL());

					lua.args().in() << pos;
					lua.pcall(_seekableOutput.seekp);

				}
				catch (...) {
					// nothing to do here
				}

				return *this;
			}

			SeekableOutput& seekp(Pos off, Engine::SeekOption way)
			{

				try {

					Lua::AuxiliaryContext lua(_seekableOutput.seekp.getL());

					lua.args().in() << off << way;
					lua.pcall(_seekableOutput.seekp);

				}
				catch (...) {
					// nothing to do here
				}

				return *this;
			}

		private:

			Engine::SeekableOutputIfce _seekableOutput;

		};

	}
}