#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <vector>
#include <fstream>

using namespace std;
using namespace sf;

class search_directory_path_class
{
private:

public:
	search_directory_path_class();
	void search_directory_path(bool& stop_search, string& load_file_path, vector<string>& file_name, vector<float>& file_size, 
		vector<string>& file_paths, vector<Text>& directory_text_vector, Text& directory_text);
};

