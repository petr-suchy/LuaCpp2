#pragma once

namespace Lua {

	template<typename Item, typename Destination>
	static void TransferInteger(const Item& item, Destination& dest)
	{
		dest.insertInteger(item.getInteger());
	}

}