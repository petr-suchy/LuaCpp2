#pragma once

#include <functional>
#include <vector>

#include "State.h"

namespace Lua {

	template<typename Destination>
	class OutputCopy {
	public:
		
		typedef std::function<
			void(Destination&)
		> BuildInstruction;
		
		OutputCopy()
		{}

		void createTable()
		{
			_build.push_back(
				[] (Destination& dest)
				{
					dest.createTable();
				}
			);
		}

		void pushNil()
		{
			_build.push_back(
				[] (Destination& dest)
				{
					dest.pushNil();
				}
			);
		}

		void pushNumber(lua_Number num)
		{
			_build.push_back(
				[num] (Destination& dest)
				{
					dest.pushNumber(num);
				}
			);
		}

		void setTable()
		{
			_build.push_back(
				[] (Destination& dest)
				{
					dest.setTable();
				}
			);
		}

		void transfer(Destination& dest)
		{
			for (auto& inst : _build) {
				inst(dest);
			}
		}

		void clear()
		{
			_build.clear();
		}

	private:

		std::vector<BuildInstruction> _build;

	};

}