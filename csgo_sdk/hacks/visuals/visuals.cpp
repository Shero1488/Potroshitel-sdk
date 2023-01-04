#include "visuals.h"

Visuals g_visuals{ };

void Visuals::ModulateWorld( ) {
	std::vector< i_material* > world, props;

	// iterate material handles.
	for( uint16_t h{ interfaces::m_material_system->first_material( ) }; h != interfaces::m_material_system->invalid_material( ); h = interfaces::m_material_system->next_material( h ) ) {
		// get material from handle.
		i_material* mat = interfaces::m_material_system->get_material( h );
		if( !mat )
			continue;

		// store world materials.
		if( strstr( mat->get_texture_group_name( ), "World textures" ) )
			world.push_back( mat );

		// store props.
		else if( strstr ( mat->get_texture_group_name( ), "StaticProp textures" ) )
			props.push_back( mat );
	}

	// night.
	if( cfg->m_visuals.bNightMode ) {
		for( const auto& w : world )
			w->color_modulate( 0.17f, 0.16f, 0.18f );

		// IsUsingStaticPropDebugModes my nigga
		if( interfaces::m_cvar_system->find_var( HASH( "r_DrawSpecificStaticProp" ) )->get_int( ) != 0 ) {
			interfaces::m_cvar_system->find_var( HASH( "r_DrawSpecificStaticProp" ) )->set_value( 0 );
		}

		for( const auto& p : props )
			p->color_modulate( 0.5f, 0.5f, 0.5f );
	}

	// disable night.
	else {
		for( const auto& w : world )
			w->color_modulate( 1.f, 1.f, 1.f );

		// restore r_DrawSpecificStaticProp.
		if( interfaces::m_cvar_system->find_var( HASH( "r_DrawSpecificStaticProp" ) )->get_int( ) != -1 ) {
			interfaces::m_cvar_system->find_var( HASH( "r_DrawSpecificStaticProp" ) )->set_value( -1 );
		}

		for( const auto& p : props )
			p->color_modulate( 1.f, 1.f, 1.f );
	}

	// transparent props.
	if( cfg->m_visuals.bTransparentProps ) {

		// IsUsingStaticPropDebugModes my nigga
		if( interfaces::m_cvar_system->find_var( HASH( "r_DrawSpecificStaticProp" ) )->get_int( ) != 0 ) {
			interfaces::m_cvar_system->find_var( HASH( "r_DrawSpecificStaticProp" ) )->set_value( 0 );
		}

		for( const auto& p : props )
			p->alpha_modulate( 0.85f );
	}

	// disable transparent props.
	else {

		// restore r_DrawSpecificStaticProp.
		if( interfaces::m_cvar_system->find_var( HASH( "r_DrawSpecificStaticProp" ) )->get_int( ) != -1 ) {
			interfaces::m_cvar_system->find_var( HASH( "r_DrawSpecificStaticProp" ) )->set_value( -1 );
		}

		for( const auto& p : props )
			p->alpha_modulate( 1.0f );
	}
}

//void Visuals::ThirdpersonThink( ) {
//	ang_t                          offset;
//	vec3_t                         origin, forward;
//	static CTraceFilterSimple_game filter{ };
//	c_game_trace                   tr;
//
//	// for whatever reason overrideview also gets called from the main menu.
//	if( !interfaces::m_engine->is_in_game( ) )
//		return;
//
//	// check if we have a local player and he is alive.
//	bool alive = local_player->m_local && local_player->m_local->is_alive( );
//
//	// camera should be in thirdperson.
//	if( m_thirdperson ) {
//
//		// if alive and not in thirdperson already switch to thirdperson.
//		if( alive && !interfaces::m_input->CAM_IsThirdPerson( ) )
//			interfaces::m_input->CAM_ToThirdPerson( );
//
//		// if dead and spectating in firstperson switch to thirdperson.
//		else if( g_cl.m_local->m_iObserverMode( ) == 4 ) {
//
//			// if in thirdperson, switch to firstperson.
//			// we need to disable thirdperson to spectate properly.
//			if( interfaces::m_input->CAM_IsThirdPerson( ) ) {
//				interfaces::m_input->CAM_ToFirstPerson( );
//				interfaces::m_input->m_camera_offset.z = 0.f;
//			}
//
//			g_cl.m_local->m_iObserverMode( ) = 5;
//		}
//	}
//
//	// camera should be in firstperson.
//	else if( interfaces::m_input->CAM_IsThirdPerson( ) ) {
//		interfaces::m_input->CAM_ToFirstPerson( );
//		interfaces::m_input->m_camera_offset.z = 0.f;
//	}
//
//	// if after all of this we are still in thirdperson.
//	if( interfaces::m_input->CAM_IsThirdPerson( ) ) {
//		// get camera angles.
//		interfaces::m_engine->get_view_angles( offset );
//
//		// get our viewangle's forward directional vector.
//		math::AngleVectors( offset, &forward );
//
//		// cam_idealdist convar.
//		offset.z = 150.f;
//
//		// start pos.
//		origin = g_cl.m_shoot_pos;
//
//		// setup trace filter and trace.
//		filter.SetPassEntity( g_cl.m_local );
//
//		interfaces::m_trace_system->trace_ray(
//			ray_t( origin, origin - ( forward * offset.z ), { -16.f, -16.f, -16.f }, { 16.f, 16.f, 16.f } ),
//			MASK_NPCWORLDSTATIC,
//			( i_trace_filter* ) &filter,
//			&tr
//		);
//
//		// adapt distance to travel time.
//		math::clamp( tr.m_fraction, 0.f, 1.f );
//		offset.z *= tr.m_fraction;
//
//		// override camera angles.
//		interfaces::m_input->m_camera_offset = { offset.x, offset.y, offset.z };
//	}
//}

void Visuals::Hitmarker( ) { // todo: need call that func
	if( !cfg->m_visuals.bHitMarker )
		return;

	if( interfaces::m_global_vars->m_cur_time > m_hit_end ) // todo: need check m_hit_end and m_hit_start and m_hit_duration
		return;

	if( m_hit_duration <= 0.f )
		return;

	float complete = ( interfaces::m_global_vars->m_cur_time - m_hit_start ) / m_hit_duration;
	int x = globals::m_width / 2,
		y = globals::m_height / 2,
		alpha = ( 1.f - complete ) * 240;

	constexpr int line{ 6 };

	render::line( x - line, y - line, x - ( line / 4 ), y - ( line / 4 ), { 200, 200, 200, alpha }, 1 ); // todo: check thickness
	render::line( x - line, y + line, x - ( line / 4 ), y + ( line / 4 ), { 200, 200, 200, alpha }, 1 );
	render::line( x + line, y + line, x + ( line / 4 ), y + ( line / 4 ), { 200, 200, 200, alpha }, 1 );
	render::line( x + line, y - line, x + ( line / 4 ), y - ( line / 4 ), { 200, 200, 200, alpha }, 1 );
}

void Visuals::NoSmoke( ) { // todo: check call and hook FrameStageNotify
	if( !smoke1 )
		smoke1 = interfaces::m_material_system->find_material( ( "particle/vistasmokev1/vistasmokev1_fire" ), ( "Other textures" ) );

	if( !smoke2 )
		smoke2 = interfaces::m_material_system->find_material( ( "particle/vistasmokev1/vistasmokev1_smokegrenade" ), ( "Other textures" ) );

	if( !smoke3 )
		smoke3 = interfaces::m_material_system->find_material( ( "particle/vistasmokev1/vistasmokev1_emods" ), ( "Other textures" ) );

	if( !smoke4 )
		smoke4 = interfaces::m_material_system->find_material( ( "particle/vistasmokev1/vistasmokev1_emods_impactdust" ), ( "Other textures" ) );

	if( cfg->m_visuals.bRemoveSmoke ) {
		if( !smoke1->get_flag( MATERIAL_FLAG_NO_DRAW ) )
			smoke1->set_flag( MATERIAL_FLAG_NO_DRAW, true );

		if( !smoke2->get_flag( MATERIAL_FLAG_NO_DRAW ) )
			smoke2->set_flag( MATERIAL_FLAG_NO_DRAW, true );

		if( !smoke3->get_flag( MATERIAL_FLAG_NO_DRAW ) )
			smoke3->set_flag( MATERIAL_FLAG_NO_DRAW, true );

		if( !smoke4->get_flag( MATERIAL_FLAG_NO_DRAW ) )
			smoke4->set_flag( MATERIAL_FLAG_NO_DRAW, true );
	}

	else {
		if( smoke1->get_flag( MATERIAL_FLAG_NO_DRAW ) )
			smoke1->set_flag( MATERIAL_FLAG_NO_DRAW, false );

		if( smoke2->get_flag( MATERIAL_FLAG_NO_DRAW ) )
			smoke2->set_flag( MATERIAL_FLAG_NO_DRAW, false );

		if( smoke3->get_flag( MATERIAL_FLAG_NO_DRAW ) )
			smoke3->set_flag( MATERIAL_FLAG_NO_DRAW, false );

		if( smoke4->get_flag( MATERIAL_FLAG_NO_DRAW ) )
			smoke4->set_flag( MATERIAL_FLAG_NO_DRAW, false );
	}
}

void Visuals::think( ) {
	// don't run anything if our local player isn't valid.
	if ( !local_player->self( ) || !interfaces::m_engine->is_in_game( ) )
		return;

	if (cfg->m_visuals.bDisableScope
		&& local_player->self()->is_alive( )
		&& local_player->self()->get_active_weapon( )
		&& local_player->self()->get_active_weapon( )->get_cs_weapon_data( )->m_weapon_type == WEAPON_TYPE_SNIPER
		&& local_player->self()->is_scoped( ) ) {

		// rebuild the original scope lines.
		int w = globals::m_width,
			h = globals::m_height,
			x = w / 2,
			y = h / 2,
			size = interfaces::m_cvar_system->find_var( HASH( "cl_crosshair_sniper_width" ) )->get_int( );
		
		// Here We Use The Euclidean distance To Get The Polar-Rectangular Conversion Formula.
		if ( size > 1 ) {
			x -= ( size / 2 );
			y -= ( size / 2 );
		}

		// draw our lines.
		render::rect_filled( 0, y, w, size, col_t::palette_t::black( ) );
		render::rect_filled( x, 0, size, h, col_t::palette_t::black( ) );
	}


	std::vector< std::pair< c_base_entity*, float > > vecOrder = { };

	for ( size_t i = 1; i < interfaces::m_entity_list->get_max_entities( ); i++ )
	{
		c_base_entity* pEntity = static_cast< c_base_entity* >( interfaces::m_entity_list->get_client_entity( i ) );

		if ( pEntity == nullptr || pEntity->is_dormant( ) )
			continue;

		// save entities and calculated distance for sort
		vecOrder.emplace_back( std::make_pair( pEntity, ( pEntity->get_render_origin( ) - local_player->m_local->get_origin( ) /*G::vecCamera*/ ).length( ) ) ); //todo: check qo0 and add G::vecCamera
	}

	// sort entities by distance to make closest entity drawn last to make it easy readable and look nicer
	std::sort( vecOrder.begin( ), vecOrder.end( ), [ ]( const auto& a, const auto& b )
	{
			return a.second > b.second;
	});

	for ( const auto& [ pEntity, flDistance ] : vecOrder )
	{
		client_class_t* pClientClass = pEntity->get_client_class( );

		if (pClientClass == nullptr)
			continue;

		const e_class_id nIndex = pClientClass->m_class_id;

		switch ( nIndex )
		{
			case C_ENV_TONEMAP_CONTROLLER: {
				ModulateWorld( );
				break;
			}

			case C_CS_PLAYER: {

				c_cs_player* player = pEntity->as< c_cs_player* >();

				// dont draw dead players.
				if ( !player->is_alive( ) )
					return;

				// check on local player
				if ( player == local_player->m_local )
					return;

				// draw player esp.
				DrawPlayer( player );
				break;
			}

			case C_BASE_CS_GRENADE_PROJECTILE:
			case C_DECOY_PROJECTILE:
			case C_SMOKE_GRENADE_PROJECTILE:
			case C_MOLOTOV_PROJECTILE:
			case C_INFERNO:
			{
				DrawProjectile( pEntity, nIndex );
				break;
			}



		}

	}

	// draw everything else.
	/*SpreadCrosshair();
	StatusIndicators();
	Spectators();
	PenetrationCrosshair();
	Hitmarker();
	DrawPlantedC4();*/
}

void Visuals::Spectators( ) {
	/*if( !g_menu.main.visuals.spectators.get( ) )
		return;

	std::vector< std::string > spectators{ XOR( "spectators" ) };
	int h = render::menu_shade.m_size.m_height;

	for( int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i ) {
		Player* player = g_csgo.m_entlist->GetClientEntity< Player* >( i );
		if( !player )
			continue;

		if( player->m_bIsLocalPlayer( ) )
			continue;

		if( player->dormant( ) )
			continue;

		if( player->m_lifeState( ) == LIFE_ALIVE )
			continue;

		if( player->GetObserverTarget( ) != g_cl.m_local )
			continue;

		player_info_t info;
		if( !g_csgo.m_engine->GetPlayerInfo( i, &info ) )
			continue;

		spectators.push_back( std::string( info.m_name ).substr( 0, 24 ) );
	}

	size_t total_size = spectators.size( ) * ( h - 1 );

	for( size_t i{ }; i < spectators.size( ); ++i ) {
		const std::string& name = spectators[ i ];

		render::menu_shade.string( g_cl.m_width - 20, ( g_cl.m_height / 2 ) - ( total_size / 2 ) + ( i * ( h - 1 ) ),
			{ 255, 255, 255, 179 }, name, render::ALIGN_RIGHT );
	}*/
}

void Visuals::StatusIndicators( ) {
// todo: check pandora
}

void Visuals::PenetrationCrosshair() {
	//int     x, y;
	//bool    valid_player_hit;
	//col_t final_color;

	//if (!g_menu.main.visuals.pen_crosshair.get() || !g_cl.m_processing)
	//	return;

	//x = g_cl.m_width / 2;
	//y = g_cl.m_height / 2;


	//valid_player_hit = (g_cl.m_pen_data.m_target && g_cl.m_pen_data.m_target->enemy(g_cl.m_local));
	//if (valid_player_hit)
	//	final_color = colors::transparent_yellow;

	//else if (g_cl.m_pen_data.m_pen)
	//	final_color = colors::transparent_green;

	//else
	//	final_color = colors::transparent_red;

	//// todo - dex; use fmt library to get damage string here?
	////             draw damage string?

	//// draw small square in center of screen.
	//render::rect_filled(x - 1, y - 1, 3, 3, final_color);
}

void Visuals::DrawProjectile( c_base_entity* ent, e_class_id nIndex ) {

	vec2_t screen;
	vec3_t origin = ent->get_abs_origin( );
	if( !render::world_to_screen( origin, screen ) )
		return;

	// setup temporary values
	const char* szName = ("NONE");
	col_t col = cfg->m_visuals.iProjectileColor;
	col.alpha(0xb4);

	// get grenade model name
	if (std::string_view szModelName = interfaces::m_model_info->get_model_name( ent->get_model( ) ); !szModelName.empty( ) )
	{
		// get grenade info
		switch (nIndex)
		{
		case C_BASE_CS_GRENADE_PROJECTILE:
		{
			// separate greandes by model name
			if (szModelName.find(("fraggrenade")) != std::string_view::npos)
				szName = ("FRAG");
			else if (szModelName.find(("flashbang")) != std::string_view::npos)
				szName = ("FLASH");
			break;
		}
		case C_DECOY_PROJECTILE:
		{
			szName = ("DECOY");
			break;
		}
		case C_SMOKE_GRENADE_PROJECTILE:
		{
			// cast to smoke grenade
			//CSmokeGrenade* pSmoke = reinterpret_cast<CSmokeGrenade*>(pGrenade);
			//flFactor = ((TICKS_TO_TIME(pSmoke->GetEffectTickBegin()) + pSmoke->GetMaxTime()) - flServerTime) / pSmoke->GetMaxTime();
			//colGrenade = Color(230, 130, 0);
			szName = ("SMOKE");
			break;
		}
		case C_MOLOTOV_PROJECTILE:
		case C_INFERNO:
		{
			// cast to inferno grenade
			//CInferno* pInferno = reinterpret_cast<CInferno*>(pGrenade);
			//flFactor = ((TICKS_TO_TIME(pInferno->GetEffectTickBegin()) + pInferno->GetMaxTime()) - flServerTime) / pInferno->GetMaxTime();
			//colGrenade = Color(255, 100, 100);

			// separate grenades by model name
			if (szModelName.find(("molotov")) != std::string_view::npos)
				szName = ("MOLOTOV");
			else if (szModelName.find(("incendiary")) != std::string_view::npos)
				szName = ("INCENDIARY");
			else
				szName = ("FIRE");
			break;
		}
		default:
			break;
		}
	}

	render::text( render::fonts::m_esp, ImVec2( screen.x, screen.y ), szName, col, true );
	// draw here other on grenade aka warning
}

void Visuals::DrawItem( c_base_combat_weapon* item ) {
	//// we only want to draw shit without owner.
	//c_base_entity* owner = static_cast< c_base_entity* >( interfaces::m_entity_list->get_client_entity_from_handle( item->get_owner_entity( ) ) );
	//if( owner )
	//	return;

	//// is the fucker even on the screen?
	//vec2_t screen;
	//vec3_t origin = item->get_abs_origin( );
	//if( !render::world_to_screen( origin, screen ) )
	//	return;

	//c_cs_weapon_data* data = item->get_cs_weapon_data( );
	//if( !data )
	//	return;

	//col_t col = g_menu.main.visuals.item_color.get( );
	//col.alpha( 0xb4 );

	//// render bomb in green.
	//if( item->is( HASH( "CC4" ) ) )
	//	render::text( render::fonts::m_esp, ImVec2( screen.x, screen.y ), ("BOMB"), { 150, 200, 60, 0xb4 }, true );

	//// if not bomb
	//// normal item, get its name.
	//else {
	//	std::string name{ item->get_name( ) };

	//	// smallfonts needs uppercase.
	//	std::transform( name.begin( ), name.end( ), name.begin( ), ::toupper );

	//	render::text( render::fonts::m_esp, ImVec2( screen.x, screen.y ), name, col, true );
	//}

	//if( !g_menu.main.visuals.ammo.get( ) )
	//	return;

	//// nades do not have ammo.
	//if( data->m_weapon_type == WEAPONTYPE_GRENADE || data->m_weapon_type == WEAPONTYPE_KNIFE )
	//	return;

	//if( item->get_item_definition_index( ) == 0 || item->get_item_definition_index( ) == C4 )
	//	return;

	//std::string ammo = tfm::format( ( "(%i/%i)" ), item->get_ammo( ), item->m_iPrimaryReserveAmmoCount( ) );
	//render::text( render::fonts::m_esp, ImVec2( screen.x, screen.y - render::fonts::m_esp->FontSize - 1 ), ammo, col, true );
}

void Visuals::OffScreen( c_cs_player* player, int alpha ) {
	//vec3_t view_origin, target_pos, delta;
	//vec2_t screen_pos, offscreen_pos;
	//float  leeway_x, leeway_y, radius, offscreen_rotation;
	//bool   is_on_screen;
	//Vertex verts[ 3 ], verts_outline[ 3 ];
	//Color  color;

	//// todo - dex; move this?
	//static auto get_offscreen_data = [ ] ( const vec3_t& delta, float radius, vec2_t& out_offscreen_pos, float& out_rotation ) {
	//	ang_t  view_angles( g_csgo.m_view_render->m_view.m_angles );
	//	vec3_t fwd, right, up( 0.f, 0.f, 1.f );
	//	float  front, side, yaw_rad, sa, ca;

	//	// get viewport angles forward directional vector.
	//	math::AngleVectors( view_angles, &fwd );

	//	// convert viewangles forward directional vector to a unit vector.
	//	fwd.z = 0.f;
	//	fwd.normalize( );

	//	// calculate front / side positions.
	//	right = up.cross( fwd );
	//	front = delta.dot( fwd );
	//	side = delta.dot( right );

	//	// setup offscreen position.
	//	out_offscreen_pos.x = radius * -side;
	//	out_offscreen_pos.y = radius * -front;

	//	// get the rotation ( yaw, 0 - 360 ).
	//	out_rotation = math::rad_to_deg( std::atan2( out_offscreen_pos.x, out_offscreen_pos.y ) + math::pi );

	//	// get needed sine / cosine values.
	//	yaw_rad = math::deg_to_rad( -out_rotation );
	//	sa = std::sin( yaw_rad );
	//	ca = std::cos( yaw_rad );

	//	// rotate offscreen position around.
	//	out_offscreen_pos.x = ( int ) ( ( g_cl.m_width / 2.f ) + ( radius * sa ) );
	//	out_offscreen_pos.y = ( int ) ( ( g_cl.m_height / 2.f ) - ( radius * ca ) );
	//};

	//if( !g_menu.main.players.offscreen.get( ) )
	//	return;

	//if( !g_cl.m_processing || !g_cl.m_local->enemy( player ) )
	//	return;

	//// get the player's center screen position.
	//target_pos = player->WorldSpaceCenter( );
	//is_on_screen = render::world_to_screen( target_pos, screen_pos );

	//// give some extra room for screen position to be off screen.
	//leeway_x = g_cl.m_width / 18.f;
	//leeway_y = g_cl.m_height / 18.f;

	//// origin is not on the screen at all, get offscreen position data and start rendering.
	//if( !is_on_screen
	//	|| screen_pos.x < -leeway_x
	//	|| screen_pos.x >( g_cl.m_width + leeway_x )
	//	|| screen_pos.y < -leeway_y
	//	|| screen_pos.y >( g_cl.m_height + leeway_y ) ) {

	//	// get viewport origin.
	//	view_origin = g_csgo.m_view_render->m_view.m_origin;

	//	// get direction to target.
	//	delta = ( target_pos - view_origin ).normalized( );

	//	// note - dex; this is the 'YRES' macro from the source sdk.
	//	radius = 200.f * ( g_cl.m_height / 480.f );

	//	// get the data we need for rendering.
	//	get_offscreen_data( delta, radius, offscreen_pos, offscreen_rotation );

	//	// bring rotation back into range... before rotating verts, sine and cosine needs this value inverted.
	//	// note - dex; reference: 
	//	// https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/src_main/game/client/tf/tf_hud_damageindicator.cpp#L182
	//	offscreen_rotation = -offscreen_rotation;

	//	// setup vertices for the triangle.
	//	verts[ 0 ] = { offscreen_pos.x, offscreen_pos.y };        // 0,  0
	//	verts[ 1 ] = { offscreen_pos.x - 12.f, offscreen_pos.y + 24.f }; // -1, 1
	//	verts[ 2 ] = { offscreen_pos.x + 12.f, offscreen_pos.y + 24.f }; // 1,  1

	//	// setup verts for the triangle's outline.
	//	verts_outline[ 0 ] = { verts[ 0 ].m_pos.x - 1.f, verts[ 0 ].m_pos.y - 1.f };
	//	verts_outline[ 1 ] = { verts[ 1 ].m_pos.x - 1.f, verts[ 1 ].m_pos.y + 1.f };
	//	verts_outline[ 2 ] = { verts[ 2 ].m_pos.x + 1.f, verts[ 2 ].m_pos.y + 1.f };

	//	// rotate all vertices to point towards our target.
	//	verts[ 0 ] = render::RotateVertex( offscreen_pos, verts[ 0 ], offscreen_rotation );
	//	verts[ 1 ] = render::RotateVertex( offscreen_pos, verts[ 1 ], offscreen_rotation );
	//	verts[ 2 ] = render::RotateVertex( offscreen_pos, verts[ 2 ], offscreen_rotation );
	//	// verts_outline[ 0 ] = render::RotateVertex( offscreen_pos, verts_outline[ 0 ], offscreen_rotation );
	//	// verts_outline[ 1 ] = render::RotateVertex( offscreen_pos, verts_outline[ 1 ], offscreen_rotation );
	//	// verts_outline[ 2 ] = render::RotateVertex( offscreen_pos, verts_outline[ 2 ], offscreen_rotation );

	//	// todo - dex; finish this, i want it.
	//	// auto &damage_data = m_offscreen_damage[ player->index( ) ];
	//	// 
	//	// // the local player was damaged by another player recently.
	//	// if( damage_data.m_time > 0.f ) {
	//	//     // // only a small amount of time left, start fading into white again.
	//	//     // if( damage_data.m_time < 1.f ) {
	//	//     //     // calculate step needed to reach 255 in 1 second.
	//	//     //     // float step = UINT8_MAX / ( 1000.f * g_csgo.m_globals->m_frametime );
	//	//     //     float step = ( 1.f / g_csgo.m_globals->m_frametime ) / UINT8_MAX;
	//	//     //     
	//	//     //     // increment the new value for the color.
	//	//     //     // if( damage_data.m_color_step < 255.f )
	//	//     //         damage_data.m_color_step += step;
	//	//     // 
	//	//     //     math::clamp( damage_data.m_color_step, 0.f, 255.f );
	//	//     // 
	//	//     //     damage_data.m_color.g( ) = (uint8_t)damage_data.m_color_step;
	//	//     //     damage_data.m_color.b( ) = (uint8_t)damage_data.m_color_step;
	//	//     // }
	//	//     // 
	//	//     // g_cl.print( "%f %f %u %u %u\n", damage_data.m_time, damage_data.m_color_step, damage_data.m_color.r( ), damage_data.m_color.g( ), damage_data.m_color.b( ) );
	//	//     
	//	//     // decrement timer.
	//	//     damage_data.m_time -= g_csgo.m_globals->m_frametime;
	//	// }
	//	// 
	//	// else
	//	//     damage_data.m_color = colors::white;

	//	// render!
	//	color = g_menu.main.players.offscreen_color.get( ); // damage_data.m_color;
	//	color.a( ) = ( alpha == 255 ) ? alpha : alpha / 2;

	//	g_csgo.m_surface->DrawSetColor( color );
	//	g_csgo.m_surface->DrawTexturedPolygon( 3, verts );

	//	// g_csgo.m_surface->DrawSetColor( colors::black );
	//	// g_csgo.m_surface->DrawTexturedPolyLine( 3, verts_outline );
	//}
}

void Visuals::DrawPlayer( c_cs_player* player ) {
	constexpr float MAX_DORMANT_TIME = 10.f;
	constexpr float DORMANT_FADE_TIME = MAX_DORMANT_TIME / 2.f;

	Rect_t		  box;
	player_info_t info;
	col_t       color;

	// get player index.
	int index = player->get_index( );

	// get reference to array variable.
	float& opacity = m_opacities[ index - 1 ];
	bool& draw = m_draw[ index - 1 ];

	// opacity should reach 1 in 300 milliseconds.
	constexpr int frequency = 1.f / 0.3f;

	// the increment / decrement per frame.
	float step = frequency * interfaces::m_global_vars->m_frame_time;

	// is player enemy.
	bool enemy = player->is_enemy( local_player->m_local );
	bool dormant = player->is_dormant( );

	if( cfg->m_visuals.bSpotted && enemy && !dormant )
		player->is_spotted( ) = true;

	// we can draw this player again.
	if( !dormant )
		draw = true;

	if( !draw )
		return;

	// if non-dormant	-> increment
	// if dormant		-> decrement
	dormant ? opacity -= step : opacity += step;

	// is dormant esp enabled for this player.
	bool dormant_esp = enemy && cfg->m_visuals.bDormant;

	// clamp the opacity.
	math::clamp( opacity, 0.f, 1.f );
	if( !opacity && !dormant_esp )
		return;

	// stay for x seconds max.
	float dt = interfaces::m_global_vars->m_cur_time - player->get_sim_time( );
	if( dormant && dt > MAX_DORMANT_TIME )
		return;

	// calculate alpha channels.
	int alpha = ( int ) ( 255.f * opacity );
	int low_alpha = ( int ) ( 179.f * opacity );

	// get color based on enemy or not.
	color = enemy ? cfg->m_visuals.iBoxColor : cfg->m_visuals.iBoxFriendlyColor;

	if( dormant && dormant_esp ) {
		alpha = 112;
		low_alpha = 80;

		// fade.
		if( dt > DORMANT_FADE_TIME ) {
			// for how long have we been fading?
			float faded = ( dt - DORMANT_FADE_TIME );
			float scale = 1.f - ( faded / DORMANT_FADE_TIME );

			alpha *= scale;
			low_alpha *= scale;
		}

		// override color.
		color = { 112, 112, 112 };
	}

	// override alpha.
	color.alpha( alpha );

	// get player info.
	if( !interfaces::m_engine->get_player_info( index, &info ) )
		return;

	// run offscreen ESP.
	//OffScreen( player, alpha );

	// attempt to get player box.
	if( !GetPlayerBoxRect( player, box ) ) {
		// OffScreen( player );
		return;
	}

	// DebugAimbotPoints( player );

	bool bone_esp = ( enemy && cfg->m_visuals.bSkeleton ) || ( !enemy && cfg->m_visuals.bSkeletonFriendly );
	if( bone_esp )
		DrawSkeleton( player, alpha );

	// is box esp enabled for this player.
	bool box_esp = ( enemy && cfg->m_visuals.bBox ) || ( !enemy && cfg->m_visuals.bBoxFriendly );

	// render box if specified.
	if( box_esp )
		render::rect_outlined( box.x, box.y, box.w, box.h, color, { 10, 10, 10, low_alpha } );

	// is name esp enabled for this player.
	bool name_esp = ( enemy && cfg->m_visuals.bName ) || ( !enemy && cfg->m_visuals.bNameFriendly );

	// draw name.
	if( name_esp ) {
		// fix retards with their namechange meme 
		// the point of this is overflowing unicode compares with hardcoded buffers, good hvh strat
		std::string name{ std::string( info.m_name ).substr( 0, 24 ) };

		col_t clr = cfg->m_visuals.iNameColor;
		// override alpha.
		clr.alpha( low_alpha );

		render::text( render::fonts::m_esp, ImVec2( box.x + box.w / 2, box.y - render::fonts::m_esp->FontSize ), name, clr, true );
	}

	// is health esp enabled for this player.
	bool health_esp = ( enemy && cfg->m_visuals.bHealth ) || ( !enemy && cfg->m_visuals.bHealthFriendly );

	if( health_esp ) {
		int y = box.y + 1;
		int h = box.h - 2;

		// retarded servers that go above 100 hp..
		int hp = min( 100, player->get_health( ) );

		// calculate hp bar color.
		int r = min( ( 510 * ( 100 - hp ) ) / 100, 255 );
		int g = min( ( 510 * hp ) / 100, 255 );

		// get hp bar height.
		int fill = ( int ) std::round( hp * h / 100.f );

		// render background.
		render::rect_filled( box.x - 6, y - 1, 4, h + 2, { 10, 10, 10, low_alpha } );

		// render actual bar.
		render::rect( box.x - 5, y + h - fill, 2, fill, { r, g, 0, alpha } );

		// if hp is below max, draw a string.
		if( hp != 100 )
			render::text( render::fonts::m_esp, ImVec2( box.x - 5, y + ( h - fill ) - 5 ), std::to_string( hp ), { 255, 255, 255, low_alpha }, true );
	}

	// draw flags.
	/* {
		std::vector< std::pair< std::string, c_color > > flags;

		auto items = enemy ? g_menu.main.players.flags_enemy.GetActiveIndices( ) : g_menu.main.players.flags_friendly.GetActiveIndices( );

		// NOTE FROM NITRO TO DEX -> stop removing my iterator loops, i do it so i dont have to check the size of the vector
		// with range loops u do that to do that.
		for( auto it = items.begin( ); it != items.end( ); ++it ) {

			// money.
			if( *it == 0 )
				flags.push_back( { tfm::format( XOR( "$%i" ), player->m_iAccount( ) ), { 150, 200, 60, low_alpha } } );

			// armor.
			if( *it == 1 ) {
				// helmet and kevlar.
				if( player->m_bHasHelmet( ) && player->m_ArmorValue( ) > 0 )
					flags.push_back( { XOR( "HK" ), { 255, 255, 255, low_alpha } } );

				// only helmet.
				else if( player->m_bHasHelmet( ) )
					flags.push_back( { XOR( "H" ), { 255, 255, 255, low_alpha } } );

				// only kevlar.
				else if( player->m_ArmorValue( ) > 0 )
					flags.push_back( { XOR( "K" ), { 255, 255, 255, low_alpha } } );
			}

			// scoped.
			if( *it == 2 && player->m_bIsScoped( ) )
				flags.push_back( { XOR( "ZOOM" ), { 60, 180, 225, low_alpha } } );

			// flashed.
			if( *it == 3 && player->m_flFlashBangTime( ) > 0.f )
				flags.push_back( { XOR( "FLASHED" ), { 255, 255, 0, low_alpha } } );

			// reload.
			if( *it == 4 ) {
				// get ptr to layer 1.
				C_AnimationLayer* layer1 = &player->m_AnimOverlay( )[ 1 ];

				// check if reload animation is going on.
				if( layer1->m_weight != 0.f && player->GetSequenceActivity( layer1->m_sequence ) == 967 ) // ACT_CSGO_RELOAD
					flags.push_back( { XOR( "RELOAD" ), { 60, 180, 225, low_alpha } } );
			}

			// bomb.
			if( *it == 5 && player->HasC4( ) )
				flags.push_back( { XOR( "BOMB" ), { 255, 0, 0, low_alpha } } );
		}

		// iterate flags.
		for( size_t i{ }; i < flags.size( ); ++i ) {
			// get flag job (pair).
			const auto& f = flags[ i ];

			int offset = i * ( render::fonts::m_esp->FontSize - 1 );

			// draw flag.
			render::text( render::fonts::m_esp, ImVec2( box.x + box.w + 2, box.y + offset ), f.first, f.second, false );
		}
	}*/

	// draw bottom bars.
	{
		int  offset{ 0 };

		// draw weapon.
		if( ( enemy && cfg->m_visuals.bWeapon ) || ( !enemy && cfg->m_visuals.bWeaponFriendly ) ) {
			c_base_combat_weapon* weapon = player->get_active_weapon( );
			if( weapon ) {
				c_cs_weapon_data* data = weapon->get_cs_weapon_data( );
				if( data ) {
					int bar;
					float scale;

					// the maxclip1 in the weaponinfo
					int max = data->m_max_clip1;
					int current = weapon->get_ammo( );

					anim_layer_t* layer1 = &player->get_anim_layers( )[ 1 ];

					// set reload state.
					bool reload = ( layer1->m_weight != 0.f ) && ( player->get_sequence_activity( layer1->m_sequence ) == 967 );

					// ammo bar.
					if( max != -1 && cfg->m_visuals.bWeaponAmmo ) {
						// check for reload.
						if( reload )
							scale = layer1->m_cycle;

						// not reloading.
						// make the division of 2 ints produce a float instead of another int.
						else
							scale = ( float ) current / max;

						// relative to bar.
						bar = ( int ) std::round( ( box.w - 2 ) * scale );

						// draw.
						render::rect_filled( box.x, box.y + box.h + 2 + offset, box.w, 4, { 10, 10, 10, low_alpha } );

						col_t clr = cfg->m_visuals.iWeaponAmmoColor;
						clr.alpha( alpha );
						render::rect( box.x + 1, box.y + box.h + 3 + offset, bar, 2, clr );

						// less then a 5th of the bullets left.
						if( current <= ( int ) std::round( max / 5 ) && !reload )
							render::text( render::fonts::m_esp, ImVec2( box.x + bar, box.y + box.h + offset ), std::to_string( current ), { 255, 255, 255, low_alpha }, true );

						offset += 6;
					}

					// text.
					if( cfg->m_visuals.bWeaponText ) {
						// construct std::string instance of localized weapon name.
						std::string name{ weapon->get_name( ) };

						// smallfonts needs upper case.
						std::transform( name.begin( ), name.end( ), name.begin( ), ::toupper );

						render::text( render::fonts::m_esp, ImVec2( box.x + box.w / 2, box.y + box.h + offset ), name, { 255, 255, 255, low_alpha }, true );
					}

					// icons.
					//if( cfg->m_visuals.bWeaponIcon ) {
					//	// icons are super fat..
					//	// move them back up.
					//	offset -= 5;

					//	std::string icon = tfm::format( ( "%c" ), m_weapon_icons[ weapon->get_item_definition_index( ) ] );
					//	render::text( render::fonts::m_weapon, ImVec2( box.x + box.w / 2, box.y + box.h + offset ), icon, { 255, 255, 255, low_alpha }, true );
					//}
				}
			}
		}
	}
}

void Visuals::DrawPlantedC4( ) {
	//bool        mode_2d, mode_3d, is_visible;
	//float       explode_time_diff, dist, range_damage;
	//vec3_t      dst, to_target;
	//int         final_damage;
	//std::string time_str, damage_str;
	//Color       damage_color;
	//vec2_t      screen_pos;

	//static auto scale_damage = [ ] ( float damage, int armor_value ) {
	//	float new_damage, armor;

	//	if( armor_value > 0 ) {
	//		new_damage = damage * 0.5f;
	//		armor = ( damage - new_damage ) * 0.5f;

	//		if( armor > ( float ) armor_value ) {
	//			armor = ( float ) armor_value * 2.f;
	//			new_damage = damage - armor;
	//		}

	//		damage = new_damage;
	//	}

	//	return std::max( 0, ( int ) std::floor( damage ) );
	//};

	//// store menu vars for later.
	//mode_2d = g_menu.main.visuals.planted_c4.get( 0 );
	//mode_3d = g_menu.main.visuals.planted_c4.get( 1 );
	//if( !mode_2d && !mode_3d )
	//	return;

	//// bomb not currently active, do nothing.
	//if( !m_c4_planted )
	//	return;

	//// calculate bomb damage.
	//// references:
	////     https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/game/shared/cstrike/weapon_c4.cpp#L271
	////     https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/game/shared/cstrike/weapon_c4.cpp#L437
	////     https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/sdk/sdk_gamerules.cpp#L173
	//{
	//	// get our distance to the bomb.
	//	// todo - dex; is dst right? might need to reverse CBasePlayer::BodyTarget...
	//	dst = g_cl.m_local->WorldSpaceCenter( );
	//	to_target = m_planted_c4_explosion_origin - dst;
	//	dist = to_target.length( );

	//	// calculate the bomb damage based on our distance to the C4's explosion.
	//	range_damage = m_planted_c4_damage * std::exp( ( dist * dist ) / ( ( m_planted_c4_radius_scaled * -2.f ) * m_planted_c4_radius_scaled ) );

	//	// now finally, scale the damage based on our armor (if we have any).
	//	final_damage = scale_damage( range_damage, g_cl.m_local->m_ArmorValue( ) );
	//}

	//// m_flC4Blow is set to gpGlobals->curtime + m_flTimerLength inside CPlantedC4.
	//explode_time_diff = m_planted_c4_explode_time - g_csgo.m_globals->m_curtime;

	//// get formatted strings for bomb.
	//time_str = tfm::format( XOR( "%.2f" ), explode_time_diff );
	//damage_str = tfm::format( XOR( "%i" ), final_damage );

	//// get damage color.
	//damage_color = ( final_damage < g_cl.m_local->m_iHealth( ) ) ? colors::white : colors::red;

	//// finally do all of our rendering.
	//is_visible = render::WorldToScreen( m_planted_c4_explosion_origin, screen_pos );

	//// 'on screen (2D)'.
	//if( mode_2d ) {
	//	// g_cl.m_height - 80 - ( 30 * i )
	//	// 80 - ( 30 * 2 ) = 20

	//	// render::menu_shade.string( 60, g_cl.m_height - 20 - ( render::hud_size.m_height / 2 ), 0xff0000ff, "", render::hud );

	//	// todo - dex; move this next to indicators?

	//	if( explode_time_diff > 0.f )
	//		render::esp.string( 2, 65, colors::white, time_str, render::ALIGN_LEFT );

	//	if( g_cl.m_local->alive( ) )
	//		render::esp.string( 2, 65 + render::esp.m_size.m_height, damage_color, damage_str, render::ALIGN_LEFT );
	//}

	//// 'on bomb (3D)'.
	//if( mode_3d && is_visible ) {
	//	if( explode_time_diff > 0.f )
	//		render::esp_small.string( screen_pos.x, screen_pos.y, colors::white, time_str, render::ALIGN_CENTER );

	//	// only render damage string if we're alive.
	//	if( g_cl.m_local->alive( ) )
	//		render::esp_small.string( screen_pos.x, ( int ) screen_pos.y + render::esp_small.m_size.m_height, damage_color, damage_str, render::ALIGN_CENTER );
	//}
}

bool Visuals::GetPlayerBoxRect(c_cs_player* player, Rect_t& box) {
	vec3_t origin, mins, maxs;
	vec2_t bottom, top;

	// get interpolated origin.
	origin = player->get_abs_origin();

	// get hitbox bounds.
	player->compute_hitbox_surrounding_box( &mins, &maxs );

	// correct x and y coordinates.
	mins = { origin.x, origin.y, mins.z };
	maxs = { origin.x, origin.y, maxs.z + 8.f };

	if (!render::world_to_screen(mins, bottom) || !render::world_to_screen(maxs, top))
		return false;

	box.h = bottom.y - top.y;
	box.w = box.h / 2.f;
	box.x = bottom.x - (box.w / 2.f);
	box.y = bottom.y - box.h;

	return true;
}

void Visuals::DrawHistorySkeleton( c_cs_player* player, int opacity ) {
	//const model_t* model;
	//studiohdr_t* hdr;
	//mstudiobone_t* bone;
	//AimPlayer* data;
	//LagRecord* record;
	//int           parent;
	//vec3_t        bone_pos, parent_pos;
	//vec2_t        bone_pos_screen, parent_pos_screen;

	//if( !g_menu.main.misc.fake_latency.get( ) )
	//	return;

	//// get player's model.
	//model = player->GetModel( );
	//if( !model )
	//	return;

	//// get studio model.
	//hdr = g_csgo.m_model_info->GetStudioModel( model );
	//if( !hdr )
	//	return;

	//data = &g_aimbot.m_players[ player->index( ) - 1 ];
	//if( !data )
	//	return;

	//record = g_resolver.FindLastRecord( data );
	//if( !record )
	//	return;

	//for( int i{ }; i < hdr->m_num_bones; ++i ) {
	//	// get bone.
	//	bone = hdr->GetBone( i );
	//	if( !bone || !( bone->m_flags & BONE_USED_BY_HITBOX ) )
	//		continue;

	//	// get parent bone.
	//	parent = bone->m_parent;
	//	if( parent == -1 )
	//		continue;

	//	// resolve main bone and parent bone positions.
	//	record->m_bones->get_bone( bone_pos, i );
	//	record->m_bones->get_bone( parent_pos, parent );

	//	Color clr = player->enemy( g_cl.m_local ) ? g_menu.main.players.skeleton_enemy.get( ) : g_menu.main.players.skeleton_friendly.get( );
	//	clr.a( ) = opacity;

	//	// world to screen both the bone parent bone then draw.
	//	if( render::WorldToScreen( bone_pos, bone_pos_screen ) && render::WorldToScreen( parent_pos, parent_pos_screen ) )
	//		render::line( bone_pos_screen.x, bone_pos_screen.y, parent_pos_screen.x, parent_pos_screen.y, clr );
	//}
}

void Visuals::DrawSkeleton( c_cs_player* player, int opacity ) {
	const model_t* model;
	studiohdr_t* hdr;
	mstudiobone_t* bone;
	int           parent;
	BoneArray     matrix[ 128 ];
	vec3_t        bone_pos, parent_pos;
	vec2_t        bone_pos_screen, parent_pos_screen;

	// get player's model.
	model = player->get_model( );
	if( !model )
		return;

	// get studio model.
	hdr = interfaces::m_model_info->get_studio_model( model );
	if( !hdr )
		return;

	// get bone matrix.
	if( !player->setup_bones( matrix, 128, BONE_FLAG_USED_BY_ANYTHING, interfaces::m_global_vars->m_cur_time ) )
		return;

	for( int i{ }; i < hdr->m_bones_count; ++i ) {
		// get bone.
		bone = hdr->get_bone( i );
		if( !bone || !( bone->m_flags & BONE_FLAG_USED_BY_HITBOX ) )
			continue;

		// get parent bone.
		parent = bone->m_parent;
		if( parent == -1 )
			continue;

		// resolve main bone and parent bone positions.
		matrix->get_bone( bone_pos, i );
		matrix->get_bone( parent_pos, parent );

		col_t clr = player->is_enemy( local_player->m_local ) ? cfg->m_visuals.iSkeletonColor : cfg->m_visuals.iSkeletonFriendlyColor;
		clr.alpha( opacity );

		// world to screen both the bone parent bone then draw.
		if( render::world_to_screen( bone_pos, bone_pos_screen ) && render::world_to_screen( parent_pos, parent_pos_screen ) )
			render::line( bone_pos_screen.x, bone_pos_screen.y, parent_pos_screen.x, parent_pos_screen.y, clr, 1 ); // todo: check thickness
	}
}

//void Visuals::RenderGlow( ) {
//	col_t   color;
//	Player* player;
//
//	if( !g_cl.m_local )
//		return;
//
//	if( !g_csgo.m_glow->m_object_definitions.Count( ) )
//		return;
//
//	float blend = g_menu.main.players.glow_blend.get( ) / 100.f;
//
//	for( int i{ }; i < g_csgo.m_glow->m_object_definitions.Count( ); ++i ) {
//		GlowObjectDefinition_t* obj = &g_csgo.m_glow->m_object_definitions[ i ];
//
//		// skip non-players.
//		if( !obj->m_entity || !obj->m_entity->IsPlayer( ) )
//			continue;
//
//		// get player ptr.
//		player = obj->m_entity->as< Player* >( );
//
//		if( player->m_bIsLocalPlayer( ) )
//			continue;
//
//		// get reference to array variable.
//		float& opacity = m_opacities[ player->index( ) - 1 ];
//
//		bool enemy = player->enemy( g_cl.m_local );
//
//		if( enemy && !g_menu.main.players.glow.get( 0 ) )
//			continue;
//
//		if( !enemy && !g_menu.main.players.glow.get( 1 ) )
//			continue;
//
//		// enemy color
//		if( enemy )
//			color = g_menu.main.players.glow_enemy.get( );
//
//		// friendly color
//		else
//			color = g_menu.main.players.glow_friendly.get( );
//
//		obj->m_render_occluded = true;
//		obj->m_render_unoccluded = false;
//		obj->m_render_full_bloom = false;
//		obj->m_color = { ( float ) color.r( ) / 255.f, ( float ) color.g( ) / 255.f, ( float ) color.b( ) / 255.f };
//		obj->m_alpha = opacity * blend;
//	}
//}

//void Visuals::DrawHitboxMatrix( LagRecord* record, Color col, float time ) {
//	const model_t* model;
//	studiohdr_t* hdr;
//	mstudiohitboxset_t* set;
//	mstudiobbox_t* bbox;
//	vec3_t             mins, maxs, origin;
//	ang_t			   angle;
//
//	model = record->m_player->GetModel( );
//	if( !model )
//		return;
//
//	hdr = g_csgo.m_model_info->GetStudioModel( model );
//	if( !hdr )
//		return;
//
//	set = hdr->GetHitboxSet( record->m_player->m_nHitboxSet( ) );
//	if( !set )
//		return;
//
//	for( int i{ }; i < set->m_hitboxes; ++i ) {
//		bbox = set->GetHitbox( i );
//		if( !bbox )
//			continue;
//
//		// bbox.
//		if( bbox->m_radius <= 0.f ) {
//			// https://developer.valvesoftware.com/wiki/Rotation_Tutorial
//
//			// convert rotation angle to a matrix.
//			matrix3x4_t rot_matrix;
//			g_csgo.AngleMatrix( bbox->m_angle, rot_matrix );
//
//			// apply the rotation to the entity input space (local).
//			matrix3x4_t matrix;
//			math::ConcatTransforms( record->m_bones[ bbox->m_bone ], rot_matrix, matrix );
//
//			// extract the compound rotation as an angle.
//			ang_t bbox_angle;
//			math::MatrixAngles( matrix, bbox_angle );
//
//			// extract hitbox origin.
//			vec3_t origin = matrix.GetOrigin( );
//
//			// draw box.
//			g_csgo.m_debug_overlay->AddBoxOverlay( origin, bbox->m_mins, bbox->m_maxs, bbox_angle, col.r( ), col.g( ), col.b( ), 0, time );
//		}
//
//		// capsule.
//		else {
//			// NOTE; the angle for capsules is always 0.f, 0.f, 0.f.
//
//			// create a rotation matrix.
//			matrix3x4_t matrix;
//			g_csgo.AngleMatrix( bbox->m_angle, matrix );
//
//			// apply the rotation matrix to the entity output space (world).
//			math::ConcatTransforms( record->m_bones[ bbox->m_bone ], matrix, matrix );
//
//			// get world positions from new matrix.
//			math::VectorTransform( bbox->m_mins, matrix, mins );
//			math::VectorTransform( bbox->m_maxs, matrix, maxs );
//
//			g_csgo.m_debug_overlay->AddCapsuleOverlay( mins, maxs, bbox->m_radius, col.r( ), col.g( ), col.b( ), col.a( ), time, 0, 0 );
//		}
//	}
//}

void Visuals::DrawBeams( ) {
	//size_t     impact_count;
	//float      curtime, dist;
	//bool       is_final_impact;
	//vec3_t     va_fwd, start, dir, end;
	//BeamInfo_t beam_info;
	//Beam_t* beam;

	//if( !g_cl.m_local )
	//	return;

	//if( !g_menu.main.visuals.impact_beams.get( ) )
	//	return;

	//auto vis_impacts = &g_shots.m_vis_impacts;

	//// the local player is dead, clear impacts.
	//if( !g_cl.m_processing ) {
	//	if( !vis_impacts->empty( ) )
	//		vis_impacts->clear( );
	//}

	//else {
	//	impact_count = vis_impacts->size( );
	//	if( !impact_count )
	//		return;

	//	curtime = game::TICKS_TO_TIME( g_cl.m_local->m_nTickBase( ) );

	//	for( size_t i{ impact_count }; i-- > 0; ) {
	//		auto impact = &vis_impacts->operator[ ]( i );
	//		if( !impact )
	//			continue;

	//		// impact is too old, erase it.
	//		if( std::abs( curtime - game::TICKS_TO_TIME( impact->m_tickbase ) ) > g_menu.main.visuals.impact_beams_time.get( ) ) {
	//			vis_impacts->erase( vis_impacts->begin( ) + i );

	//			continue;
	//		}

	//		// already rendering this impact, skip over it.
	//		if( impact->m_ignore )
	//			continue;

	//		// is this the final impact?
	//		// last impact in the vector, it's the final impact.
	//		if( i == ( impact_count - 1 ) )
	//			is_final_impact = true;

	//		// the current impact's tickbase is different than the next, it's the final impact.
	//		else if( ( i + 1 ) < impact_count && impact->m_tickbase != vis_impacts->operator[ ]( i + 1 ).m_tickbase )
	//			is_final_impact = true;

	//		else
	//			is_final_impact = false;

	//		// is this the final impact?
	//		// is_final_impact = ( ( i == ( impact_count - 1 ) ) || ( impact->m_tickbase != vis_impacts->at( i + 1 ).m_tickbase ) );

	//		if( is_final_impact ) {
	//			// calculate start and end position for beam.
	//			start = impact->m_shoot_pos;

	//			dir = ( impact->m_impact_pos - start ).normalized( );
	//			dist = ( impact->m_impact_pos - start ).length( );

	//			end = start + ( dir * dist );

	//			// setup beam info.
	//			// note - dex; possible beam models: sprites/physbeam.vmt | sprites/white.vmt
	//			beam_info.m_vecStart = start;
	//			beam_info.m_vecEnd = end;
	//			beam_info.m_nModelIndex = g_csgo.m_model_info->GetModelIndex( XOR( "sprites/purplelaser1.vmt" ) );
	//			beam_info.m_pszModelName = XOR( "sprites/purplelaser1.vmt" );
	//			beam_info.m_flHaloScale = 0.f;
	//			beam_info.m_flLife = g_menu.main.visuals.impact_beams_time.get( );
	//			beam_info.m_flWidth = 2.f;
	//			beam_info.m_flEndWidth = 2.f;
	//			beam_info.m_flFadeLength = 0.f;
	//			beam_info.m_flAmplitude = 0.f;   // beam 'jitter'.
	//			beam_info.m_flBrightness = 255.f;
	//			beam_info.m_flSpeed = 0.5f;  // seems to control how fast the 'scrolling' of beam is... once fully spawned.
	//			beam_info.m_nStartFrame = 0;
	//			beam_info.m_flFrameRate = 0.f;
	//			beam_info.m_nSegments = 2;     // controls how much of the beam is 'split up', usually makes m_flAmplitude and m_flSpeed much more noticeable.
	//			beam_info.m_bRenderable = true;  // must be true or you won't see the beam.
	//			beam_info.m_nFlags = 0;

	//			if( !impact->m_hit_player ) {
	//				beam_info.m_flRed = g_menu.main.visuals.impact_beams_color.get( ).r( );
	//				beam_info.m_flGreen = g_menu.main.visuals.impact_beams_color.get( ).g( );
	//				beam_info.m_flBlue = g_menu.main.visuals.impact_beams_color.get( ).b( );
	//			}

	//			else {
	//				beam_info.m_flRed = g_menu.main.visuals.impact_beams_hurt_color.get( ).r( );
	//				beam_info.m_flGreen = g_menu.main.visuals.impact_beams_hurt_color.get( ).g( );
	//				beam_info.m_flBlue = g_menu.main.visuals.impact_beams_hurt_color.get( ).b( );
	//			}

	//			// attempt to render the beam.
	//			beam = game::CreateGenericBeam( beam_info );
	//			if( beam ) {
	//				g_csgo.m_beams->DrawBeam( beam );

	//				// we only want to render a beam for this impact once.
	//				impact->m_ignore = true;
	//			}
	//		}
	//	}
	//}
}

void Visuals::DebugAimbotPoints( c_cs_player* player ) {
	//std::vector< vec3_t > p2{ };

	//AimPlayer* data = &g_aimbot.m_players.at( player->index( ) - 1 );
	//if( !data || data->m_records.empty( ) )
	//	return;

	//LagRecord* front = data->m_records.front( ).get( );
	//if( !front || front->dormant( ) )
	//	return;

	//// get bone matrix.
	//BoneArray matrix[ 128 ];
	//if( !g_bones.setup( player, matrix, front ) )
	//	return;

	//data->SetupHitboxes( front, false );
	//if( data->m_hitboxes.empty( ) )
	//	return;

	//for( const auto& it : data->m_hitboxes ) {
	//	std::vector< vec3_t > p1{ };

	//	if( !data->SetupHitboxPoints( front, matrix, it.m_index, p1 ) )
	//		continue;

	//	for( auto& p : p1 )
	//		p2.push_back( p );
	//}

	//if( p2.empty( ) )
	//	return;

	//for( auto& p : p2 ) {
	//	vec2_t screen;

	//	if( render::WorldToScreen( p, screen ) )
	//		render::rect_filled( screen.x, screen.y, 2, 2, { 0, 255, 255, 255 } );
	//}
}