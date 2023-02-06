#pragma once

#include <stdexcept>

#include "TransferNumber.h"

namespace Lua {

	template<typename Iterator, typename Destination>
	static void TransferTable(
		Iterator& tbl,
		Destination& dest
	)
	{
		tbl.first();
		dest.insertTable();

		while (!tbl.atEnd()) {

			// set a table field key

			if (tbl.key().isBoolean()) {
				TransferBoolean(tbl.key(), dest);
			}
			else if (tbl.key().isInteger()) {
				TransferInteger(tbl.key(), dest);
			}
			else if (tbl.key().isNumber()) {
				TransferNumber(tbl.key(), dest);
			}
			else if (tbl.key().isString()) {
				TransferString(tbl.key(), dest);
			}
			else {
				throw std::logic_error("invalid key type");
			}

			// set a table field value

			if (tbl.val().isBoolean()) {
				TransferBoolean(tbl.val(), dest);
			}
			else if (tbl.val().isInteger()) {
				TransferInteger(tbl.val(), dest);
			}
			else if (tbl.val().isNumber()) {
				TransferNumber(tbl.val(), dest);
			}
			else if (tbl.val().isString()) {
				TransferString(tbl.val(), dest);
			}
			else if (tbl.val().isTable()) {
				TransferTable(tbl.val().getTable(), dest);
			}
			else {

				throw std::logic_error(
					"invalid value type"
				);

			}

			dest.setTable();
			tbl.next();
		}

	}

}