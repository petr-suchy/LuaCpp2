#pragma once

#include "Engine.h"
#include "BasicHostDefs.h"
#include "Parameterizable.h"

namespace Lua {

	typedef Context& Lua;
	typedef Context::CalleeArguments& Args;

	static Function MakeFunc(Parameterizable::Func func)
	{
		return Function(Parameterizable{func});
	}

}