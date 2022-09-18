#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"
#include "Arguments.h"
#include "ParamList.h"

namespace Lua {

	class GlobalVars : public Arguments<WritableParams, ReadableParams> {
	public:

		GlobalVars(State& state, const ParamList& strNameList) :
			Arguments(
				WritableParams(
					state,
					// gets count
					[this] (Lua::State&)
					{
						return _nameList.size();
					},
					// prepares writing
					[] (Lua::State&)
					{},
					// finishes writing
					[this] (Lua::State& state)
					{
						// TODO: check _nameList.front().size() > 0
						state.setGlobal(_nameList.front());
						_nameList.pop_front();
					}
				),
				ReadableParams(
					state,
					// gets count
					[this] (Lua::State&)
					{
						return _nameList.size();
					},
					// prepares reading
					[this] (Lua::State& state)
					{
						// TODO: check _nameList.front().size() > 0
						state.pushGlobal(_nameList.front());
						_nameList.pop_front();
					},
					// finishes reading
					[] (Lua::State& state)
					{}
				)
			)
		{
			strNameList.readForward(
				[this] (const std::string part)
				{
					_nameList.push_back(part);
				}
			);
		}

	private:

		std::deque<std::string> _nameList;

	};

}