#pragma once

#include "../Engine/IStreamIfce.h"

#include <cstring>

namespace Luaux {
	namespace Host {

		template<typename Char, typename Size>
		class Input {
		public:

				virtual void setfail() = 0;
				virtual void seteof() = 0;

				Input(Engine::InputIfce& input) :
					_input(input),
					_count(0)
				{}

				// extracts characters from stream
				Input& read(Char* buff, Size count)
				{
					_count = 0;

					try {
						
						Lua::AuxContextFromL lua(_input.read.getL());

						lua.args().in() << count;
						lua.pcall(_input.read);

						std::string data;

						if (!lua.args().out().atEnd()) {
							lua.args().out() >> data;
						}

						bool eof = false;

						if (!lua.args().out().atEnd()) {
							lua.args().out() >> eof;
						}

						_count = data.length();
						
						if (_count > count) {
							_count = count;
						}

						memcpy(
							buff,
							data.c_str(),
							(size_t) _count
						);
						
						if (eof || (_count == 0 && count != 0)) {
							seteof();
						}

					}
					catch (...) {
						setfail();
					}

					return *this;
				}

				// returns the number of characters extracted by the last 
				// read operation
				Size gcount() const
				{
					return _count;
				}

		private:

			Engine::InputIfce _input;
			Size _count;

		};

	}
}