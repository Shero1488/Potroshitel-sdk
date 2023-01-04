#pragma once

namespace vgui {
	using h_font = unsigned long;
	using vpanel = unsigned int;
}

class i_surface {
public:
	VFUNC(unlock_cursor(), 66, void(__thiscall*)(void*))
	VFUNC(lock_cursor(), 67, void(__thiscall*)(void*))
};