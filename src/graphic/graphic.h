#ifndef GRAPHIC__GRAPHIC_H
#define GRAPHIC__GRAPHIC_H

struct GraphicStuff;

const int MESH_COLOR_RECT = 0;
const int MESH_TEXTURE_RECT = 0;

const int SHADER_COLOR_RECT = 0;
const int SHADER_TEXTURE_RECT = 1;

const int FRAMEBUFFER_MAIN = 0;

const int TEXTURE_FONT = 0;

bool graphic_init(GraphicStuff &graphic_stuff);

#endif
