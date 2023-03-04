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

					auto statePtr = _seekableInput.tellg.getWeakStatePtr().lock();
					Lua::AuxiliaryContext lua(statePtr);

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

					auto statePtr = _seekableInput.seekg.getWeakStatePtr().lock();
					Lua::AuxiliaryContext lua(statePtr);

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

					auto statePtr = _seekableInput.seekg.getWeakStatePtr().lock();
					Lua::AuxiliaryContext lua(statePtr);

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