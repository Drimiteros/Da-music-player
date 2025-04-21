#include "Keyboard_mouse_events.h"

Keyboard_mouse_events::Keyboard_mouse_events() {

}

void Keyboard_mouse_events::get_keyboard_input(Event& event, wstring& search_bar_wstring, bool& finished, RenderWindow& window) {

	if (window.hasFocus()) {
		if (event.type == Event::TextEntered && !Keyboard::isKeyPressed(Keyboard::Enter) && !Keyboard::isKeyPressed(Keyboard::BackSpace) && !Keyboard::isKeyPressed(Keyboard::Escape))
			search_bar_wstring += event.text.unicode;
		if (event.type == Event::TextEntered && !Keyboard::isKeyPressed(Keyboard::Enter) && Keyboard::isKeyPressed(Keyboard::BackSpace) && !Keyboard::isKeyPressed(Keyboard::Escape))
			search_bar_wstring = search_bar_wstring.substr(0, search_bar_wstring.size() - 1);
		if (event.type == Event::TextEntered && Keyboard::isKeyPressed(Keyboard::Enter) && !Keyboard::isKeyPressed(Keyboard::BackSpace) && !Keyboard::isKeyPressed(Keyboard::Escape))
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