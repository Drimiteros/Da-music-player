#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <vector>

using namespace std;
using namespace sf;

int main()
{
    RenderWindow window(VideoMode(1000, 900), "Da music player");
    Event event;

    Texture play_button_texture;
    Texture next_button_texture;
    Texture prev_button_texture;
    play_button_texture.loadFromFile("src/Textures/play.png");
    next_button_texture.loadFromFile("src/Textures/next.png");
    prev_button_texture.loadFromFile("src/Textures/prev.png");
    Sprite play_button;
    play_button.setTexture(play_button_texture);
    play_button.setScale(2, 2);
    play_button.setPosition(5, 600);
    Sprite prev_button;
    prev_button.setTexture(prev_button_texture);
    prev_button.setScale(2, 2);
    prev_button.setPosition(55, 600);
    Sprite next_button;
    next_button.setTexture(next_button_texture);
    next_button.setScale(2, 2);
    next_button.setPosition(125, 600);

    Font font;
    if (!font.loadFromFile("src/Fonts/font.ttf")) {
        cout << "Error loading font" << endl;
        return -1;
    }

    vector<Text> directory_text_vector;
    vector<string> file_paths;  // Store actual file paths separately
    Text directory_text;
    directory_text.setFont(font);
    directory_text.setCharacterSize(15);
    directory_text.setFillColor(Color(65, 65, 65));

    Text file_path_text;
    file_path_text.setCharacterSize(15);
    file_path_text.setFont(font);
    file_path_text.setPosition(5, 5);
    file_path_text.setFillColor(Color(255, 220, 155));

    Clock clock;
    Clock clock2;
    string file_path;
    bool stop_search = true;

    // Sound storage
    SoundBuffer soundBuffer;
    Sound sound;
    float sound_offset = 0;

    RectangleShape view;
    view.setSize(Vector2f(1000, 500));
    view.setPosition(0, 60);
    float height = 0;
    RectangleShape select_bar;
    select_bar.setSize(Vector2f(1000, 20));
    select_bar.setFillColor(Color(100, 100, 255));
    select_bar.setPosition(99999, 99999);
    RectangleShape cursor;
    cursor.setSize(Vector2f(1, 1));
    RectangleShape line[3];
    line[0].setSize(Vector2f(1000, 1));
    line[0].setPosition(0, 30);
    line[1].setSize(Vector2f(1000, 1));
    line[1].setPosition(0, 580);
    line[2].setSize(Vector2f(2, 20));
    line[2].setPosition(0, 5);
    line[2].setFillColor(Color(255, 220, 155));


    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
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
        }

        cursor.setPosition(window.mapPixelToCoords(Vector2i(Mouse::getPosition(window).x, Mouse::getPosition(window).y)));

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

        if (!stop_search) {
            file_paths.clear();
            directory_text_vector.clear();
            try {
                for (const auto& entry : filesystem::directory_iterator(file_path)) {
                    string full_path = entry.path().string(); 
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

        window.clear();
        window.draw(line[0]);
        window.draw(line[1]);
        window.draw(line[2]);
        window.draw(file_path_text);
        window.draw(play_button);
        window.draw(next_button);
        window.draw(prev_button);

        for (int i = 0; i < directory_text_vector.size(); i++) {
            directory_text_vector[i].setPosition(5, 45 + height + (i * 20));
            if (cursor.getGlobalBounds().intersects(directory_text_vector[i].getGlobalBounds()) && directory_text_vector[i].getGlobalBounds().intersects(view.getGlobalBounds())) {
                directory_text_vector[i].setFillColor(Color::White);
                select_bar.setPosition(directory_text_vector[i].getPosition());
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    string selected_file = file_paths[i];
                    if (soundBuffer.loadFromFile(selected_file)) {
                        sound.setBuffer(soundBuffer);
                        sound.play();
                        cout << "Playing: " << selected_file << endl;
                    }
                }
                window.draw(select_bar);
            }
            else {
                directory_text_vector[i].setFillColor(Color(65, 65, 65));
                select_bar.setPosition(99999, 99999);
            }
            if (view.getGlobalBounds().intersects(directory_text_vector[i].getGlobalBounds()))
                window.draw(directory_text_vector[i]);
        }

        window.display();
    }

    return 0;
}
