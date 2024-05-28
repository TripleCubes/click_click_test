#ifndef __EMSCRIPTEN__

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphic_types/graphic_types.h"
#include "graphic/graphic.h"
#include "game_time.h"
#include "input.h"
#include "mainloop.h"

#include "types/vec2i.h"

namespace {
const int INIT_W = 1000;
const int INIT_H = 600;

int window_w = INIT_W;
int window_h = INIT_H;

void on_resize(GLFWwindow *glfw_window, int w, int h) {
	window_w = w;
	window_h = h;
}

GLFWwindow * init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *glfw_window
		= glfwCreateWindow(INIT_W, INIT_H, "click click", NULL, NULL);
	if (glfw_window == NULL) {
		std::cout << "failed to create window" << std::endl;
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(glfw_window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to initialize glad" << std::endl;
		return NULL;
	}

	glViewport(0, 0, INIT_W, INIT_H);

	glfwSetFramebufferSizeCallback(glfw_window, on_resize);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return glfw_window;
}

}

int main () {
	GLFWwindow *glfw_window = init();
	if (glfw_window == NULL) {
		return 0;
	}

	float game_start_time = glfwGetTime();
	float frame_start_time = game_start_time;
	GameTime game_time;
	Input input;

	GraphicStuff graphic_stuff;
	graphic_stuff.current_window_sz.x = window_w;
	graphic_stuff.current_window_sz.y = window_h;
	graphic_stuff.px_scale = 2;
	if (!graphic_init(graphic_stuff)) {
		std::cout << "cant init graphic" << std::endl;
		return 0;
	}

	while (!glfwWindowShouldClose(glfw_window)) {
		game_time.delta = glfwGetTime() - frame_start_time;
		frame_start_time = glfwGetTime();
		game_time.time_since_start = frame_start_time - game_start_time;

		graphic_resize(graphic_stuff, vec2i_new(window_w, window_h));
		
		glfwPollEvents();

		double mouse_x;
		double mouse_y;
		glfwGetCursorPos(glfw_window, &mouse_x, &mouse_y);
		input.mouse_pos.x = mouse_x;
		input.mouse_pos.y = mouse_y;

		update(graphic_stuff, game_time, input);

		draw(graphic_stuff, game_time);
		
		glfwSwapBuffers(glfw_window);
	}

	glfwTerminate();
	std::cout << "reached end of main" << std::endl;

	return 0;
}

#endif