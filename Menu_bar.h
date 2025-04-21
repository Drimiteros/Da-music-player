#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <filesystem>

#define menu_bar_texts_count 4
#define theme_count 4
#define menu_text_count 3

using namespace std;
using namespace sf;

class Menu_bar
{
private:
	Font font;
	RectangleShape background;
	RectangleShape menu_bar_rect;
	RectangleShape menu_window;
	RectangleShape performance_check_box;
	RectangleShape waveform_check_box;
	Text menu_bar_texts[4];
	Text menu_text[menu_text_count];
	CircleShape theme_circle[theme_count];

public:

	Clock click_delay_clock;

	Menu_bar(int width, int height);
	void update_defaults(filesystem::path assets_path, int height);
	void update_colors(int& theme, float background_color[4][3], float bounds_color[4][3], float text_color[4][3]);
	void select_category(RectangleShape& cursor, bool& is_options_menu_open, bool& is_about_menu_open);
	void options(bool& is_options_menu_open, RectangleShape& cursor, int& theme, bool& performance_mode, bool& view_waveform);
	void about(bool& is_about_menu_open, RectangleShape& cursor);
	void draw(RenderWindow& window, bool& is_options_menu_open, bool& is_about_menu_open);
};

