#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <vector>
#include <fstream>

using namespace std;
using namespace sf;

class load_sound_info_class
{
private:

public:
	load_sound_info_class();
	void load_sound_info(RenderWindow& window, RectangleShape& cursor, vector<string>& file_paths, SoundBuffer& soundBuffer, 
		Sound& sound, int& sound_duration, string& final_file_name, float& final_file_size, vector<string>& file_name, 
		vector<float>& file_size, vector<Text>& directory_text_vector);
};

