#include "../globals.h"

std::string c_base_combat_weapon::get_name( ) {
	const auto weapon_data = get_cs_weapon_data( );
	if ( !weapon_data )
		return "";

	return memory::WideToMultiByte( interfaces::m_localize->find_safe( weapon_data->m_hud_name ) );
}

c_cs_weapon_data* c_base_combat_weapon::get_cs_weapon_data() { return interfaces::m_weapon_system->get_cs_weapon_data(get_item_definition_index()); }

player_info_t c_cs_player::get_info() {
	auto ret = player_info_t();

	interfaces::m_engine->get_player_info(get_index(), &ret);

	return ret;
}

bool c_cs_player::is_enemy(c_cs_player* from) {
	if (this == from)
		return false;

	if (interfaces::m_game_types->get_cur_game_type() == GAME_TYPE_FREEFORALL)
		return get_survival_team() != from->get_survival_team();

	static const auto mp_teammates_are_enemies = interfaces::m_cvar_system->find_var(HASH("mp_teammates_are_enemies"));
	if (mp_teammates_are_enemies->get_bool())
		return true;

	return get_team() != from->get_team();
}

int c_base_animating::get_sequence_activity(int sequence) {
	const auto model = get_model();
	if (!model)
		return -1;

	const auto hdr = interfaces::m_model_info->get_studio_model(model);
	if (!hdr)
		return -1;

	static const auto get_sequence_activity_fn = SIG("client.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83").cast<int(__fastcall*)(void*, studiohdr_t*, int)>();

	return get_sequence_activity_fn(this, hdr, sequence);
}

c_base_combat_weapon* c_base_combat_character::get_active_weapon() {
	const auto handle = get_active_weapon_handle();
	if (!handle.is_valid())
		return nullptr;

	return reinterpret_cast<c_base_combat_weapon*>(handle.get());
}