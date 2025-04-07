#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>

using namespace std;
using namespace sf;

class Play_audio
{
private:
	RectangleShape timestamp_bar; //A rectangle that represents the audio progress bar
	RectangleShape timestamp_bar_max; //A rectangle that represents the background of the timestamp_bar
	RectangleShape volume_bar; // A rectangle that represents the volume bar
	Sprite volume_cover;

	Texture play_button_texture;
	Texture next_button_texture;
	Texture prev_button_texture;
	Texture volume_cover_texture;
	Sprite play_button;
	Sprite next_button;
	Sprite prev_button;

	Font font;
	//The file_info_text is used to render the each file's info to the screen
	Text file_info_text;

	string file_name;
	int final_file_size;
	int remain_minutes;
	int remain_seconds;
	int current_audio;
	float music_offset;

public:
	Play_audio();
	void init_assets(filesystem::path file_assets);
	void play_audio_from_args(bool& has_passed, filesystem::path& passed_audio_from_args, Music& music, SoundBuffer& soundBuffer);
	void play_audio_from_current_dir(string& search_bar, vector<Text>& found_files_vector_text, RectangleShape& cursor, Music& music, SoundBuffer& soundBuffer, vector<float>& file_size, 
		RenderWindow& window, RectangleShape& view_bounds);
	void control_time_stamp(RectangleShape& cursor, Music& music, SoundBuffer& soundBuffer, bool& is_clicked, RenderWindow& window, string& search_bar, vector<Text>& found_files_vector_text, 
		vector<float>& file_size);
	void control_volume(RectangleShape& cursor, Music& music, SoundBuffer& soundBuffer, RenderWindow& window);
	void draw(RenderWindow& window);
};

