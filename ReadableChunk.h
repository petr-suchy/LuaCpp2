#pragma once

#include "ReadableValue.h"
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

			state.prepareReading(LUA_TFUNCTION);

			state.dumpChunk(
				&StreamWriter<OutputStream>::cb,
				&writer,
				_strip
			);

			_os.flush();

			state.finishReading();
		}

	private:

		OutputStream& _os;
		bool _strip;

	};

}