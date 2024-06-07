#include "tab.h"

#include "../types/color.h"

#include "../graphic_types/texture.h"
#include "../graphic_types/framebuffer.h"
#include "../graphic_types/shader.h"
#include "../graphic_types/mesh.h"
#include "../graphic/graphic.h"
#include "../graphic/draw_rect.h"
#include "../graphic/draw_texture.h"

#include "../graphic_types/graphic_types.h"
#include "../input.h"
#include "../pos_convert.h"
#include "../basic_math.h"

namespace {

int get_blank_index(const std::vector<Tab> &list) {
	for (int i = 0; i < (int)list.size(); i++) {
		if (!list[i].running) {
			return i;
		}
	}

	return -1;
}

void px(Tab &tab, Vec2i pos, float pallete_index) {
	int index = tab.sz.y * pos.y + pos.x;
	tab.draw_data[index] = pallete_index;
}

void pallete_data_color(Tab &tab, int pallete_index, Color color) {
	int index = pallete_index * 4;
	tab.pallete_data[index    ] = color.r * 255;
	tab.pallete_data[index + 1] = color.g * 255;
	tab.pallete_data[index + 2] = color.b * 255;
	tab.pallete_data[index + 3] = color.a * 255;
}

void draw_draw_texture(const Tab &tab, const GraphicStuff &gs,
Vec2 pos) {
	Vec2 main_fb_sz_f = to_vec2(fb_get_sz(gs, FRAMEBUFFER_MAIN));
	Vec2 sz_f = to_vec2(tab.sz);
	Vec2 pos_normalized = vec2_new(
		pos.x / main_fb_sz_f.x,
		pos.y / main_fb_sz_f.y
	);
	pos_normalized = vec2_mul(pos_normalized, 2);
	pos_normalized = vec2_add(pos_normalized, vec2_new(-1, -1));
	Vec2 sz_normalized = vec2_new(
		sz_f.x / main_fb_sz_f.x * 2 * tab.px_scale,
		sz_f.y / main_fb_sz_f.y * 2 * tab.px_scale
	);
	pos_normalized.y = - pos_normalized.y - sz_normalized.y;

	use_shader(gs, SHADER_TAB_DRAW);
	set_uniform_vec2(gs, SHADER_TAB_DRAW, "u_pos", pos_normalized);
	set_uniform_vec2(gs, SHADER_TAB_DRAW, "u_sz", sz_normalized);
	set_uniform_texture(gs, SHADER_TAB_DRAW,
		"u_draw_texture", 0, texture_get_id(gs, tab.draw_texture_index));
	set_uniform_texture(gs, SHADER_TAB_DRAW,
		"u_pallete_texture", 1, texture_get_id(gs, tab.pallete_texture_index));

	draw_mesh(gs, MESH_RECT);
}

}

int tab_new(std::vector<Tab> &tab_list, GraphicStuff &gs,
Vec2 pos, Vec2i sz, int px_scale) {
	int index = get_blank_index(tab_list);
	if (index == -1) {
		Tab new_tab;
		tab_list.push_back(new_tab);

		index = (int)tab_list.size() - 1;
	}
	Tab &tab = tab_list[index];

	tab.pos = pos;
	tab.sz = sz;
	tab.px_scale = px_scale;
	tab.draw_data.resize(sz.x * sz.y, 0);
	tab.pallete_data.resize(16 * 16, 1);
	tab.color_picker = color_picker_new(vec2_new(50, 10));
	tab.color_pallete = color_pallete_new(vec2_new(300, 10));

	tab.draw_texture_index = texture_blank_new_red(gs, sz.x, sz.y);
	tab.pallete_texture_index = texture_blank_new(gs, 16, 16);

	texture_data(gs, tab.pallete_texture_index,
		vec2i_new(16, 16), tab.pallete_data);

	return index;
}

void tab_update(Tab &tab, GraphicStuff &gs, const Input &input,
Vec2 parent_pos, bool show) {
	color_picker_update(tab.color_picker, gs, input, parent_pos, show);
	color_pallete_update(tab.color_pallete, gs, input, parent_pos, show);

	if (!show) {
		return;
	}

	Vec2 pos = vec2_add(parent_pos, tab.pos);
	
	int pallete_index = tab.color_pallete.selected_index;

	if (tab.color_picker.color_changed) {
		Color color = color_picker_get_rgb(tab.color_picker);
		tab.color_pallete.color_list[pallete_index] = color;

		pallete_data_color(tab, pallete_index, color);
		texture_data(gs, tab.pallete_texture_index,
			vec2i_new(16, 16), tab.pallete_data);
	}

	if (tab.color_pallete.selection_changed) {
		Color color = tab.color_pallete.color_list[pallete_index];
		color_picker_set_rgb(tab.color_picker, color);
	}


	Vec2 main_fb_mouse_pos = get_main_fb_mouse_pos(gs, input.mouse_pos);
	Vec2 main_fb_sz = to_vec2(get_main_fb_sz(gs));
	Vec2i tex_draw_mouse_pos
		= get_tex_draw_mouse_pos(tab, pos, main_fb_mouse_pos);

	if (!in_rect(main_fb_mouse_pos, vec2_new(0, 0), main_fb_sz)) {
		return;
	}

	if (!in_rect(to_vec2(tex_draw_mouse_pos),vec2_new(0, 0),to_vec2(tab.sz))) {
		return;
	}

	if ((input.left_down && input.mouse_move) || input.left_click) {
		px(tab, tex_draw_mouse_pos, pallete_index);
		texture_data_red(gs, tab.draw_texture_index, tab.sz, tab.draw_data);
	}
}

void tab_draw(const Tab &tab, const GraphicStuff &gs, const Input &input,
Vec2 parent_pos) {
	Vec2i main_fb_sz = fb_get_sz(gs, FRAMEBUFFER_MAIN);
	Vec2 pos = vec2_add(parent_pos, tab.pos);

	draw_draw_texture(tab, gs, pos);

	Vec2 main_fb_mouse_pos = get_main_fb_mouse_pos(gs, input.mouse_pos);
	Vec2i tex_draw_mouse_pos
		= get_tex_draw_mouse_pos(tab, pos, main_fb_mouse_pos);

	if (in_rect(to_vec2(tex_draw_mouse_pos),vec2_new(0, 0),to_vec2(tab.sz))) {
		draw_rect_sz(
			gs,
			main_fb_sz,
			vec2_new(
				floor2(main_fb_mouse_pos.x, tab.px_scale),
				floor2(main_fb_mouse_pos.y, tab.px_scale)
			),
			vec2_new(tab.px_scale, tab.px_scale),
			color_new(0, 0, 0, 1)
		);
	}

	color_picker_draw(tab.color_picker, gs, parent_pos);
	color_pallete_draw(tab.color_pallete, gs, parent_pos);
}

void tab_close(std::vector<Tab> &tab_list, GraphicStuff &gs, int index) {
	Tab &tab = tab_list[index];
	tab.draw_data.clear();
	tab.pallete_data.clear();
	texture_release(gs, tab.draw_texture_index);
	texture_release(gs, tab.pallete_texture_index);
	tab.running = false;
}
