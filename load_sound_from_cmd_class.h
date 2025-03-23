#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <vector>
#include <fstream>

using namespace std;
using namespace sf;

class load_sound_from_cmd_class
{
private:

public:
	load_sound_from_cmd_class();
	void load_sound_from_cmd(SoundBuffer& soundBuffer, Sound& sound, int& sound_duration, string& final_file_name, float& final_file_size, 
		vector<string>& file_name, vector<float>& file_size, bool& has_passed, string& passed_file_path);
};

