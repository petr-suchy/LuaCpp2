#pragma once

#include "Transfer.h"
#include "InputStack.h"
#include "OutputStack.h"

namespace Lua {

	template<typename Destination>
	static int Transfer(
		State& src,
		Destination& dest,
		int startIndex = -1,
		int count = 1
	)
	{
		if (startIndex < 0) {

			// convert relative index to absolute index
			startIndex = (src.getStackTop() + 1) + startIndex;

			if (startIndex < 1) {
				throw std::logic_error("invalid index");
			}

		}

		int destCount = Transfer(
			InputStack{src, startIndex, count},
			dest
		);

		if (destCount < count) {

			int padding = count - destCount;

			for (int i = 0; i < padding; i++) {
				dest.pushNil();
			}

		}

		return count;
	}

	static int Transfer(
		State& src,
		State& dest,
		int startIndex = -1,
		int count = 1
	)
	{
		return Transfer(
			src,
			OutputStack{dest},
			startIndex,
			count
		);
	}

}