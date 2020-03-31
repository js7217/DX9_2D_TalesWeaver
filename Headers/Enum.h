#pragma once

enum WIN_MODE { MODE_FULL, MODE_WIN };

namespace GroupID
{
	enum ID
	{
		BACKGROUND,
		GAMEOBJECT,
		EFFECT,
		ICON,
		UI,
		SHOP,
		MOUSE,
		END
	};
}

enum ITEM_TYPE { WEAPON, HP, MP };
enum ITEM_LOCATION {EQUIP, BUYSHOP, SELLSHOP, READY, INVEN};