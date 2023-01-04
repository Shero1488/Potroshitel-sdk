#pragma once
#include "../../globals.h"
#include "fonts/roboto_medium.h"
#include "fonts/weapon_csgo.h"

namespace render {
	inline std::mutex m_render_mutex{ };

	static ImU32 get_u32( col_t color ) {
		return ( ( color.a( ) & 0xff ) << 24 ) + ( ( color.b( ) & 0xff ) << 16 ) + ( ( color.g( ) & 0xff ) << 8 ) + ( color.r( ) & 0xff );
	}

	namespace fonts {
		inline ImFont* m_esp{};
		inline ImFont* m_weapon{};
	}

	void rect(float x1, float y1, float x2, float y2, col_t color, float thickness = 1.f, float rounding = 0.f);
	void rect_filled(float x1, float y1, float x2, float y2, col_t color);
	void rect_outlined(float x1, float y1, float x2, float y2, col_t color, col_t color2);

	void circle_2d(vec3_t position, int point_count, float radius, col_t color);
	void circle_2d_filled(vec3_t position, int point_count, float radius, col_t color);
	void circle_3d(vec3_t position, int point_count, float radius, col_t color);
	void circle_3d_filled(const vec3_t& origin, float radius, col_t color);
	void line(float x1, float y1, float x2, float y2, col_t color, float thickness);
	void triangle(ImVec2 first, ImVec2 second, ImVec2 third, col_t color);
	void text(const ImFont* font, ImVec2 text_position, const std::string& text, col_t color, bool centered,
		bool outline = true,
		bool menu = false);
	void init() noexcept;
	void begin();
	void end();
	void clear_draw_list();
	ImDrawList* render_scene();
	bool world_to_screen( const vec3_t& world, vec2_t& screen );

	inline ImDrawList* m_draw_list_act{};
	inline ImDrawList* m_draw_list_rendering{};
	inline ImDrawList* m_draw_list{};
	inline ImDrawData m_draw_data{};

}

