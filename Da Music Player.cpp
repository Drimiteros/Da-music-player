#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <vector>
#include <fstream>

#include "dynamic_asset_load_class.h"
#include "load_sound_from_cmd_class.h"
#include "load_sound_info_class.h"
#include "waveform_class.h"
#include "UI_class.h"
#include "search_directory_path_class.h"

using namespace std;
using namespace sf;

//This function loads the latest visited path from a .txt file that is stored inside the project dir (src/Saves/save.txt)
string load_latest_path(filesystem::path& assets_path) {
    string load_file_path;
    //Open the file that stores the latest save
    ifstream loadFile((assets_path / "Saves/save.txt").string());
    string this_line;
    //Read each line from save.txt
    while (getline(loadFile, this_line)) {
        //If it finds the sentence "latest directory: " it stores the entire line to the var load_file_path
        //ex. "latest directory: C:/music"
        if (this_line.find("latest directory: ") == 0) {
            //Cuts the "latest directory: " and saves to load_file_path only the path that follows after
            //ex. "latest directory: C:/music" is now "C:/music"
            load_file_path = this_line.substr(18);
        }
    }
    return load_file_path;
}

//This function saves the latest visited path from a .txt file and stores it inside the project dir (src/Saves/save.txt)
void save_file_path(string& load_file_path) {
    ofstream saveFile("src/Saves/save.txt");
    saveFile.clear();
    saveFile << "latest directory: " + load_file_path;
}

int main(int argc, char* argv[]) {
    //Load app assets dynamically if the .exe is indepented, like when
    //"Da music player" is set as a default application for opening audio files
    filesystem::path assets_path;
    bool has_passed = false; //If a cmd argumant has passed
    string passed_file_path;
    dynamic_asset_load_class dynamic_asset_load_class_obj;
    dynamic_asset_load_class_obj.dynamic_asset_load(argc, argv, assets_path, has_passed, passed_file_path);

    //Sets the default path that contains the audio files
    string load_file_path = load_latest_path(assets_path);

    //Initialize the version, the window and its events
    string version = "Da music player v.3.1";
    RenderWindow window(VideoMode(995, 770), version, Style::Close);
    Event event;
    //Set the window icon
    Image image;
    image.loadFromFile((assets_path / "Textures/logo.png").string());
    window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

    load_sound_from_cmd_class load_sound_from_cmd_class_obj;
    load_sound_info_class load_sound_info_class_obj;
    search_directory_path_class search_directory_path_class_obj;
    waveform_class waveform_class_obj;

    //This value increases or decreases if the mouse wheel goes up or down
    RectangleShape cursor; //A rectangle that acts as a physical cursor. It's used to see whith what objects our actul cursor comes in contact
    cursor.setSize(Vector2f(1, 1));

    //Creates a vector of type text that holds each file's name, inside a directory, and can be used to render the file's name in the window
    vector<Text> directory_text_vector;
    //directory_text is only used to set the properties(like the string it contains, etc...) for directory_text_vector. We can't directly set the
    //properties for directory_text_vector because is a vector. We assign the properties to it by typing directory_text_vector.push_back(directory_text); later
    Text directory_text;
    UI_class UI_class_obj;
    UI_class_obj.init_vars(assets_path, load_file_path, directory_text);

    vector<string> file_paths; //Stores the full path of each file inside a directory
    bool stop_search = false; //When this variable is true, we stop the loop that searches through the directory we chose to view
    vector<string>file_name; //Store every file name inside a directory
    string final_file_name; //Store the name of the current selected audio file that we are playing
    vector<float>file_size; //Store every file size inside a directory
    float final_file_size = 0; //Store the size of the current selected audio file that we are playing

    SoundBuffer soundBuffer; //Stores the audio we want to play in memory
    Sound sound; //Holds the audio data from the soundBuffer
    float sound_offset = 0; //Is used to set the timestamp of the audio (what is the current position of the sound)
    int sound_duration = 0; //Holds the sound duration

    //Load and play sound at startup if the user clicks directly to an audio file
    load_sound_from_cmd_class_obj.load_sound_from_cmd(soundBuffer, sound, sound_duration, final_file_name, final_file_size, file_name, 
        file_size, has_passed, passed_file_path);

    //Main window loop, terminates when the window is closed
    while (window.isOpen()) {
        
        //Handle events
        while (window.pollEvent(event)) {
            //If the user clicks the "x" button in the window, we terminate the window
            if (event.type == Event::Closed)
                window.close();

            UI_class_obj.set_new_target_directory(window, event, load_file_path);
            UI_class_obj.set_file_list_position(window, event, stop_search);
            UI_class_obj.control_playback_via_keys(window, event, cursor, sound, sound_offset);
        }
        
        //We set the physical mouse cursor to follow the actual mouse cursor
        cursor.setPosition(window.mapPixelToCoords(Vector2i(Mouse::getPosition(window).x, Mouse::getPosition(window).y)));

        UI_class_obj.control_playback_via_UI(window, cursor, soundBuffer, sound);

        UI_class_obj.UI_update(cursor, sound, soundBuffer, sound_duration, final_file_name, final_file_size, 
            directory_text_vector, directory_text);

        search_directory_path_class_obj.search_directory_path(stop_search, load_file_path, file_name, file_size, file_paths,
            directory_text_vector, directory_text);

        load_sound_info_class_obj.load_sound_info(window, cursor, file_paths, soundBuffer, sound, 
            sound_duration, final_file_name, final_file_size, file_name, file_size, directory_text_vector);

        waveform_class_obj.waveform_logic(soundBuffer, sound);

        //Draw graphics
        window.clear();
        waveform_class_obj.draw(window);
        UI_class_obj.draw(window, cursor, directory_text_vector, directory_text);
        window.display();
    }

    save_file_path(load_file_path);

    return 0;
}