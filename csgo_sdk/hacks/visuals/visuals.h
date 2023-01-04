#pragma once
#include "../../globals.h"
#include "../../utils/render/render.h"

struct Rect_t {
    int x;
    int y;
    int w;
    int h;
};

//struct OffScreenDamageData_t {
//    float m_time, m_color_step;
//    c_color m_color;
//
//    __forceinline OffScreenDamageData_t() : m_time{ 0.f }, m_color{ c_color::white } {}
//    __forceinline OffScreenDamageData_t(float time, float m_color_step, c_color color) : m_time{ time }, m_color{ color } {}
//};

struct WorldHitmarkerData_t {
    float m_time, m_alpha;
    float m_pos_x, m_pos_y, m_pos_z;

    vec2_t m_world_pos;
    bool m_world_to_screen;
};

class Visuals {
public:
	std::array< bool, 64 >                  m_draw;
	std::array< float, 2048 >               m_opacities;
    //std::array< OffScreenDamageData_t, 64 > m_offscreen_damage;
	vec2_t                                  m_crosshair;
	bool                                    m_thirdperson;
	float					                m_hit_start, m_hit_end, m_hit_duration;

    // info about planted c4.
   /* bool        m_c4_planted;
    Entity      *m_planted_c4;
    float       m_planted_c4_explode_time;
    vec3_t      m_planted_c4_explosion_origin;
    float       m_planted_c4_damage;
    float       m_planted_c4_radius;
    float       m_planted_c4_radius_scaled;
    std::string m_last_bombsite;*/

	i_material* smoke1;
    i_material* smoke2;
    i_material* smoke3;
    i_material* smoke4;

    /*std::unordered_map< int, char > m_weapon_icons = {
    	{ WEAPON_DESERT_EAGLE, u8'\uE001' },
    	{ WEAPON_DUAL_BERETTAS, u8'\uE002' },
    	{ WEAPON_FIVESEVEN, u8'\uE003' },
    	{ WEAPON_GLOCK18, u8'\uE004' },
    	{ WEAPON_AK47, u8'\uE007' },
    	{ WEAPON_AUG, u8'\uE008' },
    	{ WEAPON_AWP, u8'\uE009' },
    	{ WEAPON_FAMAS, u8'\uE00A' },
    	{ WEAPON_G3SG1, u8'\uE00B' },
    	{ WEAPON_GALIL_AR, u8'\uE00D' },
    	{ WEAPON_M249, u8'\uE00E' },
    	{ WEAPON_M4A4, u8'\uE010' },
    	{ WEAPON_MAC10, u8'\uE011' },
    	{ WEAPON_P90, u8'\uE013' },
        { WEAPON_MP5SD, u8'\uE017' },
    	{ WEAPON_UMP45, u8'\uE018' },
    	{ WEAPON_XM1014, u8'\uE019' },
    	{ WEAPON_PPBIZON, u8'\uE01A' },
    	{ WEAPON_MAG7, u8'\uE01B' },
    	{ WEAPON_NEGEV, u8'\uE01C' },
    	{ WEAPON_SAWEDOFF, u8'\uE01D' },
    	{ WEAPON_TEC9, u8'\uE01E' },
    	{ WEAPON_ZEUS_X27, u8'\uE01F' },
    	{ WEAPON_P2000, u8'\uE020' },
    	{ WEAPON_MP7, u8'\uE021' },
    	{ WEAPON_MP9, u8'\uE022' },
    	{ WEAPON_NOVA, u8'\uE023' },
    	{ WEAPON_P250, u8'\uE024' },
    	{ WEAPON_SCAR20, u8'\uE026' },
    	{ WEAPON_SG_553, u8'\uE027' },
    	{ WEAPON_SSG_08, u8'\uE028' },
    	{ WEAPON_KNIFE_DEFAULT_CT, u8'\uE02A' },
    	{ WEAPON_GRENADE_FLASHBANG, u8'\uE02B' },
    	{ WEAPON_GRENADE_HE, u8'\uE02C' },
    	{ WEAPON_GRENADE_SMOKE, u8'\uE02D' },
    	{ WEAPON_GRENADE_MOLOTOV, u8'\uE02E' },
        { WEAPON_GRENADE_INCENDIARY, u8'\uE030' },
    	{ WEAPON_GRENADE_DECOY, 'Deco' },
    	{ WEAPON_C4, u8'\uE031' },
    	{ WEAPON_KNIFE_DEFAULT_T, u8'\uE03B' },
    	{ WEAPON_M4A1S, u8'\uE03C' },
    	{ WEAPON_USPS, u8'\uE03D' },
    	{ WEAPON_CZ75A, u8'\uE03F' },
    	{ WEAPON_R8_REVOLVER, u8'\uE040' },
    	{ WEAPON_KNIFE_BAYONET, u8'\uE1F4' },
    	{ WEAPON_KNIFE_FLIP, u8'\uE1F9' },
    	{ WEAPON_KNIFE_GUT, u8'\uE1FA' },
    	{ WEAPON_KNIFE_KARAMBIT, u8'\uE1FB' },
    	{ WEAPON_KNIFE_M9_BAYONET, u8'\uE1FC' },
    	{ WEAPON_KNIFE_HUNTSMAN, u8'\uE1FD' },
    	{ WEAPON_KNIFE_FALCHION, u8'\uE200' },
    	{ WEAPON_KNIFE_BOWIE, u8'\uE202' },
    	{ WEAPON_KNIFE_BUTTERFLY, u8'\uE203' },
    	{ WEAPON_KNIFE_SHADOW_DAGGERS, u8'\uE204' },
    };*/

public:
	static void ModulateWorld( );
	void ThirdpersonThink( );
	void Hitmarker( );
	void NoSmoke( );
	void think( );
	void Spectators( );
	void StatusIndicators( );
    void PenetrationCrosshair( );
    void DrawPlantedC4();
	void draw( c_base_entity* ent );
	void DrawProjectile( c_base_entity* ent, e_class_id nIndex );
	void DrawItem( c_base_combat_weapon* item );
	void OffScreen( c_cs_player* player, int alpha );
	void DrawPlayer( c_cs_player* player );
	bool GetPlayerBoxRect( c_cs_player* player, Rect_t& box );
	void DrawHistorySkeleton( c_cs_player* player, int opacity );
	void DrawSkeleton( c_cs_player* player, int opacity );
	void RenderGlow( );
	//void DrawHitboxMatrix( LagRecord* record, Color col, float time );
    void DrawBeams( );
	void DebugAimbotPoints( c_cs_player* player );
};

extern Visuals g_visuals;



