#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace sf;

class Play_audio
{
private:
	RectangleShape timestamp_bar; //A rectangle that represents the audio progress bar
	RectangleShape timestamp_bar_max; //A rectangle that represents the background of the timestamp_bar
	RectangleShape volume_bar; // A rectangle that represents the volume bar
	RectangleShape volume_cover;

	Texture play_button_texture;
	Texture next_button_texture;
	Texture prev_button_texture;
	Texture mode_button_texture;
	Sprite play_button;
	Sprite next_button;
	Sprite prev_button;
	Sprite mode_button;

	Font font;
	//The file_info_text is used to render the each file's info to the screen
	Text file_info_text;
	Text volume_icons_text[2];
	Text volume_percentage_text;

	wstring file_name;
	int final_file_size;
	int remain_minutes;
	int remain_seconds;
	int current_audio;
	float music_offset;

	Clock click_delay_clock;

public:
	int current_mode = 0;
	string looping_mode[4] = { "none", "replay", "random", "cycle" };

	Play_audio();
	void init_assets(filesystem::path file_assets);
	void update_colors(int& theme, float(&background_color)[4][3], float(&bounds_color)[4][3], float(&text_color)[4][3]);
	void play_audio_from_args(bool& has_passed, filesystem::path& passed_audio_from_args, Music& music, SoundBuffer& soundBuffer);
	void play_audio_from_current_dir(wstring& search_bar_wstring, vector<Text>& found_files_vector_text, RectangleShape& cursor, Music& music, SoundBuffer& soundBuffer, vector<float>& file_size, RenderWindow& window, RectangleShape& view_bounds, bool is_options_menu_open);
	void control_time_stamp(RectangleShape& cursor, Music& music, SoundBuffer& soundBuffer, bool& is_clicked, RenderWindow& window, wstring& search_bar_wstring, vector<Text>& found_files_vector_text, vector<float>& file_size, bool is_options_menu_open);
	void control_volume(RectangleShape& cursor, Music& music, SoundBuffer& soundBuffer, RenderWindow& window, bool is_options_menu_open);
	void control_looping(RectangleShape& cursor, Music& music, RenderWindow& window, wstring& search_bar_wstring, vector<Text>& found_files_vector_text, SoundBuffer& soundBuffer, vector<float>& file_size);
	void draw(RenderWindow& window);
};

