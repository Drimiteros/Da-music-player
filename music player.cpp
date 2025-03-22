#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <vector>
#include <fstream>

using namespace std;
using namespace sf;

//This function loads the latest visited path from a .txt file that is stored inside the project dir (src/Saves/save.txt)
string load_latest_path() {
    string load_file_path;
    //Open the file that stores the latest save
    ifstream loadFile("src/Saves/save.txt");
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

void set_new_target_directory(Event& event, string& load_file_path, Text& file_path_text, RectangleShape line[]) {
    //If the user clicks buttons, we get what button is being pressed and add it to the load_file_path string in order to set the 
            //new directory we want to search
    if (event.type == Event::TextEntered && !Keyboard::isKeyPressed(Keyboard::BackSpace) && !Keyboard::isKeyPressed(Keyboard::Enter))
        load_file_path += event.text.unicode;
    //If the user enters backspace, we remove the last character from the load_file_path string 
    if (event.type == Event::TextEntered && Keyboard::isKeyPressed(Keyboard::BackSpace) && !Keyboard::isKeyPressed(Keyboard::Enter))
        load_file_path = load_file_path.substr(0, load_file_path.size() - 1);
    //Set the final path to file_path_text in order to display it in the screen
    file_path_text.setString(load_file_path);
    //We update the position of the blinking cursor to be next to the file_path_text always
    line[2].setPosition(file_path_text.getLocalBounds().getSize().x + 10, 5);
}

void set_file_list_position(Event& event, float& height, int& scroll_value, bool& stop_search) {
    //We update the height of the visible files names on the screen
    if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Down))
        height -= 20;
    if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Up))
        height += 20;
    //When we scroll the mouse wheel, we increase or decrease the height of the visible files names on the screen
    if (event.type == Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
            //If mouse wheel up
            if (event.mouseWheelScroll.delta > 0)
                scroll_value++;
            //If mouse whell down
            if (event.mouseWheelScroll.delta < 0)
                scroll_value--;
        }
    }
    //When we press Enter, we reset the search of files in the current directory
    if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Enter) && !Keyboard::isKeyPressed(Keyboard::LShift))
        stop_search = false;
}

void control_playback_via_keys(Event& event, RectangleShape& cursor, Sprite& next_button, Sprite& prev_button, 
    Sprite& play_button, Sound& sound, float& sound_offset) {
    //When we press the right arrow key, we increase the progress of the audio by 5 sec
    if ((Keyboard::isKeyPressed(Keyboard::Right)) || (cursor.getGlobalBounds().intersects(next_button.getGlobalBounds()) && event.type == Event::MouseButtonPressed
        && Mouse::isButtonPressed(Mouse::Left))) {
        sound_offset = sound.getPlayingOffset().asSeconds() + 5;
        sound.setPlayingOffset(seconds(sound_offset));
    }
    //When we press the left arrow key, we decrease the progress of the audio by 5 sec
    if ((Keyboard::isKeyPressed(Keyboard::Left)) || (cursor.getGlobalBounds().intersects(prev_button.getGlobalBounds()) && event.type == Event::MouseButtonPressed
        && Mouse::isButtonPressed(Mouse::Left))) {
        sound_offset = sound.getPlayingOffset().asSeconds() - 5;
        sound.setPlayingOffset(seconds(sound_offset));
    }
    //When we press the spacebar and the audio is playing, we pause the audio
    if (((event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Space))
        || (cursor.getGlobalBounds().intersects(play_button.getGlobalBounds()) && event.type == Event::MouseButtonPressed && Mouse::isButtonPressed(Mouse::Left)))
        && sound.getStatus() == sound.Playing)
        sound.pause();
    //When we press the spacebar and the audio is not playing, we play the audio
    else if (((event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Space))
        || (cursor.getGlobalBounds().intersects(play_button.getGlobalBounds()) && event.type == Event::MouseButtonPressed && Mouse::isButtonPressed(Mouse::Left)))
        && sound.getStatus() == sound.Paused)
        sound.play();
}

void control_playback_via_UI(RectangleShape& cursor, RectangleShape& timestamp_bar, RectangleShape& timestamp_bar_max,
    SoundBuffer& soundBuffer, Sound& sound) {
    //When we click inside the timestamp_bar_max, we set the current progress of the audio.
        //We dynamically calculate the audio progress based on the cursor position relative to the position and size of timestamp_bar
    if (cursor.getGlobalBounds().intersects(timestamp_bar_max.getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
        float playing_offset = ((cursor.getPosition().x - timestamp_bar.getPosition().x) / timestamp_bar_max.getLocalBounds().width) * soundBuffer.getDuration().asSeconds();
        sound.setPlayingOffset(seconds(playing_offset));
    }
}

void UI_update(RectangleShape& cursor, Sprite& play_button, Sprite& next_button, Sprite& prev_button, Clock& clock, 
    RectangleShape line[], Sound& sound, RectangleShape& timestamp_bar, RectangleShape& timestamp_bar_max, SoundBuffer& soundBuffer,
    int& remain_minutes, int& remain_seconds, int& sound_duration, Text& file_info_text, string& final_file_name,
    float& final_file_size, vector<Text>& directory_text_vector, float& height, int& scroll_value, RectangleShape& view_bounds,
    RectangleShape& select_bar) {
    //We update the UI and render a different part of the texture for the buttons every time the cursor intersects with them
    if (cursor.getGlobalBounds().intersects(play_button.getGlobalBounds()))
        play_button.setTextureRect(IntRect(20, 0, 20, 20));
    else
        play_button.setTextureRect(IntRect(0, 0, 20, 20));
    if (cursor.getGlobalBounds().intersects(next_button.getGlobalBounds()))
        next_button.setTextureRect(IntRect(30, 0, 30, 20));
    else
        next_button.setTextureRect(IntRect(0, 0, 30, 20));
    if (cursor.getGlobalBounds().intersects(prev_button.getGlobalBounds()))
        prev_button.setTextureRect(IntRect(0, 0, 30, 20));
    else
        prev_button.setTextureRect(IntRect(30, 0, 30, 20));

    if (clock.getElapsedTime().asSeconds() < 0.5)
        line[2].setFillColor(Color(255, 220, 155));
    if (clock.getElapsedTime().asSeconds() >= 0.5)
        line[2].setFillColor(Color(0, 0, 0));
    if (clock.getElapsedTime().asSeconds() >= 1)
        clock.restart();

    if (sound.getStatus() == sound.Playing)
        timestamp_bar.setSize(Vector2f(((sound.getPlayingOffset().asSeconds() * 1) / soundBuffer.getDuration().asSeconds()) * 180, 10));
    else
        timestamp_bar.setSize(Vector2f(180, 10));
    timestamp_bar_max.setSize(Vector2f(190, 20));

    for (int i = 0; i < directory_text_vector.size(); i++) {
        directory_text_vector[i].setPosition(5, 45 + height + (scroll_value * 20) + (i * 20));
        if (cursor.getGlobalBounds().intersects(directory_text_vector[i].getGlobalBounds()) && directory_text_vector[i].getGlobalBounds().intersects(view_bounds.getGlobalBounds())) {
            directory_text_vector[i].setFillColor(Color::White);
            select_bar.setPosition(directory_text_vector[i].getPosition());
            break;
        }
        else {
            directory_text_vector[i].setFillColor(Color(65, 65, 65));
            select_bar.setPosition(99999, 99999);
        }
    }

    remain_minutes = (sound_duration - int(sound.getPlayingOffset().asSeconds())) / 60;
    remain_seconds = (sound_duration - int(sound.getPlayingOffset().asSeconds())) % 60;
    file_info_text.setString(final_file_name.substr(0, 19) + "\nFile size: " + to_string(final_file_size / 1048576).substr(0, 4) + " Mb\nTime: " + to_string(remain_minutes) + "m " + to_string(remain_seconds) + "s");
}

void search_directory_path(bool& stop_search, Text& directory_text, vector<Text>& directory_text_vector,
    string& load_file_path, vector<string>& file_name, vector<float>& file_size, vector<string>& file_paths) {
    
    //Run until we reach the end of the current directory
    if (!stop_search) {
        file_paths.clear();
        directory_text_vector.clear();
        try {
            //Iterate through the directory
            for (const auto& entry : filesystem::directory_iterator(load_file_path)) {
                string full_path = entry.path().string();
                file_name.push_back(entry.path().filename().string());
                file_size.push_back(entry.file_size());
                file_paths.push_back(full_path);

                directory_text.setString(entry.path().filename().string());
                directory_text_vector.push_back(directory_text);
            }
            stop_search = true;
        }
        catch (const filesystem::filesystem_error& e) {
            cout << e.what() << endl;
            stop_search = true;
        }
    }
}

void waveform_update(SoundBuffer& soundBuffer, Sound& sound, const Int16* samples, size_t& sample_count, size_t& channel_count, 
    VertexArray& waveform, RectangleShape& waveformArea, float& scaleY) {
    if (soundBuffer.getSampleCount() > 0 && sound.getStatus() != Sound::Stopped) {
        samples = soundBuffer.getSamples();
        sample_count = soundBuffer.getSampleCount();
        channel_count = soundBuffer.getChannelCount();
        size_t startSample = sound.getPlayingOffset().asSeconds() * soundBuffer.getSampleRate() * channel_count;
        for (size_t i = 0; i < 770; ++i) {
            size_t sampleIndex = startSample + (i * 5);
            if (sampleIndex < sample_count) {
                float sampleValue = 0;
                for (size_t channel = 0; channel < channel_count; ++channel) {
                    if (sampleIndex + channel < sample_count)
                        sampleValue += samples[sampleIndex + channel];
                }
                sampleValue /= channel_count;
                waveform[i].position = Vector2f(waveformArea.getPosition().x + i, waveformArea.getPosition().y + (waveformArea.getSize().y / 2) + (sampleValue * scaleY));
                float intensity = abs(sampleValue) / 32768.0f;
                waveform[i].color = Color(150 + 190 * intensity, 100 + 195 * intensity, 55);
            }
            else {
                waveform[i].position = Vector2f(waveformArea.getPosition().x + i, waveformArea.getPosition().y + (waveformArea.getSize().y / 2));
                waveform[i].color = Color(50, 50, 250);
            }
        }
    }
}

void load_sound_info(RenderWindow& window, RectangleShape cursor, vector<Text>directory_text_vector, float& height, 
    int& scroll_value, RectangleShape& select_bar, vector<string>& file_paths, SoundBuffer& soundBuffer, Sound& sound, 
    RectangleShape& view_bounds, int& sound_duration,string& final_file_name, float& final_file_size, 
    vector<string>& file_name, vector<float>& file_size) {
    for (int i = 0; i < directory_text_vector.size(); i++) {
        directory_text_vector[i].setPosition(5, 45 + height + (scroll_value * 20) + (i * 20));
        if (cursor.getGlobalBounds().intersects(directory_text_vector[i].getGlobalBounds()) && directory_text_vector[i].getGlobalBounds().intersects(view_bounds.getGlobalBounds())) {
            if (Mouse::isButtonPressed(Mouse::Left)) {
                string selected_file = file_paths[i];
                if (soundBuffer.loadFromFile(selected_file)) {
                    sound.setBuffer(soundBuffer);
                    sound_duration = int(soundBuffer.getDuration().asSeconds());
                    final_file_name = file_name[i];
                    final_file_size = file_size[i];
                    sound.play();
                }
            }
        }
    }
}

void draw_UI(RenderWindow& window, RectangleShape& cursor, vector<Text>& directory_text_vector, RectangleShape& select_bar, 
    RectangleShape& view_bounds, RectangleShape line[], Text& file_path_text, Sprite& play_button, Sprite& next_button, 
    Sprite& prev_button, Text& file_info_text, RectangleShape& timestamp_bar, RectangleShape& timestamp_bar_max,
    RectangleShape& waveformArea, VertexArray& waveform) {
    window.clear();
    window.draw(line[0]);
    window.draw(line[1]);
    window.draw(line[2]);
    window.draw(file_path_text);
    window.draw(play_button);
    window.draw(next_button);
    window.draw(prev_button);
    window.draw(file_info_text);
    window.draw(timestamp_bar_max);
    window.draw(timestamp_bar);
    
    for (int i = 0; i < directory_text_vector.size(); i++) {
        if (cursor.getGlobalBounds().intersects(directory_text_vector[i].getGlobalBounds()) && directory_text_vector[i].getGlobalBounds().intersects(view_bounds.getGlobalBounds())) {
            window.draw(select_bar);
        }
        if (view_bounds.getGlobalBounds().intersects(directory_text_vector[i].getGlobalBounds()))
            window.draw(directory_text_vector[i]);
    }

    window.draw(waveformArea);
    window.draw(waveform);
    window.display();
}

int main() {
    //Initialize the version, the window and its events
    string version = "Da music player v.2.1";
    RenderWindow window(VideoMode(995, 735), version, Style::Close);
    Event event;

    //Sets the default path that contains the audio files
    string load_file_path = load_latest_path();

    //This value increases or decreases if the mouse wheel goes up or down
    //It is used to go up and down the audio list in the current directory
    int scroll_value = 0;

    //Load a font for the text
    Font font;
    font.loadFromFile("src/Fonts/font.ttf");
    //Initialize the UI
    //Create textures for each button
    Texture play_button_texture;
    Texture next_button_texture;
    Texture prev_button_texture;
    //Load the texture
    play_button_texture.loadFromFile("src/Textures/play.png");
    next_button_texture.loadFromFile("src/Textures/next.png");
    prev_button_texture.loadFromFile("src/Textures/prev.png");
    //Create the sprites and give them the textures
    Sprite play_button;
    play_button.setTexture(play_button_texture);
    play_button.setScale(2, 2);
    play_button.setPosition(5, 588);
    Sprite prev_button;
    prev_button.setTexture(prev_button_texture);
    prev_button.setScale(2, 2);
    prev_button.setPosition(60, 588);
    Sprite next_button;
    next_button.setTexture(next_button_texture);
    next_button.setScale(2, 2);
    next_button.setPosition(135, 588);
    //Creates a vector of type text that holds each file's name, inside a directory, and can be used to render the file's name in the window
    vector<Text> directory_text_vector;
    //directory_text is only used to set the properties(like the string it contains, etc...) for directory_text_vector. We can't directly set the
    //properties for directory_text_vector because is a vector. We assign the properties to it by typing directory_text_vector.push_back(directory_text); later
    Text directory_text;
    directory_text.setFont(font);
    directory_text.setCharacterSize(15);
    directory_text.setFillColor(Color(65, 65, 65));
    //The file_path_text is used to render the file path, the user has entered, to the screen
    Text file_path_text;
    file_path_text.setCharacterSize(15);
    file_path_text.setFont(font);
    file_path_text.setPosition(5, 5);
    file_path_text.setFillColor(Color(255, 220, 155));
    file_path_text.setString(load_file_path);//sets the initial string to be the path we loaded from the save
    //The file_info_text is used to render the each file's info to the screen
    Text file_info_text;
    file_info_text.setCharacterSize(15);
    file_info_text.setFont(font);
    file_info_text.setPosition(5, 638);
    file_info_text.setFillColor(Color(255, 255, 255));
    file_info_text.setString("Now playing:\nFile size:\nDuration:"); //Placeholder string
    vector<string> file_paths; //Stores the full path of each file inside a directory

    Clock clock; //We use this to make the cursor blink ever 0.5 sec
    bool stop_search = false; //When this variable is true, we stop the loop that searches through the directory we chose to view
    vector<string>file_name; //Store every file name inside a directory
    string final_file_name; //Store the name of the current selected audio file that we are playing
    vector<float>file_size; //Store every file size inside a directory
    float final_file_size = 0; //Store the size of the current selected audio file that we are playing

    SoundBuffer soundBuffer; //Stores the audio we want to play in memory
    Sound sound; //Holds the audio data from the soundBuffer
    float sound_offset = 0; //Is used to set the timestamp of the audio (what is the current position of the sound)
    int sound_duration = 0; //Holds the sound duration

    //view_bounds is a rectangle that sets the limit of the current visible names of audio files inside a directory. 
    //If the current Text that each holds the name of a file (directory_text_vector) intersects with view_bounds, then it's being painted to the screen
    RectangleShape view_bounds;
    view_bounds.setSize(Vector2f(1000, 500));
    view_bounds.setPosition(0, 60);
    float height = 0; //Controls the y position in the screen of every file name inside a directory
    RectangleShape select_bar; //A wide rectangle that highlights the selected name of an audio file
    select_bar.setSize(Vector2f(1000, 20));
    select_bar.setFillColor(Color(255, 155, 55));
    select_bar.setPosition(99999, 99999);
    RectangleShape cursor; //A rectangle that acts as a physical cursor. It's used to see whith what objects our actul cursor comes in contact
    cursor.setSize(Vector2f(1, 1));
    RectangleShape line[3]; //An array of rectangle lines. Lines 0-1 are used to draw white barriers to the screen. Line 2 is the blinking cursor
    line[0].setSize(Vector2f(1000, 1));
    line[0].setPosition(0, 30);
    line[1].setSize(Vector2f(1000, 1));
    line[1].setPosition(0, 580);
    line[2].setSize(Vector2f(2, 20));
    line[2].setPosition(file_path_text.getLocalBounds().getSize().x + 10, 5); //Set the blinking cursor's default position next to the file_path_text position
    line[2].setFillColor(Color(255, 220, 155));
    RectangleShape timestamp_bar; //A rectangle that represents the audio progress bar
    timestamp_bar.setSize(Vector2f(1000, 20));
    timestamp_bar.setPosition(10, 712);
    timestamp_bar.setFillColor(Color(255, 155, 55));
    RectangleShape timestamp_bar_max; //A rectangle that represents the background of the timestamp_bar
    timestamp_bar_max.setPosition(5, 707);
    timestamp_bar_max.setFillColor(Color(255, 255, 255));

    //Initialize the waveform
    //VertexArray waveform stores a collection of vertices (each vertex uses a audio sample to set its current y position in the screen). 
    //Linestrip connects the vertices and 770 is the amount of verticies we want to be displayed (the more verticies, the better the resolution)
    VertexArray waveform(LineStrip, 770); 
    const Int16* samples = nullptr; //Unsigned 16 bit integer that stores the audio samples
    size_t sample_count = 0; //Unsigned integer that stores the sample amount
    size_t channel_count = 0; //Unsigned integer that stores the audio channel amount.
    RectangleShape waveformArea; //A rectangle that acts as a backgorund for the waveform
    waveformArea.setSize(Vector2f(770, 142));
    waveformArea.setPosition(220, 588);
    waveformArea.setFillColor(Color(30, 30, 30));
    float scaleY = 0.003; //Controls the max heihgt of the waveform
    int remain_minutes = 0; //Stores the remain minutes of the audio
    int remain_seconds = 0; //Stores the remain seconds of the audio

    //Main window loop, terminates when the window is closed
    while (window.isOpen()) {
        
        //Handle events
        while (window.pollEvent(event)) {
            //If the user clicks the "x" button in the window, we terminate the window
            if (event.type == Event::Closed)
                window.close();

            set_new_target_directory(event, load_file_path, file_path_text, line);
            set_file_list_position(event, height, scroll_value, stop_search);
            control_playback_via_keys(event, cursor, next_button, prev_button, play_button, sound, sound_offset);
        }

        //We set the physical mouse cursor to follow the actual mouse cursor
        cursor.setPosition(window.mapPixelToCoords(Vector2i(Mouse::getPosition(window).x, Mouse::getPosition(window).y)));

        control_playback_via_UI(cursor, timestamp_bar, timestamp_bar_max, soundBuffer, sound);

        UI_update(cursor, play_button, next_button, prev_button, clock, line, sound, timestamp_bar, timestamp_bar_max, soundBuffer, 
            remain_minutes, remain_seconds, sound_duration, file_info_text, final_file_name, final_file_size, directory_text_vector, 
            height, scroll_value, view_bounds, select_bar);

        search_directory_path(stop_search, directory_text, directory_text_vector, load_file_path, file_name, file_size, file_paths);

        load_sound_info(window, cursor, directory_text_vector, height, scroll_value, select_bar, file_paths, soundBuffer, sound,
            view_bounds, sound_duration, final_file_name, final_file_size, file_name, file_size);

        waveform_update(soundBuffer, sound, samples, sample_count, channel_count, waveform, waveformArea, scaleY);

        draw_UI(window, cursor, directory_text_vector, select_bar, view_bounds, line, file_path_text, play_button, 
            next_button, prev_button, file_info_text, timestamp_bar, timestamp_bar_max, waveformArea, waveform);
    }

    save_file_path(load_file_path);

    return 0;
}