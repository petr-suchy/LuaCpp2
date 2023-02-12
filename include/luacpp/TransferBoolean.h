#pragma once

namespace Lua {

	template<typename Item, typename Destination>
	static void TransferBoolean(const Item& item, Destination& dest)
	{
		dest.insertBoolean(item.getBoolean());
	}

}