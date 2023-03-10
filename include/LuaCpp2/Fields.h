#pragma once

#include "Field.h"

namespace Lua {
	
	class Fields : public StackModifier {
	public:

		Fields(const std::string& keyList) :
			_keyList(keyList)
		{}

		virtual void modify(State& state) const
		{
			explodeList(state, _keyList);
		}

	protected:

		virtual std::string _filterKey(const std::string& key) const
		{
			return key;
		}

	private:

		void explodeList(
			State& state,
			const std::string& keyList
		) const
		{
			size_t start = keyList.length();
			size_t end = start;

			while (
				start != 0 &&
				(start = keyList.rfind(',', start - 1)) != std::string::npos
			) {

				std::string key = _filterKey(
					keyList.substr(start + 1, end - start - 1)
				);

				if (!key.empty()) {
					state << Lua::Field(key);
				}

				end = start;

			}

			std::string key = _filterKey(
				keyList.substr(0, end)
			);

			if (!key.empty()) {
				state << Lua::Field(key);
			}

		}

		std::string _keyList;

	};
	
}