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

		void insertNil()
		{
			_build.push_back(
				[] (Destination& dest)
				{
					dest.insertNil();
				}
			);
		}

		void insertBoolean(bool boolVal)
		{
			_build.push_back(
				[boolVal] (Destination& dest)
				{
					dest.insertBoolean(boolVal);
				}
			);
		}

		void insertInteger(lua_Integer num)
		{
			_build.push_back(
				[num] (Destination& dest)
				{
					dest.insertInteger(num);
				}
			);
		}

		void insertNumber(Library::Number num)
		{
			_build.push_back(
				[num] (Destination& dest)
				{
					dest.insertNumber(num);
				}
			);
		}

		void insertString(const std::string& str)
		{
			_build.push_back(
				[str] (Destination& dest)
				{
					dest.insertString(str);
				}
			);
		}

		void insertTable()
		{
			_build.push_back(
				[] (Destination& dest)
				{
					dest.insertTable();
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

		void transfer(Destination& dest) const
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