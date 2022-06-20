#pragma once

#include "Engine.h"
#include "Parameterizable.h"
#include "NumericParam.h"
#include "StringParam.h"
#include "PointerParam.h"
#include "FuncParam.h"
#include "StructParam.h"

namespace Lua {

	typedef Context& Lua;
	typedef Context::CalleeArguments& Args;

	static Function MakeFunc(Parameterizable::Func func)
	{
		return Function(Parameterizable{func});
	}

}