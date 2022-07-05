#pragma once

#include "BooleanParam.h"
#include "NumericParam.h"
#include "StringParam.h"
#include "PointerParam.h"
#include "FuncParam.h"
#include "StructParam.h"

#include <codecvt>

defhost_number(float)
defhost_number(double)

defhost_signed(char)
defhost_signed(short)
defhost_signed(int)
defhost_signed(long)
defhost_signed(long long)

defhost_unsigned(char)
defhost_unsigned(short)
defhost_unsigned(int)
defhost_unsigned(long)
defhost_unsigned(long long)

defhost_wstring(
	std::wstring,
	std::wstring_convert<
		std::codecvt_utf8<wchar_t>
	>
)

defhost_string(std::string)