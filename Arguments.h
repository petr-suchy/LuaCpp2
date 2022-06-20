#pragma once

namespace Lua {

	template<typename InputType, typename OutputType>
	class Arguments {
	public:

		Arguments(
			InputType& in,
			OutputType& out
		) :
			_in(in),
			_out(out)
		{}

		InputType &in() { return _in; }
		OutputType &out() { return _out; }

	private:

		InputType _in;
		OutputType _out;

	};

}