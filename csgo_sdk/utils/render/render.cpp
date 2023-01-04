#include "render.h"

void render::init() noexcept {
	ImGui::CreateContext();
	m_draw_list = new ImDrawList(ImGui::GetDrawListSharedData());
	m_draw_list_act = new ImDrawList(ImGui::GetDrawListSharedData());
	m_draw_list_rendering = new ImDrawList(ImGui::GetDrawListSharedData());

	ImFontConfig config;
	config.PixelSnapH = false;
	config.OversampleH = 5;
	config.OversampleV = 5;
	config.RasterizerMultiply = 1.2f;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
		0x2DE0, 0x2DFF, // Cyrillic Extended-A
		0xA640, 0xA69F, // Cyrillic Extended-B
		0xE000, 0xE226, // icons
		0,
	};

	config.GlyphRanges = ranges;

	fonts::m_esp = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		roboto_medium_compressed_data,
		roboto_medium_compressed_size,
		12.f,
		&config, ranges);

	fonts::m_weapon = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		weapon_icons_compressed_data,
		weapon_icons_compressed_size,
		36.f,
		&config, ranges);

	ImGui::GetIO().Fonts->AddFontDefault();
	ImGuiFreeType::BuildFontAtlas(ImGui::GetIO().Fonts, 0x00);

	interfaces::m_engine->get_screen_size( globals::m_width, globals::m_height );
}

void render::begin() {
	m_draw_list->Clear();
	m_draw_list->PushClipRectFullScreen();
}

void render::end() {
	m_render_mutex.lock();
	*m_draw_list_act = *m_draw_list;
	m_render_mutex.unlock();
}

void render::clear_draw_list() {
	m_render_mutex.lock();
	m_draw_list_act->Clear();
	m_render_mutex.unlock();
}

bool render::world_to_screen( const vec3_t& world, vec2_t& screen ) {
	const v_matrix& matrix = interfaces::m_engine->world_to_screen_matrix( );

	// check if it's in view first.
	// note - dex; w is below 0 when world position is around -90 / +90 from the player's camera on the y axis.
	float w = matrix[ 3 ][ 0 ] * world.x + matrix[ 3 ][ 1 ] * world.y + matrix[ 3 ][ 2 ] * world.z + matrix[ 3 ][ 3 ];
	if ( w < 0.001f )
		return false;

	// calculate x and y.
	screen.x = matrix[ 0 ][ 0 ] * world.x + matrix[ 0 ][ 1 ] * world.y + matrix[ 0 ][ 2 ] * world.z + matrix[ 0 ][ 3 ];
	screen.y = matrix[ 1 ][ 0 ] * world.x + matrix[ 1 ][ 1 ] * world.y + matrix[ 1 ][ 2 ] * world.z + matrix[ 1 ][ 3 ];

	screen /= w;

	// calculate screen position.
	screen.x = ( globals::m_width / 2 ) + ( screen.x * globals::m_width ) / 2;
	screen.y = ( globals::m_height / 2 ) - ( screen.y * globals::m_height ) / 2;

	return true;
}

ImDrawList* render::render_scene() {

	if (m_render_mutex.try_lock()) {
		*m_draw_list_rendering = *m_draw_list_act;
		m_render_mutex.unlock();
	}

	return m_draw_list_rendering;
}

void render::rect(const float x1, const float y1, const float x2, const float y2, const col_t color, const float thickness, const float rounding) {
	m_draw_list->AddRect(ImVec2(x1, y1), ImVec2(x1 + x2, y1 + y2), get_u32(color), rounding, 15, thickness);
}

void render::rect_filled(const float x1, const float y1, const float x2, const float y2, const col_t color) {
	m_draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x1 + x2, y1 + y2), get_u32(color), 0.0f);
}

void render::rect_outlined(const float x1, const float y1, const float x2, const float y2, const col_t color, const col_t color2) {
	rect(x1, y1, x2, y2, color, 0.0f);
	rect(x1 - 1, y1 - 1, x2 + 2, y2 + 2, color2, 0.0f);
	rect(x1 + 1, y1 + 1, x2 - 2, y2 - 2, color2, 0.0f);
}

void render::circle_2d(const vec3_t position, const int point_count, const float radius, const col_t color) {
	auto screen_position = vec2_t(0, 0);
	if (world_to_screen(position, screen_position))
		return;

	m_draw_list->AddCircle(ImVec2(screen_position.x, screen_position.y), radius, get_u32(color), point_count);
}

void render::circle_2d_filled(const vec3_t position, const int point_count, const float radius, const col_t color) {
	auto screen_position = vec2_t(0, 0);
	if (world_to_screen(position, screen_position))
		return;

	m_draw_list->AddCircleFilled(ImVec2(screen_position.x, screen_position.y), radius, get_u32(color), point_count);
}

void render::circle_3d(const vec3_t position, const int point_count, const float radius, const col_t color) {
	const float fl_step = math::m_pi_float * 2.0f / static_cast<float>(point_count);
	for (float a = 0; a < math::m_pi_float * 2.0f; a += fl_step) {
		auto start = vec3_t(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
		auto end = vec3_t(radius * cosf(a + fl_step) + position.x, radius * sinf(a + fl_step) + position.y,
			position.z);

		vec2_t start_2d, end_2d;
		if (world_to_screen( start, start_2d )
			|| world_to_screen( end, end_2d ))
			continue;

		line(start_2d.x, start_2d.y, end_2d.x, end_2d.y, color, 1.0f);
	}
}

void render::circle_3d_filled(const vec3_t& origin, const float radius, const col_t color) {
	static auto previous_screen_pos = vec2_t(0, 0);
	static float step = math::m_pi_float * 2.0f / 72.0f;

	auto screen_position = vec2_t(0, 0);
	if (world_to_screen( origin, screen_position ) )
		return;

	for (float rotation = 0.0f; rotation <= math::m_pi_float * 2.0f; rotation += step) {
		auto world_position = vec3_t(radius * cos(rotation) + origin.x,
			radius * sin(rotation) + origin.y, origin.z);

		if (world_to_screen( world_position, screen_position ) )
			continue;

		line(previous_screen_pos.x, previous_screen_pos.y, screen_position.x, screen_position.y, color,
			1.0f);
		triangle
		(
			ImVec2(screen_position.x, screen_position.y),
			ImVec2(screen_position.x, screen_position.y),
			ImVec2(previous_screen_pos.x, previous_screen_pos.y),
			col_t(color.r(), color.g(), color.b(), color.a() / 2)
		);

		previous_screen_pos = screen_position;
	}
}

void render::line(const float x1, const float y1, const float x2, const float y2, const col_t color, const float thickness) {
	m_draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), get_u32(color), thickness);
}

void render::triangle(const ImVec2 first, const ImVec2 second, const ImVec2 third, const col_t color) {
	m_draw_list->AddTriangleFilled(first, second, third, get_u32(color));
}

void render::text(const ImFont* font, ImVec2 text_position, const std::string& text, const col_t color, const bool centered,
	const bool outline, const bool menu) {
	const ImVec2 im_text_size = font->CalcTextSizeA(font->FontSize, FLT_MAX, 0.0f, text.c_str());
	if (!font->ContainerAtlas)
		return;

	ImDrawList* old_draw_list = m_draw_list;
	if (menu)
		m_draw_list = ImGui::GetOverlayDrawList();

	m_draw_list->PushTextureID(font->ContainerAtlas->TexID);
	if (centered)
		text_position.x -= im_text_size.x / 2.0f;

	if (outline) {
		m_draw_list->AddText(font, font->FontSize, ImVec2(text_position.x + 1, text_position.y + 1),
			get_u32(col_t(30, 30, 36, color.a())), text.c_str());
		m_draw_list->AddText(font, font->FontSize, ImVec2(text_position.x - 1, text_position.y - 1),
			get_u32(col_t(30, 30, 36, color.a())), text.c_str());
		m_draw_list->AddText(font, font->FontSize, ImVec2(text_position.x + 1, text_position.y - 1),
			get_u32(col_t(30, 30, 36, color.a())), text.c_str());
		m_draw_list->AddText(font, font->FontSize, ImVec2(text_position.x - 1, text_position.y + 1),
			get_u32(col_t(30, 30, 36, color.a())), text.c_str());
	}

	m_draw_list->AddText(font, font->FontSize, text_position, get_u32(color), text.c_str());
	m_draw_list->PopTextureID();

	if (menu)
		m_draw_list = old_draw_list;
}