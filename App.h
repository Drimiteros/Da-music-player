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
	string version = "Da Music Player v.3.3_2 (revamped)";
	RenderWindow window;
	Event event;
	Image window_icon;
	string search_bar;
	bool finished = false; // Returns true after a successful iteration of all the files inside a directory
	bool has_passed = false; // Returns true if the app has passed cmd arguments
	bool is_clicked = false; // Returns true if the left mouse click is pressed
	int scroll_value = 0; // The value holds the scroll value of the mouse wheel
	vector<float> file_size;
	Clock clock; // This updates the blinking rate of the caret

	// Initialize audio properties
	Music music;
	SoundBuffer soundBuffer;

	// Initialize UI related stuff
	RectangleShape cursor;
	RectangleShape caret;
	RectangleShape search_bar_bounds;
	RectangleShape view_bounds; // This rectangle sets the visible bounds for the found_files_vector_text
	Font font;
	Text search_bar_text; // This text contains the search bar string
	vector<Text> found_files_vector_text; // This text contains each file found inside the current directory
	Text found_files_text; // This text is used to set the properties of found_files_vector_text before the pushBack

public:
	// Init classes
	Play_audio play_audio;
	Preferences preferences;
	Keyboard_mouse_events keyboard_mouse_events;
	Search_directory search_directory;
	Waveform waveform;

	App(int argc, char* argv[]);

	void events();
	void main_loop();
	void update();
	void draw();
};

