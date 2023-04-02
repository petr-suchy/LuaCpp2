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

						Lua::AuxiliaryContext lua(_output.write.getL());

						lua.args().in() << std::basic_string<Char>{buff, count};
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
						Lua::AuxiliaryContext lua(_output.flush.getL());
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