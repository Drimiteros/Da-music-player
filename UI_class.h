#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <vector>
#include <fstream>

using namespace std;
using namespace sf;

class UI_class
{
protected:
    //Load a font for the text
    Font font;
    //Initialize the UI
    //view_bounds is a rectangle that sets the limit of the current visible names of audio files inside a directory. 
    //If the current Text that each holds the name of a file (directory_text_vector) intersects with view_bounds, then it's being painted to the screen
    RectangleShape view_bounds;
    //Create textures for each button
    Texture play_button_texture;
    Texture next_button_texture;
    Texture prev_button_texture;
    //Create the sprites and give them the textures
    Sprite play_button;
    Sprite prev_button;
    Sprite next_button;
    //The file_path_text is used to render the file path, the user has entered, to the screen
    Text file_path_text;
    //The file_info_text is used to render the each file's info to the screen
    Text file_info_text;
    RectangleShape select_bar; //A wide rectangle that highlights the selected name of an audio file
    RectangleShape line[3]; //An array of rectangle lines. Lines 0-1 are used to draw white barriers to the screen. Line 2 is the blinking cursor
    RectangleShape timestamp_bar; //A rectangle that represents the audio progress bar
    RectangleShape timestamp_bar_max; //A rectangle that represents the background of the timestamp_bar
    //Initialize sidebar
    RectangleShape sidebar;
    Texture gear_texture;
    Text text[5];

    Clock clock; //We use this to make the cursor blink ever 0.5 sec

    float height = 0; //Controls the y position in the screen of every file name inside a directory
    int scroll_value = 0; //It is used to go up and down the audio list in the current directory
    int remain_minutes = 0; //Stores the remain minutes of the audio
    int remain_seconds = 0; //Stores the remain seconds of the audio

public:
    filesystem::path assets_path;
    string load_file_path;

	UI_class();
    void init_vars(filesystem::path& get_assets_path, string& get_load_file_path, Text& directory_text);
    void set_new_target_directory(RenderWindow& window, Event& event, string& load_file_path);
    void set_file_list_position(RenderWindow& window, Event& event, bool& stop_search);
    void control_playback_via_keys(RenderWindow& window, Event& event, RectangleShape& cursor, Sound& sound, float& sound_offset);
    void UI_update(RectangleShape& cursor, Sound& sound, SoundBuffer& soundBuffer, int& sound_duration, string& final_file_name, 
        float& final_file_size, vector<Text>& directory_text_vector, Text& directory_text);
    void control_playback_via_UI(RenderWindow& window, RectangleShape& cursor, SoundBuffer& soundBuffer, Sound& sound);
    void draw(RenderWindow& window, RectangleShape& cursor, vector<Text>& directory_text_vector, Text& directory_text);
};

