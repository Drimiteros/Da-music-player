#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <filesystem>

using namespace std;
using namespace sf;

class Search_directory
{
private:

public:
	RectangleShape view_bounds; // This rectangle sets the visible bounds for the found_files_vector_text
	RectangleShape select_bar;

	Search_directory();
	void search_current_directory(string& search_bar, vector<Text>& found_files_vector_text, Text& found_files_text, bool& finished, vector<float>& file_size);
	void draw(RenderWindow& window, vector<Text>& found_files_vector_text, int& scroll_value, RectangleShape& cursor);
};

