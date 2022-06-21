#pragma once

#include "ReadableValue.h"
#include "WritableValue.h"

namespace Lua {

	class AbstractFunction : public ReadableValue, public WritableValue {
	};

}