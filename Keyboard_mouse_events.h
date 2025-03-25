#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Keyboard_mouse_events
{
private:

public:
	Keyboard_mouse_events();
	void get_keyboard_input(Event& event, string& search_bar, bool& finished, RenderWindow& window);
	void get_mouse_scroll(Event& event, int& scroll_value, RenderWindow& window);
	void get_mouse_click(Event& event, bool& is_clicked, RenderWindow& window);
};

