#pragma once

namespace Lua {

	template<typename Item, typename Destination>
	static void TransferNumber(const Item& item, Destination& dest)
	{
		dest.pushNumber(item.getNumber());
	}

}