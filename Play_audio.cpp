#include "Play_audio.h"

Play_audio::Play_audio() {
	timestamp_bar.setSize(Vector2f(1000, 20));
	timestamp_bar.setPosition(5, 745);
	timestamp_bar.setFillColor(Color(255, 207, 150));

	timestamp_bar_max.setPosition(5, 745);
	timestamp_bar_max.setSize(Vector2f(985, 20));
	timestamp_bar_max.setOutlineColor(Color::White);
	timestamp_bar_max.setOutlineThickness(1);

	file_info_text.setCharacterSize(15);
	file_info_text.setFont(font);
	file_info_text.setPosition(5, 660);
	file_info_text.setString("Now playing:\nFile size:\nDuration:"); //Placeholder string

	for (int i = 0; i < 2; i++) {
		volume_icons_text[i].setCharacterSize(20);
		volume_icons_text[i].setFont(font);
	}
	volume_icons_text[0].setPosition(810, 685);
	volume_icons_text[0].setString("-");
	volume_icons_text[1].setPosition(970, 685);
	volume_icons_text[1].setString("+");
	volume_percentage_text.setFont(font);
	volume_percentage_text.setCharacterSize(20);

	play_button.setScale(2, 2);
	play_button.setTextureRect(IntRect(0, 0, 20, 20));
	play_button.setPosition(995 / 2 - 30, 770 / 2 + 290);

	next_button.setScale(2, 2);
	next_button.setTextureRect(IntRect(0, 0, 30, 20));
	next_button.setPosition(995 / 2 + 40, 770 / 2 + 290);

	prev_button.setScale(2, 2);
	prev_button.setTextureRect(IntRect(30, 0, 30, 20));
	prev_button.setPosition(995 / 2 - 120, 770 / 2 + 290);

	mode_button.setScale(2, 2);
	mode_button.setTextureRect(IntRect(0, 0, 20, 20));
	mode_button.setPosition(995 / 2 + 130, 770 / 2 + 290);

	volume_bar.setSize(Vector2f(129, 34));
	volume_bar.setPosition(830, 680);
	volume_bar.setOutlineColor(Color::White);
	volume_bar.setOutlineThickness(1);
	volume_cover.setSize(Vector2f(129, 34));
	volume_cover.setPosition(830, 680);

	srand(time(0));
}

void Play_audio::init_assets(filesystem::path file_assets) {
	play_button_texture.loadFromFile((file_assets / "Textures/play.png").string());
	next_button_texture.loadFromFile((file_assets / "Textures/next.png").string());
	prev_button_texture.loadFromFile((file_assets / "Textures/prev.png").string());
	mode_button_texture.loadFromFile((file_assets / "Textures/mode.png").string());
	play_button.setTexture(play_button_texture);
	next_button.setTexture(next_button_texture);
	prev_button.setTexture(prev_button_texture);
	mode_button.setTexture(mode_button_texture);

	font.loadFromFile((file_assets / "Fonts/font.ttf").string());
}

void Play_audio::update_colors(int& theme, float(&background_color)[4][3], float(&bounds_color)[4][3], float(&text_color)[4][3]) {

	file_info_text.setFillColor(Color(text_color[theme][0], text_color[theme][1], text_color[theme][2]));
	timestamp_bar.setFillColor(Color(text_color[theme][0], text_color[theme][1], text_color[theme][2]));
	timestamp_bar_max.setFillColor(Color(bounds_color[theme][0], bounds_color[theme][1], bounds_color[theme][2]));
	volume_bar.setFillColor(Color(bounds_color[theme][0], bounds_color[theme][1], bounds_color[theme][2]));
	volume_percentage_text.setOutlineColor(Color(bounds_color[theme][0], bounds_color[theme][1], bounds_color[theme][2]));
	volume_icons_text[0].setOutlineColor(Color(bounds_color[theme][0], bounds_color[theme][1], bounds_color[theme][2]));
	volume_icons_text[1].setOutlineColor(Color(bounds_color[theme][0], bounds_color[theme][1], bounds_color[theme][2]));
}

void Play_audio::play_audio_from_args(bool& has_passed, filesystem::path& passed_audio_from_args, Music& music, SoundBuffer& soundBuffer) {
	
	if (has_passed) {
		soundBuffer.loadFromFile(passed_audio_from_args.string());
		music.openFromFile(passed_audio_from_args.string());
		music.play();
		file_name = passed_audio_from_args.filename().wstring();
		final_file_size = filesystem::file_size(passed_audio_from_args);
		has_passed = false;
	}
}

void Play_audio::play_audio_from_current_dir(wstring& search_bar_wstring, vector<Text>& found_files_vector_text, RectangleShape& cursor, Music& music, SoundBuffer& soundBuffer,
	vector<float>& file_size, RenderWindow& window, RectangleShape& view_bounds, bool is_options_menu_open) {
	
	if (window.hasFocus() && !is_options_menu_open) {
		for (int i = 0; i < found_files_vector_text.size(); i++) {
			if (cursor.getGlobalBounds().intersects(found_files_vector_text[i].getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)
				&& cursor.getGlobalBounds().intersects(view_bounds.getGlobalBounds())) {
				if (search_bar_wstring[search_bar_wstring.size()] == '\\' && search_bar_wstring[search_bar_wstring.size()] == '/') {
					music.openFromFile(search_bar_wstring + found_files_vector_text[i].getString());
					soundBuffer.loadFromFile(search_bar_wstring + found_files_vector_text[i].getString());
					current_audio = i;
				}
				else {
					music.openFromFile(search_bar_wstring + "\\" + found_files_vector_text[i].getString());
					soundBuffer.loadFromFile(search_bar_wstring + "\\" + found_files_vector_text[i].getString());
					current_audio = i;
				}
				file_name = found_files_vector_text[i].getString();
				final_file_size = file_size[i];
				music.play();
			}
		}
	}
}

void Play_audio::control_time_stamp(RectangleShape& cursor, Music& music, SoundBuffer& soundBuffer, bool& is_clicked, RenderWindow& window, wstring& search_bar_wstring, vector<Text>& found_files_vector_text,
	vector<float>& file_size, bool is_options_menu_open) {
	
	if (window.hasFocus() && !is_options_menu_open) {
		timestamp_bar.setSize(Vector2f(((music.getPlayingOffset().asSeconds() * 1) / music.getDuration().asSeconds()) * 985, 20));

		// Align audio with timestamp
		// When we click inside the timestamp_bar_max, we set the current progress of the audio.
		// We dynamically calculate the audio progress based on the cursor position relative to the position and size of timestamp_bar
		if (cursor.getGlobalBounds().intersects(timestamp_bar_max.getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
			float playing_offset = ((cursor.getPosition().x - timestamp_bar.getPosition().x) / timestamp_bar_max.getLocalBounds().width) * music.getDuration().asSeconds();
			music.setPlayingOffset(seconds(playing_offset));
		}

		// Play - Pause audio
		if (cursor.getGlobalBounds().intersects(play_button.getGlobalBounds())) {
			if (music.getStatus() == music.Playing)
				play_button.setTextureRect(IntRect(20, 20, 20, 20));
			else
				play_button.setTextureRect(IntRect(20, 0, 20, 20));
			if (is_clicked && music.getStatus() == music.Playing) {
				music.pause();
				is_clicked = false;
			}
			else if (is_clicked && music.getStatus() == music.Paused) {
				music.play();
				is_clicked = false;
			}
		}
		else
		{
			if (music.getStatus() == music.Playing)
				play_button.setTextureRect(IntRect(0, 20, 20, 20));
			else
				play_button.setTextureRect(IntRect(0, 0, 20, 20));
		}

		// Skip - Return to audio
		if (cursor.getGlobalBounds().intersects(next_button.getGlobalBounds())) {
			next_button.setTextureRect(IntRect(30, 0, 30, 20));
			if (is_clicked && current_audio + 1 < found_files_vector_text.size()) {
				current_audio++;
				if (search_bar_wstring[search_bar_wstring.size()] == '\\' && search_bar_wstring[search_bar_wstring.size()] == '/') {
					music.openFromFile(search_bar_wstring + found_files_vector_text[current_audio].getString());
					soundBuffer.loadFromFile(search_bar_wstring + found_files_vector_text[current_audio].getString());
				}
				else {
					music.openFromFile(search_bar_wstring + "\\" + found_files_vector_text[current_audio].getString());
					soundBuffer.loadFromFile(search_bar_wstring + "\\" + found_files_vector_text[current_audio].getString());
				}
				file_name = found_files_vector_text[current_audio].getString();
				final_file_size = file_size[current_audio];
				music.play();
				is_clicked = false;
			}
		}
		else
			next_button.setTextureRect(IntRect(0, 0, 30, 20));
		if (cursor.getGlobalBounds().intersects(prev_button.getGlobalBounds())) {
			prev_button.setTextureRect(IntRect(0, 0, 30, 20));
			if (is_clicked && current_audio - 1 >= 0) {
				current_audio--;
				if (search_bar_wstring[search_bar_wstring.size()] == '\\' && search_bar_wstring[search_bar_wstring.size()] == '/') {
					music.openFromFile(search_bar_wstring + found_files_vector_text[current_audio].getString());
					soundBuffer.loadFromFile(search_bar_wstring + found_files_vector_text[current_audio].getString());
				}
				else {
					music.openFromFile(search_bar_wstring + "\\" + found_files_vector_text[current_audio].getString());
					soundBuffer.loadFromFile(search_bar_wstring + "\\" + found_files_vector_text[current_audio].getString());
				}
				file_name = found_files_vector_text[current_audio].getString();
				final_file_size = file_size[current_audio];
				music.play();
				is_clicked = false;
			}
		}
		else
			prev_button.setTextureRect(IntRect(30, 0, 30, 20));
	}

	remain_minutes = (music.getDuration().asSeconds() - int(music.getPlayingOffset().asSeconds())) / 60;
	remain_seconds = (int(music.getDuration().asSeconds()) - int(music.getPlayingOffset().asSeconds())) % 60;
	file_info_text.setString(file_name.substr(0, 33) + "...\nFile size: " + to_string(final_file_size / 1048576).substr(0, 4) + " Mb\nTime: " + to_string(remain_minutes) + "m " + to_string(remain_seconds) + "s");
}

void Play_audio::control_volume(RectangleShape& cursor, Music& music, SoundBuffer& soundBuffer, RenderWindow& window, bool is_options_menu_open) {

	if (window.hasFocus() && !is_options_menu_open) {
		if (cursor.getGlobalBounds().intersects(volume_cover.getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
			volume_bar.setSize(Vector2f(cursor.getPosition().x - volume_bar.getPosition().x, 34));
			if (volume_bar.getSize().x < 0)
				volume_bar.setSize(Vector2f(0, 34));
			if ((cursor.getPosition().x - volume_bar.getPosition().x) * 100 / 129 < 0)
				music.setVolume(0);
			else
				music.setVolume((cursor.getPosition().x - volume_bar.getPosition().x) * 100 / 129);
			if (music.getVolume() > 96) {
				music.setVolume(100);
				volume_bar.setSize(Vector2f(129, 34));
			}
		}
		volume_percentage_text.setString(to_string(int(music.getVolume())) + "%");
		volume_percentage_text.setOrigin(volume_percentage_text.getLocalBounds().width / 2, 0);
		volume_percentage_text.setPosition(volume_bar.getPosition().x + 70, volume_bar.getPosition().y + 5);
	}
}

void Play_audio::control_looping(RectangleShape& cursor, Music& music, RenderWindow& window, wstring& search_bar_wstring, vector<Text>& found_files_vector_text, SoundBuffer& soundBuffer, vector<float>& file_size) {

	// Change looping mode
	if (cursor.getGlobalBounds().intersects(mode_button.getGlobalBounds()))
		mode_button.setTextureRect(IntRect(20, current_mode * 20, 20, 20));
	else
		mode_button.setTextureRect(IntRect(0, current_mode * 20, 20, 20));
	if (cursor.getGlobalBounds().intersects(mode_button.getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)
		&& click_delay_clock.getElapsedTime().asSeconds() > 0.3) {
		if (current_mode < 3)
			current_mode++;
		else
			current_mode = 0;
		cout << looping_mode[current_mode] << endl;
		click_delay_clock.restart();
	}

	if (looping_mode[current_mode] == "none")
		music.setLoop(false);
	if (looping_mode[current_mode] == "replay")
		music.setLoop(true);
	if (looping_mode[current_mode] == "random" || looping_mode[current_mode] == "cycle") {
		music.setLoop(false);
		if (music.getStatus() == music.Stopped) {
			int current_music = current_audio;
			if (looping_mode[current_mode] == "cycle")
				if (current_music < found_files_vector_text.size() - 1)
					current_music++;
				else
					current_music = 0;
			if (looping_mode[current_mode] == "random")
				current_music = rand() % found_files_vector_text.size() - 1;

			if (search_bar_wstring[search_bar_wstring.size()] == '\\' && search_bar_wstring[search_bar_wstring.size()] == '/') {
				music.openFromFile(search_bar_wstring + found_files_vector_text[current_music].getString());
				soundBuffer.loadFromFile(search_bar_wstring + found_files_vector_text[current_music].getString());
				current_audio = current_music;
			}
			else {
				music.openFromFile(search_bar_wstring + "\\" + found_files_vector_text[current_music].getString());
				soundBuffer.loadFromFile(search_bar_wstring + "\\" + found_files_vector_text[current_music].getString());
				current_audio = current_music;
			}
			file_name = found_files_vector_text[current_music].getString();
			final_file_size = file_size[current_music];
			music.play();
		}
	}
}

void Play_audio::draw(RenderWindow& window) {
	
	window.draw(timestamp_bar_max);
	window.draw(timestamp_bar);
	window.draw(play_button);
	window.draw(next_button);
	window.draw(prev_button);
	window.draw(mode_button);
	window.draw(file_info_text);
	window.draw(volume_bar);
	for (int i = 0; i < 2; i++)
		window.draw(volume_icons_text[i]);
	window.draw(volume_percentage_text);
}