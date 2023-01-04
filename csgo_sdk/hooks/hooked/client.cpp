#include "../hooks.h"

__declspec (naked) void __stdcall hooks::client::create_move::proxy(int sequence_number, float input_sample_frame_time, bool active) {
	__asm {
		push ebp
		mov ebp, esp

		push ebx
		push esp
		push dword ptr[ebp + 16]
		push dword ptr[ebp + 12]
		push dword ptr[ebp + 8]

		call hook

		pop ebx

		pop ebp
		retn 12
	}
}

void __stdcall hooks::client::create_move::hook(int sequence_number, float input_sample_frame_time, bool active, bool& send_packet) {
	static const auto original = m_client->get_original< fn >( client::create_move::index );

	// let original run first.
	original( interfaces::m_client_dll, sequence_number, input_sample_frame_time, active );

	/* add this shit in frame_stage_notify too */
	/* but i very lazy for hooking frame_stage_notify */
	local_player->m_local = reinterpret_cast<c_cs_player*>(interfaces::m_entity_list->get_client_entity(interfaces::m_engine->get_local_player()));

	const auto cmd = interfaces::m_input->get_user_cmd(sequence_number);
	const auto verified = interfaces::m_input->get_verified_user_cmd(sequence_number);

	if ( !interfaces::m_engine->is_in_game( ) || !cmd || !cmd->m_command_number || !local_player->self( ) || !verified )
		return original( interfaces::m_client_dll, sequence_number, input_sample_frame_time, active );

	// save previous view angles for movement correction
	const auto old_angles = cmd->m_view_angles;

	// @note: need do bunnyhop and other movements before prediction
	movement->handle(cmd);

	/*
	 * CL_RunPrediction
	 * correct prediction when framerate is lower than tickrate
	 * https://github.com/VSES/SourceEngine2007/blob/master/se2007/engine/cl_pred.cpp#L41
	 */
	local_player->update();

	// start engine prediction
	local_player->start(cmd);
	{
		




	}
	local_player->end();

	if ( local_player->self( )->is_alive( ) )
		movement->correct( cmd, old_angles );

	// clamp & normalize view angles
	if (cfg->bMiscAntiUntrusted)
	{
		cmd->m_view_angles.sanitize();
		cmd->m_view_angles.clamp();
	}

	// store next tick view angles state
	//G::angRealView = cmd->m_view_angles;

	// store current tick send packet state
	//G::bSendPacket = send_packet;

	// @note: i seen many times this mistake and please do not set/clamp angles here cuz u get confused with psilent aimbot later!

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->get_check_sum();
}

//void __fastcall hooks::client::level_init_post_entity::hook( void* ecx, void* edx ) {
//	static const auto original = m_client->get_original< decltype(&hook) >( client::level_init_post_entity::index );
//
//	// store class ids.
//	client_class_t* list;
//
//	// clear old shit.
//	m_client_ids.clear();
//
//	// sanity check on client.
//	if (!interfaces::m_client_dll)
//		return;
//
//	// grab linked list.
//	list = interfaces::m_client_dll->get_all_classes();
//	if (!list)
//		return;
//
//	// iterate list.
//	for (; list != nullptr; list = list->m_next)
//		m_client_ids[HASH_RT(list->m_network_name)] = list->m_class_id;
//	
//
//	// invoke original method.
//	original( ecx, edx );
//
//}

