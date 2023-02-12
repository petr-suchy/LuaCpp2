#pragma once

#include "TransferBoolean.h"
#include "TransferInteger.h"
#include "TransferNumber.h"
#include "TransferString.h"
#include "TransferTable.h"

namespace Lua {

	template<typename Source, typename Destination>
	static int Transfer(
		Source& src,
		Destination& dest
	)
	{
		int count = 0;

		while (!src.atEnd()) {
			
			if (src.curr().isBoolean()) {
				TransferBoolean(src.curr(), dest);
			}
			else if (src.curr().isInteger()) {
				TransferInteger(src.curr(), dest);
			}
			else if (src.curr().isNumber()) {
				TransferNumber(src.curr(), dest);
			}
			else if (src.curr().isString()) {
				TransferString(src.curr(), dest);
			}
			else if (src.curr().isTable()) {
				auto tableIt = src.curr().getTableIterator();
				TransferTable(tableIt, dest);
			}
			else {
				throw std::logic_error("invalid type for transfering");
			}

			count++;
			src.next();
		}

		return count;
	}

}