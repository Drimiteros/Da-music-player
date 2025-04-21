#include "Menu_bar.h"

Menu_bar::Menu_bar(int width, int height) {

	menu_bar_rect.setSize(Vector2f(width, 40));
	menu_bar_rect.setPosition(0, height - 37);

	menu_bar_texts[0].setString("Options");
	menu_bar_texts[1].setString("Website");
	menu_bar_texts[2].setString("Feedback");
	menu_bar_texts[3].setString("Donate");

	background.setSize(Vector2f(width, height));
	background.setFillColor(Color(0, 0, 0, 150));

	menu_window.setSize(Vector2f(700, 500));
	menu_window.setOrigin(menu_window.getSize().x / 2, menu_window.getSize().y / 2 + 60);
	menu_window.setPosition(width / 2, height / 2);
	menu_window.setOutlineColor(Color::White);
	menu_window.setOutlineThickness(2);

	for (int i = 0; i < theme_count; i ++) {
		theme_circle[i].setRadius(15);
		theme_circle[i].setPointCount(60);
		theme_circle[i].setPosition(170 + (i * 40), 150);
	}
	theme_circle[0].setFillColor(Color(31, 34, 44));
	theme_circle[1].setFillColor(Color(252, 119, 3));
	theme_circle[2].setFillColor(Color(0, 132, 255));
	theme_circle[3].setFillColor(Color(0, 255, 132));

	for (int i = 0; i < menu_text_count; i++) {
		menu_text[i].setFont(font);
		menu_text[i].setCharacterSize(17);
		menu_text[i].setFillColor(Color(161, 164, 174));
	}
	menu_text[0].setString("Select theme: ");
	menu_text[0].setPosition(170, 110);
	menu_text[1].setString("Power saver mode: ");
	menu_text[1].setPosition(170, 210);
	menu_text[2].setString("Toggle waveform / performance mode: ");
	menu_text[2].setPosition(170, 300);

	performance_check_box.setSize(Vector2f(20, 20));
	performance_check_box.setPosition(170, 250);
	performance_check_box.setFillColor(Color(61, 64, 74));
	performance_check_box.setOutlineThickness(2);
	performance_check_box.setOutlineColor(Color::White);

	waveform_check_box.setSize(Vector2f(20, 20));
	waveform_check_box.setPosition(170, 340);
	waveform_check_box.setFillColor(Color(61, 64, 74));
	waveform_check_box.setOutlineThickness(2);
	waveform_check_box.setOutlineColor(Color::White);
}

void Menu_bar::update_defaults(filesystem::path assets_path, int height) {
	font.loadFromFile((assets_path / "Fonts/font.ttf").string());
	for (int i = 0; i < menu_bar_texts_count; i++) {
		menu_bar_texts[i].setFont(font);
		static float x = 5;
		menu_bar_texts[i].setCharacterSize(14);
		menu_bar_texts[i].setFillColor(Color(255, 205, 155));
		menu_bar_texts[i].setPosition(x, height - 25);
		x += menu_bar_texts[i].getLocalBounds().width + 20;
	}
}

void Menu_bar::update_colors(int& theme, float background_color[4][3], float bounds_color[4][3], float text_color[4][3]) {

	menu_bar_rect.setFillColor(Color(bounds_color[theme][0], bounds_color[theme][1], bounds_color[theme][2]));
	menu_window.setFillColor(Color(background_color[theme][0], background_color[theme][1], background_color[theme][2]));
	for (int i = 0; i < menu_text_count; i++)
		menu_text[i].setFillColor(Color(text_color[theme][0], text_color[theme][1], text_color[theme][2]));
}

void Menu_bar::select_category(RectangleShape& cursor, bool& is_options_menu_open, bool& is_about_menu_open) {

	for (int i = 0; i < menu_bar_texts_count; i++) {
		if (cursor.getGlobalBounds().intersects(menu_bar_texts[i].getGlobalBounds())) {
			menu_bar_texts[i].setFillColor(Color(255, 205, 155));
		}
		else {
			menu_bar_texts[i].setFillColor(Color::White);
		}
	}
	if (cursor.getGlobalBounds().intersects(menu_bar_texts[0].getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
		is_options_menu_open = true;
		is_about_menu_open = false;
	}
	if (cursor.getGlobalBounds().intersects(menu_bar_texts[1].getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
		is_options_menu_open = false;
		is_about_menu_open = true;
		system("start https://drimiteros.github.io/Da-Music-Player-Website/");
	}
	if (cursor.getGlobalBounds().intersects(menu_bar_texts[3].getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left))
		system("start https://www.paypal.com/donate/?hosted_button_id=78P55A9PQUYFU");
	if (cursor.getGlobalBounds().intersects(menu_bar_texts[2].getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left))
		system("start https://tally.so/r/w2o8Rp");

	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		is_options_menu_open = false;
		is_about_menu_open = false;
	}
}

void Menu_bar::options(bool& is_options_menu_open, RectangleShape& cursor, int& theme, bool& performance_mode, bool& view_waveform) {
	
	if (is_options_menu_open) {
		for (int i = 0; i < theme_count; i++) {
			if (cursor.getGlobalBounds().intersects(theme_circle[i].getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
				theme = i;
				break;
			}
		}

		if (cursor.getGlobalBounds().intersects(performance_check_box.getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left) && !performance_mode && click_delay_clock.getElapsedTime().asSeconds() > 0.3) {
			performance_mode = true;
			click_delay_clock.restart();
		}
		else if (cursor.getGlobalBounds().intersects(performance_check_box.getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left) && performance_mode && click_delay_clock.getElapsedTime().asSeconds() > 0.3) {
			performance_mode = false;	
			click_delay_clock.restart();
		}
		if (performance_mode)
			performance_check_box.setFillColor(Color::White);
		else if (!performance_mode)
			performance_check_box.setFillColor(Color(61, 64, 74));

		if (cursor.getGlobalBounds().intersects(waveform_check_box.getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left) && click_delay_clock.getElapsedTime().asSeconds() > 0.3 && view_waveform) {
			view_waveform = false;
			click_delay_clock.restart();
		}
		else if (cursor.getGlobalBounds().intersects(waveform_check_box.getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left) && click_delay_clock.getElapsedTime().asSeconds() > 0.3 && !view_waveform) {
			view_waveform = true;
			click_delay_clock.restart();
		}
		if (view_waveform)
			waveform_check_box.setFillColor(Color(61, 64, 74));
		else if (!view_waveform)
			waveform_check_box.setFillColor(Color::White);
	}
}

void Menu_bar::about(bool& is_about_menu_open, RectangleShape& cursor) {

	if (is_about_menu_open) {

	}
}

void Menu_bar::draw(RenderWindow& window, bool& is_options_menu_open, bool& is_about_menu_open) {

	if (is_options_menu_open == true) {
		window.draw(background);
		window.draw(menu_window);
		for (int i = 0; i < theme_count; i++) {
			window.draw(theme_circle[i]);
		}
		for (int i = 0; i < menu_text_count; i++) {
			window.draw(menu_text[i]);
		}
		window.draw(performance_check_box);
		window.draw(waveform_check_box);
	}
	window.draw(menu_bar_rect);
	for (int i = 0; i < menu_bar_texts_count; i++) {
		window.draw(menu_bar_texts[i]);
	}
}