#include "Play_audio.h"

Play_audio::Play_audio() {
	timestamp_bar.setSize(Vector2f(1000, 20));
	timestamp_bar.setPosition(5, 745);
	timestamp_bar.setFillColor(Color(255, 207, 150));

	timestamp_bar_max.setPosition(5, 745);
	timestamp_bar_max.setFillColor(Color(31, 34, 44));
	timestamp_bar_max.setSize(Vector2f(985, 20));

	file_info_text.setCharacterSize(15);
	file_info_text.setFont(font);
	file_info_text.setPosition(5, 660);
	file_info_text.setFillColor(Color(131, 134, 144));
	file_info_text.setString("Now playing:\nFile size:\nDuration:"); //Placeholder string

	play_button.setScale(2, 2);
	play_button.setTextureRect(IntRect(0, 0, 20, 20));
	play_button.setPosition(995 / 2 - 30, 770 / 2 + 290);

	next_button.setScale(2, 2);
	next_button.setTextureRect(IntRect(0, 0, 30, 20));
	next_button.setPosition(995 / 2 + 40, 770 / 2 + 290);

	prev_button.setScale(2, 2);
	prev_button.setTextureRect(IntRect(30, 0, 30, 20));
	prev_button.setPosition(995 / 2 - 120, 770 / 2 + 290);

	volume_cover.setPosition(820, 650);
	volume_bar.setSize(Vector2f(129, 74));
	volume_bar.setFillColor(Color(131, 134, 144));
	volume_bar.setPosition(838, 650);
}

void Play_audio::init_assets(filesystem::path file_assets) {
	play_button_texture.loadFromFile((file_assets / "Textures/play.png").string());
	next_button_texture.loadFromFile((file_assets / "Textures/next.png").string());
	prev_button_texture.loadFromFile((file_assets / "Textures/prev.png").string());
	volume_cover_texture.loadFromFile((file_assets / "Textures/volume cover.png").string());
	play_button.setTexture(play_button_texture);
	next_button.setTexture(next_button_texture);
	prev_button.setTexture(prev_button_texture);
	volume_cover.setTexture(volume_cover_texture);

	font.loadFromFile((file_assets / "Fonts/font.ttf").string());
}

void Play_audio::play_audio_from_args(bool& has_passed, filesystem::path& passed_audio_from_args, Music& music, SoundBuffer& soundBuffer) {
	
	if (has_passed) {
		soundBuffer.loadFromFile(passed_audio_from_args.string());
		music.openFromFile(passed_audio_from_args.string());
		music.play();
		has_passed = false;
	}
}

void Play_audio::play_audio_from_current_dir(string& search_bar, vector<Text>& found_files_vector_text, RectangleShape& cursor, Music& music, SoundBuffer& soundBuffer, 
	vector<float>& file_size, RenderWindow& window, RectangleShape& view_bounds) {
	
	if (window.hasFocus()) {
		for (int i = 0; i < found_files_vector_text.size(); i++) {
			if (cursor.getGlobalBounds().intersects(found_files_vector_text[i].getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)
				&& cursor.getGlobalBounds().intersects(view_bounds.getGlobalBounds())) {
				if (search_bar[search_bar.size()] == '\\' && search_bar[search_bar.size()] == '/') {
					music.openFromFile(search_bar + found_files_vector_text[i].getString());
					soundBuffer.loadFromFile(search_bar + found_files_vector_text[i].getString());
					current_audio = i;
				}
				else {
					music.openFromFile(search_bar + "\\" + found_files_vector_text[i].getString());
					soundBuffer.loadFromFile(search_bar + "\\" + found_files_vector_text[i].getString());
					current_audio = i;
				}
				file_name = found_files_vector_text[i].getString().toAnsiString();
				final_file_size = file_size[i];
				music.play();
			}
		}
	}
}

void Play_audio::control_time_stamp(RectangleShape& cursor, Music& music, SoundBuffer& soundBuffer, bool& is_clicked, RenderWindow& window, string& search_bar, vector<Text>& found_files_vector_text,
	vector<float>& file_size) {
	
	if (window.hasFocus()) {
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
			if (is_clicked && current_audio + 1 <= found_files_vector_text.size()) {
				current_audio++;
				if (search_bar[search_bar.size()] == '\\' && search_bar[search_bar.size()] == '/') {
					music.openFromFile(search_bar + found_files_vector_text[current_audio].getString());
					soundBuffer.loadFromFile(search_bar + found_files_vector_text[current_audio].getString());
				}
				else {
					music.openFromFile(search_bar + "\\" + found_files_vector_text[current_audio].getString());
					soundBuffer.loadFromFile(search_bar + "\\" + found_files_vector_text[current_audio].getString());
				}
				file_name = found_files_vector_text[current_audio].getString().toAnsiString();
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
				if (search_bar[search_bar.size()] == '\\' && search_bar[search_bar.size()] == '/') {
					music.openFromFile(search_bar + found_files_vector_text[current_audio].getString());
					soundBuffer.loadFromFile(search_bar + found_files_vector_text[current_audio].getString());
				}
				else {
					music.openFromFile(search_bar + "\\" + found_files_vector_text[current_audio].getString());
					soundBuffer.loadFromFile(search_bar + "\\" + found_files_vector_text[current_audio].getString());
				}
				file_name = found_files_vector_text[current_audio].getString().toAnsiString();
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

void Play_audio::control_volume(RectangleShape& cursor, Music& music, SoundBuffer& soundBuffer, RenderWindow& window) {

	if (window.hasFocus()) {
		if (cursor.getGlobalBounds().intersects(volume_cover.getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
			volume_bar.setSize(Vector2f(cursor.getPosition().x - volume_bar.getPosition().x, 74));
			music.setVolume((cursor.getPosition().x - volume_bar.getPosition().x) * 100 / 129);
		}
	}
}

void Play_audio::draw(RenderWindow& window) {
	
	window.draw(timestamp_bar_max);
	window.draw(timestamp_bar);
	window.draw(play_button);
	window.draw(next_button);
	window.draw(prev_button);
	window.draw(file_info_text);
	window.draw(volume_bar);
	window.draw(volume_cover);
}