#pragma once

#include <string>

#include "WritableValue.h"
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

		virtual void insertTo(State& state)
		{
			StreamReader<InputStream> reader(_is);

			state.prepareWriting();

			state.loadChunk(
				&StreamReader<InputStream>::cb,
				&reader,
				_chunkName,
				_mode
			);

			state.finishWriting();
		}

	private:

		InputStream& _is;
		std::string _chunkName;
		std::string _mode;

	};

}