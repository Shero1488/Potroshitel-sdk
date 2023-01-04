#include "../hooks.h"

void __fastcall hooks::panel::hook(i_panel* vpanel, void* edx, const vgui::vpanel panel, const bool force_repaint, const bool allow_force) {
	static const auto original = m_panel->get_original< decltype(&hook) >( panel::index );

	static uint32_t hud_zoom_panel = 0;
	static vgui::vpanel panel_id = 0;

	// cache CHudZoom panel once.
	if (!hud_zoom_panel)
		if (!strcmp("HudZoom", interfaces::m_panel->get_name(panel)))
			hud_zoom_panel = panel;

	// cache tools panel once.
	if (!panel_id)
		if (!strcmp(interfaces::m_panel->get_name(panel), "MatSystemTopPanel"))
			panel_id = panel;

	// don't call the original function if we want to remove the scope.
	if (hud_zoom_panel == panel && cfg->m_visuals.bDisableScope)
		return;

	// render hack stuff.
	if (panel_id == panel)
	{
		render::begin();
		{
			g_visuals.think( );
		}
		render::end();
	}

	original(vpanel, edx, panel, force_repaint, allow_force);
}