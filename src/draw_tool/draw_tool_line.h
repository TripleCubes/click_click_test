#ifndef DRAW_TOOL__DRAW_TOOL_LINE_H
#define DRAW_TOOL__DRAW_TOOL_LINE_H

#include <vector>

struct Vec2;
struct Vec2i;
struct Selection;

bool draw_tool_line(std::vector<unsigned char> &data,
	const Selection &selection, Vec2i data_sz,
	Vec2 pos_1, Vec2 pos_2, unsigned char pallete_index, int sz);

#endif
