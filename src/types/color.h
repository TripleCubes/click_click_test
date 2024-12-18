#ifndef TYPES__COLOR_H
#define TYPES__COLOR_H

#include <string>

struct Color {
	float r = 1;
	float g = 1;
	float b = 1;
	float a = 1;
};

Color color_new(float r, float g, float b, float a);
Color color_add(Color color_a, Color color_b);
Color color_sub(Color color_a, Color color_b);
Color hue_to_rgb(float hue);
Color hsv_to_rgb(Color hsv);
Color rgb_to_hsv(Color rgb);
std::string color_to_hex(Color color);
bool is_valid_hex(const std::string &hex);
Color hex_to_color(const std::string &hex);

#endif
