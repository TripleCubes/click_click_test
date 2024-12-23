#ifndef UI__APP_UI_H
#define UI__APP_UI_H

#include "tab_bar/tab_bar.h"
#include "file_picker/file_picker.h"
#include "new_tab_menu/new_tab_menu.h"
#include "resize_menu/resize_menu.h"
#include "top_left_menu/top_left_menu.h"
#include "app_menu/app_menu.h"
#include "dialog_box/dialog_box.h"

struct GraphicStuff;

struct AppUI {
	TabBar tab_bar;
	FilePicker file_picker;
	NewTabMenu new_tab_menu;
	ResizeMenu resize_menu;
	TopLeftMenu top_left_menu;
	AppMenu app_menu;

	DialogBox dialog_box;
};

void app_ui_init(AppUI &app_ui, GraphicStuff &gs);

void app_ui_release(AppUI &app_ui, GraphicStuff &gs);

#endif
