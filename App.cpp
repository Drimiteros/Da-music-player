#include "App.h"

App::App(int argc, char* argv[]) : menu_bar(width, height) {

	// Load assets at the start of the app
	get_cmd_arguments.find_assets_path(argc, argv, assets_path);
	// Load potential arguments at the start of the app
	get_cmd_arguments.get_Audio_Path_From_Args(argc, argv, has_passed, passed_audio_from_args);
	// Load the latest path the user entered 
	preferences.load_preferences(assets_path, search_bar_wstring, theme, performance_mode, view_waveform);
	// Update default values for the menu bar
	menu_bar.update_defaults(assets_path, height);

	// Initialize window and its properties
	window.create(VideoMode(width, height), version, Style::Close);
	window_icon.loadFromFile((assets_path / "Textures/logo.png").string());
	window.setIcon(window_icon.getSize().x, window_icon.getSize().y, window_icon.getPixelsPtr());

	// Initialize UI 
	play_audio.init_assets(assets_path);
	cursor.setSize(Vector2f(5, 5));
	font.loadFromFile((assets_path / "Fonts/font.ttf").string());
	search_bar_text.setFont(font);
	search_bar_text.setCharacterSize(15);
	search_bar_text.setPosition(5, 5);
	search_bar_text.setFillColor(Color(255, 207, 150));
	found_files_text.setFont(font);
	found_files_text.setCharacterSize(15);
	found_files_text.setFillColor(Color(155, 155, 155));
	caret.setSize(Vector2f(15, 2));
	caret.setFillColor(Color(255, 207, 150));
	search_bar_bounds.setSize(Vector2f(995, 26));
	view_bounds.setSize(Vector2f(995, 595));
	view_bounds.setPosition(0, 45);
}

void App::events() {

	while (window.pollEvent(event)) {
		if (event.type == Event::Closed)
			window.close();
		keyboard_mouse_events.get_keyboard_input(event, search_bar_wstring, finished, window);
		keyboard_mouse_events.get_mouse_scroll(event, scroll_value, window);
		keyboard_mouse_events.get_mouse_click(event, is_clicked, window);
	}
}

void App::main_loop() {

	while (window.isOpen()) {
		
		events();

		update();

		window.clear(Color(background_color[theme][0], background_color[theme][1], background_color[theme][2]));

		draw();

		window.display();
	}

	// Save preferences
	Preferences preferences;
	preferences.save_preferences(assets_path, search_bar_wstring, theme, performance_mode, view_waveform);
}

void App::update() {
	
	if (!performance_mode)
		window.setFramerateLimit(99999);
	else
		window.setFramerateLimit(60);

	// Update colors
	view_bounds.setFillColor(Color(bounds_color[theme][0], bounds_color[theme][1], bounds_color[theme][2]));
	search_bar_bounds.setFillColor(Color(bounds_color[theme][0], bounds_color[theme][1], bounds_color[theme][2]));

	// Update the physical cursor's position to match the actual cursor's position
	cursor.setPosition(window.mapPixelToCoords(Mouse::getPosition(window)));

	// Play sound that was passed from cmd arguments
	play_audio.play_audio_from_args(has_passed, passed_audio_from_args, music, soundBuffer);
	play_audio.play_audio_from_current_dir(search_bar_wstring, found_files_vector_text, cursor, music, soundBuffer, file_size, window, view_bounds, is_options_menu_open);
	play_audio.control_time_stamp(cursor, music, soundBuffer, is_clicked, window, search_bar_wstring, found_files_vector_text, file_size, is_options_menu_open);
	play_audio.control_volume(cursor, music, soundBuffer, window, is_options_menu_open);
	play_audio.update_colors(theme, background_color, bounds_color, text_color);
	play_audio.control_looping(cursor, music, window, search_bar_wstring, found_files_vector_text, soundBuffer, file_size);

	// Set the string for search bar and update caret
	search_bar_text.setString(search_bar_wstring);
	if (clock.getElapsedTime().asSeconds() >= 0.5 && clock.getElapsedTime().asSeconds() <= 1) 
		caret.setFillColor(Color::Transparent);
	else if (clock.getElapsedTime().asSeconds() > 1) {
		caret.setFillColor(Color(255, 207, 150));
		clock.restart();
	}
	caret.setPosition(search_bar_text.getPosition().x + search_bar_text.getLocalBounds().width + 1, search_bar_text.getPosition().y + 15);

	//Search the files inside the current directory
	search_directory.search_current_directory(search_bar_wstring, found_files_vector_text, found_files_text, finished, file_size);

	waveform.waveform_logic(music, soundBuffer, theme, background_color, view_waveform);

	menu_bar.select_category(cursor, is_options_menu_open, is_about_menu_open);
	menu_bar.options(is_options_menu_open, cursor, theme, performance_mode, view_waveform);
	menu_bar.update_colors(theme, background_color, bounds_color, text_color);
}

void App::draw() {

	window.draw(view_bounds);
	waveform.draw(window, view_waveform);
	window.draw(search_bar_bounds);
	window.draw(search_bar_text);
	window.draw(caret);
	search_directory.draw(window, found_files_vector_text, scroll_value, cursor, view_bounds, is_options_menu_open);
	play_audio.draw(window);
	menu_bar.draw(window, is_options_menu_open, is_about_menu_open);
}