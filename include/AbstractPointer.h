#pragma once

#include "ReadableValue.h"
#include "WritableValue.h"

namespace Lua {

	class AbstractPointer : public ReadableValue, public WritableValue {
	};

}