#pragma once

#include <string>

namespace Luaux {

	class ModuleNames {
	public:

		ModuleNames(const std::string& names) :
			_names(names),
			_start(0)
		{}

		bool atEnd() const
		{
			return _start == std::string::npos;
		}

		std::string extract()
		{
			if (atEnd()) {
				return "";
			}

			size_t end = _names.find(',', _start);
			std::string name;

			if (end != std::string::npos) {
				name = TrimName(_names.substr(_start, end - _start));
				_start = end + 1;
			}
			else {
				name = TrimName(_names.substr(_start, _names.length()));
				_start = std::string::npos;
			}

			return name;
		}

	private:

		std::string _names;
		size_t _start;

		static std::string TrimName(const std::string str)
		{
			std::string result;
			size_t start = str.find_first_not_of(" \t");

			if (start != std::string::npos) {

				size_t end = str.find_last_not_of(" \t") + 1;

				if (end > start) {
					result = str.substr(start, end - start);
				}

			}

			return result;
		}

	};

}