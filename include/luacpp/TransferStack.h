#pragma once

#include "Transfer.h"
#include "InputStack.h"
#include "OutputStack.h"

namespace Lua {

	// Copies the given number of items from the source stack to the destination
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

		InputStack istack(src, startIndex, count);

		int destCount = Transfer(
			istack,
			dest
		);

		if (destCount < count) {

			int padding = count - destCount;

			for (int i = 0; i < padding; i++) {
				dest.insertNil();
			}

		}

		return count;
	}

	// Copies the given number of items from the source stack to the destination stack
	static int Transfer(
		State& src,
		State& dest,
		int startIndex = -1,
		int count = 1
	)
	{
		OutputStack ostack(dest);

		int transfered = Transfer(
			src,
			ostack,
			startIndex,
			count
		);

		ostack.finish();

		return transfered;
	}

}