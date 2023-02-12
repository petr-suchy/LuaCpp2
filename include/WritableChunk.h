#pragma once

#include <string>

#include "WritableValue.h"
#include "WritableStackSlot.h"
#include "StreamReader.h"

namespace Lua {

	template<typename InputStream>
	class WritableChunk : public WritableValue {
	public:

		WritableChunk(
			InputStream& is,
			const std::string& chunkName = "",
			const std::string& mode = "bt"
		) :
			_is(is),
			_chunkName(chunkName),
			_mode(mode)
		{}

		virtual void insertTo(State& state) const
		{
			StreamReader<InputStream> reader(_is);
			WritableStackSlot slot(state);
			
			slot.prepare();

			state.loadChunk(
				&StreamReader<InputStream>::cb,
				&reader,
				_chunkName,
				_mode
			);

			slot.finish();
		}

	private:

		InputStream& _is;
		std::string _chunkName;
		std::string _mode;

	};

}