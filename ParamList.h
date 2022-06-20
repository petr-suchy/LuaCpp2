#pragma once

#include <string>
#include <functional>

namespace Lua {

	class ParamList {
	public:

		typedef std::function<void(const std::string)> FuncPrototype;

		ParamList(const std::string strList) :
			_strList(strList)
		{}

		void readForward(FuncPrototype func) const
		{
			size_t start = 0;
			size_t end = std::string::npos;

			while ((end = _strList.find(',', start)) != std::string::npos) {

				func(
					trimParamName(_strList.substr(start, end - start))
				);

				start = end + 1;
			}

			func(
				trimParamName(_strList.substr(start, _strList.length()))
			);
		}

		void readBackward(FuncPrototype func) const
		{
			size_t start = std::string::npos;
			size_t end = _strList.length();

			while (end != 0 && (start = _strList.rfind(',', end - 1)) != std::string::npos) {

				func(
					trimParamName(_strList.substr(start + 1, end - start - 1))
				);

				end = start;
			}

			func(
				trimParamName(_strList.substr(0, end))
			);
		}

	private:

		std::string _strList;

		static std::string trimParamName(const std::string name)
		{
			std::string result;
			size_t start = name.find_first_not_of(" \t");

			if (start != std::string::npos) {

				size_t end = name.find_last_not_of(" \t") + 1;

				if (end > start) {
					result = name.substr(start, end - start);
				}

			}

			return result;
		}

	};

}