#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <vector>
#include <fstream>
#include <Windows.h>

using namespace std;
using namespace sf;

int main() {
    string version = "Da music player v.2.1";
    RenderWindow window(VideoMode(995, 735), version, Style::Close);
    Event event;

    ifstream loadFile("src/Saves/save.txt");
    string this_line;
    string load_file_path;
    while (getline(loadFile, this_line)) {
        if (this_line.find("latest directory: ") == 0) {
            load_file_path = this_line.substr(18);
        }
    }

    int scroll_value = 0;

    Texture play_button_texture;
    Texture next_button_texture;
    Texture prev_button_texture;
    play_button_texture.loadFromFile("src/Textures/play.png");
    next_button_texture.loadFromFile("src/Textures/next.png");
    prev_button_texture.loadFromFile("src/Textures/prev.png");
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

    Font font;
    font.loadFromFile("src/Fonts/font.ttf");

    Clock clock;
    Clock clock2;
    string file_path = load_file_path;
    vector<string>file_name;
    string final_file_name;
    float final_file_size = 0;
    vector<float>file_size;
    bool stop_search = false;

    vector<Text> directory_text_vector;
    vector<string> file_paths;
    Text directory_text;
    directory_text.setFont(font);
    directory_text.setCharacterSize(15);
    directory_text.setFillColor(Color(65, 65, 65));
    Text file_path_text;
    file_path_text.setCharacterSize(15);
    file_path_text.setFont(font);
    file_path_text.setPosition(5, 5);
    file_path_text.setFillColor(Color(255, 220, 155));
    file_path_text.setString(file_path);
    Text file_info_text;
    file_info_text.setCharacterSize(15);
    file_info_text.setFont(font);
    file_info_text.setPosition(5, 638);
    file_info_text.setFillColor(Color(255, 255, 255));
    file_info_text.setString("Now playing:\nFile size:\nDuration:");

    SoundBuffer soundBuffer;
    Sound sound;
    float sound_offset = 0;
    int sound_duration = 0;
    VertexArray waveform(LineStrip, 770);
    const Int16* samples = nullptr;
    size_t sample_count = 0;
    size_t channel_count = 0;
    RectangleShape waveformArea;
    waveformArea.setSize(Vector2f(770, 142));
    waveformArea.setPosition(220, 588);
    waveformArea.setFillColor(Color(30, 30, 30));
    float scaleY = 0.003;
    int remain_minutes = 0;
    int remain_seconds = 0;

    RectangleShape view_bounds;
    view_bounds.setSize(Vector2f(1000, 500));
    view_bounds.setPosition(0, 60);
    float height = 0;
    RectangleShape select_bar;
    select_bar.setSize(Vector2f(1000, 20));
    select_bar.setFillColor(Color(255, 155, 55));
    select_bar.setPosition(99999, 99999);
    RectangleShape cursor;
    cursor.setSize(Vector2f(1, 1));
    RectangleShape line[3];
    line[0].setSize(Vector2f(1000, 1));
    line[0].setPosition(0, 30);
    line[1].setSize(Vector2f(1000, 1));
    line[1].setPosition(0, 580);
    line[2].setSize(Vector2f(2, 20));
    line[2].setPosition(file_path_text.getLocalBounds().getSize().x + 10, 5);
    line[2].setFillColor(Color(255, 220, 155));
    RectangleShape timestamp_bar;
    timestamp_bar.setSize(Vector2f(1000, 20));
    timestamp_bar.setPosition(10, 712);
    timestamp_bar.setFillColor(Color(255, 155, 55));
    RectangleShape timestamp_bar_max;
    timestamp_bar_max.setPosition(5, 707);
    timestamp_bar_max.setFillColor(Color(255, 255, 255));

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::TextEntered && !Keyboard::isKeyPressed(Keyboard::BackSpace) && !Keyboard::isKeyPressed(Keyboard::Enter))
                file_path += event.text.unicode;
            if (event.type == Event::TextEntered && Keyboard::isKeyPressed(Keyboard::BackSpace) && !Keyboard::isKeyPressed(Keyboard::Enter))
                file_path = file_path.substr(0, file_path.size() - 1);
            file_path_text.setString(file_path);
            line[2].setPosition(file_path_text.getLocalBounds().getSize().x + 10, 5);

            if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Down))
                height -= 20;
            if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Up))
                height += 20;

            if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Enter) && !Keyboard::isKeyPressed(Keyboard::LShift))
                stop_search = false;
            if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Enter) && Keyboard::isKeyPressed(Keyboard::LShift)) {
                file_paths.clear();
                directory_text_vector.clear();
            }

            if ((Keyboard::isKeyPressed(Keyboard::Right)) || (cursor.getGlobalBounds().intersects(next_button.getGlobalBounds()) && event.type == Event::MouseButtonPressed
                && Mouse::isButtonPressed(Mouse::Left))) {
                sound_offset = sound.getPlayingOffset().asSeconds() + 1;
                sound.setPlayingOffset(seconds(sound_offset));
            }
            if ((Keyboard::isKeyPressed(Keyboard::Left)) || (cursor.getGlobalBounds().intersects(prev_button.getGlobalBounds()) && event.type == Event::MouseButtonPressed
                && Mouse::isButtonPressed(Mouse::Left))) {
                sound_offset = sound.getPlayingOffset().asSeconds() - 1;
                sound.setPlayingOffset(seconds(sound_offset));
            }

            if (((event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Space))
                || (cursor.getGlobalBounds().intersects(play_button.getGlobalBounds()) && event.type == Event::MouseButtonPressed && Mouse::isButtonPressed(Mouse::Left)))
                && sound.getStatus() == sound.Playing)
                sound.pause();
            else if (((event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Space))
                || (cursor.getGlobalBounds().intersects(play_button.getGlobalBounds()) && event.type == Event::MouseButtonPressed && Mouse::isButtonPressed(Mouse::Left)))
                && sound.getStatus() == sound.Paused)
                sound.play();

            if (event.type == Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
                    if (event.mouseWheelScroll.delta > 0)
                        scroll_value++;
                    if (event.mouseWheelScroll.delta < 0)
                        scroll_value--;
                }
            }
        }

        cursor.setPosition(window.mapPixelToCoords(Vector2i(Mouse::getPosition(window).x, Mouse::getPosition(window).y)));
        if (cursor.getGlobalBounds().intersects(timestamp_bar_max.getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
            float playing_offset = ((cursor.getPosition().x - timestamp_bar.getPosition().x) / timestamp_bar_max.getLocalBounds().width) * soundBuffer.getDuration().asSeconds();
            sound.setPlayingOffset(seconds(playing_offset));
        }

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

        if (clock2.getElapsedTime().asSeconds() < 0.5)
            line[2].setFillColor(Color(255, 220, 155));
        if (clock2.getElapsedTime().asSeconds() >= 0.5)
            line[2].setFillColor(Color(0, 0, 0));
        if (clock2.getElapsedTime().asSeconds() >= 1)
            clock2.restart();

        if (sound.getStatus() == sound.Playing)
            timestamp_bar.setSize(Vector2f(((sound.getPlayingOffset().asSeconds() * 1) / soundBuffer.getDuration().asSeconds()) * 180, 10));
        else
            timestamp_bar.setSize(Vector2f(180, 10));
        timestamp_bar_max.setSize(Vector2f(190, 20));

        if (!stop_search) {
            file_paths.clear();
            directory_text_vector.clear();
            try {
                for (const auto& entry : filesystem::directory_iterator(file_path)) {
                    string full_path = entry.path().string();
                    file_name.push_back(entry.path().filename().string());
                    file_size.push_back(entry.file_size());
                    file_paths.push_back(full_path);

                    Text temp_text = directory_text;
                    temp_text.setString(entry.path().filename().string());
                    directory_text_vector.push_back(temp_text);
                }
                stop_search = true;
            }
            catch (const filesystem::filesystem_error& e) {
                cout << e.what() << endl;
                stop_search = true;
            }
        }

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
            directory_text_vector[i].setPosition(5, 45 + height + (scroll_value * 20) + (i * 20));
            if (cursor.getGlobalBounds().intersects(directory_text_vector[i].getGlobalBounds()) && directory_text_vector[i].getGlobalBounds().intersects(view_bounds.getGlobalBounds())) {
                directory_text_vector[i].setFillColor(Color::White);
                select_bar.setPosition(directory_text_vector[i].getPosition());
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
                window.draw(select_bar);
            }
            else {
                directory_text_vector[i].setFillColor(Color(65, 65, 65));
                select_bar.setPosition(99999, 99999);
            }
            if (view_bounds.getGlobalBounds().intersects(directory_text_vector[i].getGlobalBounds()))
                window.draw(directory_text_vector[i]);
        }
        remain_minutes = (sound_duration - int(sound.getPlayingOffset().asSeconds())) / 60;
        remain_seconds = (sound_duration - int(sound.getPlayingOffset().asSeconds())) % 60;
        file_info_text.setString(final_file_name.substr(0, 19) + "\nFile size: " + to_string(final_file_size / 1048576).substr(0, 4) + " Mb\nTime: " + to_string(remain_minutes) + "m " + to_string(remain_seconds) + "s");
        
        window.draw(waveformArea);
        window.draw(waveform);
        window.display();
    }

    ofstream saveFile("src/Saves/save.txt");
    saveFile.clear();
    saveFile << "latest directory: " + file_path;

    return 0;
}