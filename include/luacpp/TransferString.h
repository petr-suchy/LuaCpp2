#pragma once

namespace Lua {

	template<typename Item, typename Destination>
	static void TransferString(const Item& item, Destination& dest)
	{
		dest.insertString(item.getString());
	}

}