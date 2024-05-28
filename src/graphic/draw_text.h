#ifndef GRAPHIC__DRAW_TEXT_H
#define GRAPHIC__DRAW_TEXT_H

#include <string>

struct GraphicStuff;
struct Vec2;
struct Vec2i;
struct Color;

void draw_text(const GraphicStuff &gs, const std::string &text,
Vec2i fb_sz, Vec2 pos, Vec2 box_sz, int scale, Color color);

#endif