#include "btn_panel.h"

#include "../../consts.h"
#include "../../input_map.h"

const Vec2 BTN_PANEL_SZ = vec2_new(67, 11);

BtnPanel btn_panel_new(Vec2 pos) {
	BtnPanel btn_panel;
	
	btn_panel.pos = pos;

	btn_panel.undo_btn = btn_new(
		vec2_new(0, 0),
		vec2_new(12, 12),
		BTN_TEXTAREA_COLOR,
		"<"
	);

	btn_panel.redo_btn = btn_new(
		vec2_new(12, 0),
		vec2_new(12, 12),
		BTN_TEXTAREA_COLOR,
		">"
	);

	btn_panel.zoom_out_btn = btn_new(
		vec2_new(28, 0),
		vec2_new(12, 12),
		BTN_TEXTAREA_COLOR,
		"ICON_MINUS"
	);

	btn_panel.zoom_in_btn = btn_new(
		vec2_new(40, 0),
		vec2_new(12, 12),
		BTN_TEXTAREA_COLOR,
		"ICON_PLUS"
	);

	btn_panel.zoom_0_btn = btn_new(
		vec2_new(56, 0),
		vec2_new(12, 12),
		BTN_TEXTAREA_COLOR,
		"0"
	);

	return btn_panel;
}

void btn_panel_update(BtnPanel &btn_panel, const GraphicStuff &gs,
const Input &input, Vec2 parent_pos, bool show) {
	Vec2 pos = vec2_add(parent_pos, btn_panel.pos);
	btn_update(btn_panel.undo_btn, gs, input, pos, show);
	btn_update(btn_panel.redo_btn, gs, input, pos, show);
	btn_update(btn_panel.zoom_out_btn, gs, input, pos, show);
	btn_update(btn_panel.zoom_in_btn, gs, input, pos, show);
	btn_update(btn_panel.zoom_0_btn, gs, input, pos, show);
}

void btn_panel_draw(const BtnPanel &btn_panel, GraphicStuff &gs,
const Input &input, Vec2 parent_pos) {
	Vec2 pos = vec2_add(parent_pos, btn_panel.pos);
	btn_draw(btn_panel.undo_btn, gs, pos, map_down(input, MAP_UNDO));
	btn_draw(btn_panel.redo_btn, gs, pos, map_down(input, MAP_REDO)
	                                   || map_down(input, MAP_REDO_1));
	btn_draw(btn_panel.zoom_out_btn, gs, pos, map_down(input, MAP_ZOOM_OUT));
	btn_draw(btn_panel.zoom_in_btn, gs, pos, map_down(input, MAP_ZOOM_IN));
	btn_draw(btn_panel.zoom_0_btn, gs, pos, map_down(input, MAP_ZOOM_0));
}
