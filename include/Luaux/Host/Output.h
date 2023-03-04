#pragma once

#include "../Engine/OStreamIfce.h"

namespace Luaux {
	namespace Host {

		template<typename Char, typename Size>
		class Output {
		public:

				virtual void setfail() = 0;

				Output(Engine::OutputIfce& output) :
					_output(output)
				{}

				// insert characters to stream
				Output& write(const Char* buff, Size count)
				{

					try {

						auto statePtr = _output.write.getWeakStatePtr().lock();
						Lua::AuxiliaryContext lua(statePtr);

						lua.args().in() << buff;
						lua.pcall(_output.write);

					}
					catch (...) {
						setfail();
					}

					return *this;
				}

				// synchronizes with the underlying storage device
				Output& flush()
				{
					try {
						auto statePtr = _output.flush.getWeakStatePtr().lock();
						Lua::AuxiliaryContext lua(statePtr);
					}
					catch (...) {
						// nothing to do here
					}

					return *this;
				}

		private:

			Engine::OutputIfce _output;

		};

	}
}