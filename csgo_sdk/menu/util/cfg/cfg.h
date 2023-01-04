#pragma once
#include "../../../utils/singleton.h"

class c_cfg : public c_singleton<c_cfg> {
public:
	struct move_t {
		bool m_bhop;
	} m_move;

	bool bMiscAntiUntrusted = true;

	struct visuals_t {

		bool bSpotted;


		bool bDormant;
		bool bBox;
		col_t iBoxColor = col_t( 255, 255, 255 );
		bool bBoxFriendly;
		col_t iBoxFriendlyColor = col_t( 255, 255, 255 );
	
		bool bHealth;
		bool bHealthFriendly;

		bool bName;
		col_t iNameColor = col_t( 255, 255, 255 );
		bool bNameFriendly;

		bool bWeapon;
		bool bWeaponFriendly;
		bool bWeaponAmmo;
		col_t iWeaponAmmoColor = col_t( 50, 140, 255 );
		bool bWeaponText;
		bool bWeaponIcon;

		bool bSkeleton;
		col_t iSkeletonColor = col_t( 255, 255, 255 );
		bool bSkeletonFriendly;
		col_t iSkeletonFriendlyColor = col_t( 255, 255, 255 );

		bool bDisableScope;
		bool bNightMode;
		bool bTransparentProps;
		bool bHitMarker;
		bool bRemoveSmoke;

		bool bProjectile;
		col_t iProjectileColor = col_t( 255, 255, 255 );

	} m_visuals;


	struct models_t {
		int m_player_models_type;

		bool m_player_models;
		float m_player_models_clr[4] = { 1.f, 1.f, 1.f, 1.f };

		bool m_player_models_occluded;
		float m_player_models_occluded_clr[4] = { 1.f, 1.f, 1.f, 1.f };
	} m_models;
};

#define cfg c_cfg::instance()