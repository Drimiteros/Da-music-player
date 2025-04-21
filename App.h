#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include "Get_cmd_arguments.h"
#include "Play_audio.h"
#include "Preferences.h"
#include "keyboard_mouse_events.h"
#include "Search_directory.h"
#include "Waveform.h"
#include "Menu_bar.h"

using namespace std;
using namespace sf;

class App
{
private:
	// Load the app's assets (font's, textures) dynamically. 
	// If the program is executed outside the working directory
	filesystem::path assets_path;
	Get_cmd_arguments get_cmd_arguments;

	// Extract the audio's path from the cmd arguments 
	// If the app is set as a defult way to open audio files
	filesystem::path passed_audio_from_args;

	// INITIALIZE WINDOW, PROPERTIES AND EVENTS
	int width = 995;
	int height = 810;
	int scroll_value = 0; // The value holds the scroll value of the mouse wheel
	vector<float> file_size;
	bool finished = false; // Returns true after a successful iteration of all the files inside a directory
	bool has_passed = false; // Returns true if the app has passed cmd arguments
	bool is_clicked = false; // Returns true if the left mouse click is pressed
	bool performance_mode = false;
	bool is_options_menu_open = false;
	bool is_about_menu_open = false;
	bool view_waveform = true;
	string version = "Da Music Player v.4.0 (revamped)";
	RenderWindow window;
	Event event;
	Image window_icon;
	wstring search_bar_wstring;
	Clock clock; // This updates the blinking rate of the caret

	// Initialize audio properties
	Music music;
	SoundBuffer soundBuffer;

	// Initialize general UI related stuff
	RectangleShape cursor;
	RectangleShape caret;
	RectangleShape search_bar_bounds;
	RectangleShape view_bounds; // This rectangle sets the visible bounds for the found_files_vector_text
	Font font;
	Text search_bar_text; // This text contains the search bar string
	vector<Text> found_files_vector_text; // This text contains each file found inside the current directory
	Text found_files_text; // This text is used to set the properties of found_files_vector_text before the pushBack
	
	// Themes
	int theme = 0;
	float background_color[4][3] = { 
		{ 61, 64, 74 },
		{ 150, 95, 4 },
		{ 4, 95, 150 },
		{ 4, 150, 95 }
	};
	float bounds_color[4][3] = {
		{ 31, 34, 44 },
		{ 60, 42,  2 },
		{ 23, 45, 95 },
		{ 23, 95, 45 }
	};
	float text_color[4][3] = {
		{ 161, 164, 174 },
		{ 250, 195, 134 },
		{ 134, 195, 250 },
		{ 134, 250, 195 }
	};

public:
	// Init classes
	Play_audio play_audio;
	Preferences preferences;
	Keyboard_mouse_events keyboard_mouse_events;
	Search_directory search_directory;
	Waveform waveform;
	Menu_bar menu_bar;

	App(int argc, char* argv[]);

	void events();
	void main_loop();
	void update();
	void draw();
};

