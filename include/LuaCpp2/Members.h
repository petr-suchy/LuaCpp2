#pragma once

#include "Fields.h"

namespace Lua {

	class Members : public Fields {
	public:

		Members(const std::string& memberList) :
			Fields(memberList)
		{}

	protected:

		virtual std::string _filterKey(const std::string& key) const
		{
			std::string memberName;

			size_t start = key.find_first_not_of(" \t");

			if (start != std::string::npos) {

				size_t dotpos = key.find('.');

				if (dotpos != std::string::npos) {
					start = dotpos + 1;
				}

				size_t end = key.find_last_not_of(" \t") + 1;

				if (end > start) {
					memberName = key.substr(start, end - start);
				}

			}

			return memberName;
		}

	};

}