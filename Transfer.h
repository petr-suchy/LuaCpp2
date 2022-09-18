#pragma once

#include "TransferNumber.h"
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
			
			if (src.curr().isNumber()) {
				TransferNumber(src.curr(), dest);
			}
			else if (src.curr().isTable()) {
				TransferTable(src.curr().getTable(), dest);
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