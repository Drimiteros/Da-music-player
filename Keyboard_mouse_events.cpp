#include "Keyboard_mouse_events.h"

Keyboard_mouse_events::Keyboard_mouse_events() {

}

void Keyboard_mouse_events::get_keyboard_input(Event& event, string& search_bar, bool& finished, RenderWindow& window) {

	if (window.hasFocus()) {
		if (event.type == Event::TextEntered && !Keyboard::isKeyPressed(Keyboard::Enter) && !Keyboard::isKeyPressed(Keyboard::BackSpace))
			search_bar += event.text.unicode;
		if (event.type == Event::TextEntered && !Keyboard::isKeyPressed(Keyboard::Enter) && Keyboard::isKeyPressed(Keyboard::BackSpace))
			search_bar = search_bar.substr(0, search_bar.size() - 1);
		if (event.type == Event::TextEntered && Keyboard::isKeyPressed(Keyboard::Enter) && !Keyboard::isKeyPressed(Keyboard::BackSpace))
			finished = false;
	}
}

void Keyboard_mouse_events::get_mouse_scroll(Event& event, int& scroll_value, RenderWindow& window) {

	if (window.hasFocus()) {
		if (event.type == Event::MouseWheelScrolled) {
			if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
				if (event.mouseWheelScroll.delta > 0)
					scroll_value++;
				if (event.mouseWheelScroll.delta < 0)
					scroll_value--;
			}
		}
	}
}

void Keyboard_mouse_events::get_mouse_click(Event& event, bool& is_clicked, RenderWindow& window) {

	if (window.hasFocus()) {
		if (event.type == Event::MouseButtonPressed) {
			if (Mouse::isButtonPressed(Mouse::Left))
				is_clicked = true;
		}
		else
			is_clicked = false;
	}
}