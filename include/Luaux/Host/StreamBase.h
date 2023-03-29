#pragma once

#include "../Engine/StreamBaseIfce.h"

namespace Luaux {
	namespace Host {

		class StreamBase {
		public:

			StreamBase(Engine::StreamBaseIfce& base) :
				_base(base),
				_eof(false),
				_fail(false)
			{}

			bool eof() { return _eof; }
			bool fail() { return _fail; }
			bool good() { return !_eof && !_fail; }

			void clear()
			{
				_eof = false;
				_fail = false;

				try {
					Lua::AuxiliaryContext lua(_base.clear.getL());

					lua.pcall(_base.clear);
				}
				catch (...) {

				}
			}

		protected:

			bool _eof;
			bool _fail;

		private:

			Engine::StreamBaseIfce _base;

		};

		class SeekableStreamBase {
		public:

			Engine::SeekOption beg;
			Engine::SeekOption cur;
			Engine::SeekOption end;

			SeekableStreamBase(Engine::SeekableStreamBaseIfce& seekableBase) :
				beg(seekableBase.begp),
				cur(seekableBase.curp),
				end(seekableBase.endp)
			{}

		};

	}
}