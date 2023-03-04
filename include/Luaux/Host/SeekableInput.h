#pragma once

#include "../Engine/IStreamIfce.h"

namespace Luaux {
	namespace Host {

		template<typename Char, typename Size, typename Pos>
		class SeekableInput {
		public:

			SeekableInput(Engine::SeekableInputIfce& seekableInput) :
				_seekableInput(seekableInput)
			{}

			// returns the input position indicator
			Pos tellg()
			{
				Pos pos = -1;

				try {

					Lua::AuxContextFromL lua(_seekableInput.tellg.getL());

					lua.pcall(_seekableInput.tellg);
					lua.args().out() >> pos;

				}
				catch (...) {
					// nothing to do here
				}

				return pos;
			}
			
			// sets the input position indicator
			SeekableInput& seekg(Pos pos)
			{

				try {

					Lua::AuxContextFromL lua(_seekableInput.seekg.getL());

					lua.args().in() << pos;
					lua.pcall(_seekableInput.seekg);

				}
				catch (...) {
					// nothing to do here
				}

				return *this;
			}

			SeekableInput& seekg(Pos off, Engine::SeekOption way)
			{

				try {

					Lua::AuxContextFromL lua(_seekableInput.seekg.getL());

					lua.args().in() << off << way;
					lua.pcall(_seekableInput.seekg);

				}
				catch (...) {
					// nothing to do here
				}

				return *this;
			}

		private:

			Engine::SeekableInputIfce _seekableInput;

		};

	}
}