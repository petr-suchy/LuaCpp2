#pragma once

#include "ReadableValue.h"
#include "ReadableStackSlot.h"
#include "StreamWriter.h"

namespace Lua {

	template<typename OutputStream>
	class ReadableChunk : public ReadableValue {
	public:

		ReadableChunk(OutputStream& os, bool strip = false) :
			_os(os),
			_strip(strip)
		{}

		virtual void getFrom(State& state)
		{
			StreamWriter<OutputStream> writer(_os);
			ReadableStackSlot slot(state);

			slot.prepare(LUA_TFUNCTION);

			state.dumpChunk(
				&StreamWriter<OutputStream>::cb,
				&writer,
				_strip
			);

			slot.finish();
			_os.flush();
		}

	private:

		OutputStream& _os;
		bool _strip;

	};

}