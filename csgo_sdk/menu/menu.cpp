#include "menu.h"

const char* player_models_type[] {
	"regular",
	"flat"
};

namespace menu {

	void color_edit(const char* label, col_t* color) {
		float a_color[4] =
		{
			color->r() / 255.0f,
			color->g() / 255.0f,
			color->b() / 255.0f,
			color->a() / 255.0f
		};

		if (ImGui::ColorEdit4(label, a_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_RGB))
			color->set(a_color[0], a_color[1], a_color[2], a_color[3]);
	}

	void render( ) {
		if ( !m_bOpened )
			return;

		ImGui::SetNextWindowSize( ImVec2( 856, 380 ) );

		ImGui::Begin( "csgo - sdk", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse ); {
			
			ImDrawList* window = ImGui::GetWindowDrawList( );
			const ImVec2 posR = ImGui::GetCursorScreenPos( );

			ImGui::SetCursorPos( { 8.f, 27.f } );

			window->AddRect( posR, ImVec2( posR.x + 840, posR.y + 344 ), ImColor( 0.43f, 0.43f, 0.50f, 0.50f ) );

			if (ImGui::Button(("Aimbot"), ImVec2(140.0f, 22.0f)))
				m_iCurrentTab = 0;

			ImGui::SameLine(0.0f, 0.0f);

			if (ImGui::Button(("Anti-Aim"), ImVec2(140.0f, 22.0f)))
				m_iCurrentTab = 1;

			ImGui::SameLine(0.0f, 0.0f);

			if (ImGui::Button(("Visuals"), ImVec2(140.0f, 22.0f)))
				m_iCurrentTab = 2;

			ImGui::SameLine(0.0f, 0.0f);

			if (ImGui::Button(("Misc"), ImVec2(140.0f, 22.0f)))
				m_iCurrentTab = 3;

			ImGui::SameLine(0.0f, 0.0f);

			if (ImGui::Button(("Skins"), ImVec2(140.0f, 22.0f)))
				m_iCurrentTab = 4;

			ImGui::SameLine(0.0f, 0.0f);

			if (ImGui::Button(("Config/Lua"), ImVec2(140.0f, 22.0f)))
				m_iCurrentTab = 5;

			switch (m_iCurrentTab)
			{
			case 0:

				break;
			case 1:




				break;
			case 2:


				ImGui::Checkbox( "Dormant", &cfg->m_visuals.bDormant );

				ImGui::Checkbox( "Bounding box", &cfg->m_visuals.bBox ); ImGui::SameLine( ); color_edit( "##box_clr", &cfg->m_visuals.iBoxColor );

				ImGui::Checkbox( "Bounding box friendly", &cfg->m_visuals.bBoxFriendly ); ImGui::SameLine( ); color_edit( "##box_friendly_clr", &cfg->m_visuals.iBoxFriendlyColor );

				ImGui::Checkbox( "Health bar", &cfg->m_visuals.bHealth );
				ImGui::Checkbox( "Health bar friendly", &cfg->m_visuals.bHealthFriendly );

				ImGui::Checkbox( "Name", &cfg->m_visuals.bName ); ImGui::SameLine( ); color_edit( "##name_clr", &cfg->m_visuals.iNameColor );
				ImGui::Checkbox( "Name friendly", &cfg->m_visuals.bNameFriendly );

				ImGui::Checkbox( "Weapon", &cfg->m_visuals.bWeapon );
				ImGui::Checkbox( "Weapon friendly", &cfg->m_visuals.bWeaponFriendly );
				if ( cfg->m_visuals.bWeapon ) {

					ImGui::Checkbox( "Weapon ammo", &cfg->m_visuals.bWeaponAmmo ); ImGui::SameLine( ); color_edit( "##ammo_clr", &cfg->m_visuals.iWeaponAmmoColor );

					ImGui::Checkbox( "Weapon text", &cfg->m_visuals.bWeaponText );
					ImGui::Checkbox( "Weapon icon", &cfg->m_visuals.bWeaponIcon );
					//ImGui::Checkbox( "", &cfg->m_visuals. );
					//ImGui::Checkbox( "", &cfg->m_visuals. );

				}

				ImGui::Checkbox( "Remove Scope", &cfg->m_visuals.bDisableScope );

				ImGui::Checkbox( "Night", &cfg->m_visuals.bNightMode );
				ImGui::Checkbox( "Transparent props", &cfg->m_visuals.bTransparentProps );

				ImGui::Checkbox( "Hitmarker", &cfg->m_visuals.bHitMarker );


				ImGui::Checkbox( "Remove smoke", &cfg->m_visuals.bRemoveSmoke );

				ImGui::Checkbox( "Projectile", &cfg->m_visuals.bProjectile );

				ImGui::Checkbox( "Skeleton", &cfg->m_visuals.bSkeleton );
				ImGui::Checkbox( "Skeleton friendly", &cfg->m_visuals.bSkeletonFriendly );




				ImGui::Combo("player models type", &cfg->m_models.m_player_models_type, player_models_type, ARRAYSIZE(player_models_type));
				ImGui::Checkbox("player models chams", &cfg->m_models.m_player_models);
				ImGui::ColorEdit4("player models chams color", cfg->m_models.m_player_models_clr);
				ImGui::Checkbox("player models chams occluded", &cfg->m_models.m_player_models_occluded);
				ImGui::ColorEdit4("player models chams occluded color", cfg->m_models.m_player_models_occluded_clr);



				break;
			case 3:

				ImGui::Checkbox("bhop", &cfg->m_move.m_bhop);
				ImGui::Checkbox("anti-untrusted", &cfg->bMiscAntiUntrusted);

				break;
			case 4:


				break;
			case 5:

				break;
			}


		}
		ImGui::End();










	}

	void init() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(input::m_hwnd);
		ImGui_ImplDX9_Init(interfaces::m_d3d_device);

		//ImGui::GetStyle().WindowMinSize = ImVec2(500, 500);

		auto& io = ImGui::GetIO();
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	}

	void undo() {
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}
