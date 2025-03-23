#include "UI_class.h"

UI_class::UI_class() {
    view_bounds.setSize(Vector2f(1000, 500));
    view_bounds.setPosition(0, 60);
}

void UI_class::init_vars(filesystem::path& get_assets_path, string& get_load_file_path, Text& directory_text) {

    assets_path = get_assets_path;
    load_file_path = get_load_file_path;

    font.loadFromFile((assets_path / "Fonts/font.ttf").string());
    directory_text.setFont(font);
    directory_text.setCharacterSize(15);

    //Load the texture
    play_button_texture.loadFromFile((assets_path / "Textures/play.png").string());
    next_button_texture.loadFromFile((assets_path / "Textures/next.png").string());
    prev_button_texture.loadFromFile((assets_path / "Textures/prev.png").string());

    play_button.setTexture(play_button_texture);
    play_button.setScale(2, 2);
    play_button.setPosition(5, 588);

    prev_button.setTexture(prev_button_texture);
    prev_button.setScale(2, 2);
    prev_button.setPosition(60, 588);

    next_button.setTexture(next_button_texture);
    next_button.setScale(2, 2);
    next_button.setPosition(135, 588);

    file_path_text.setCharacterSize(15);
    file_path_text.setFont(font);
    file_path_text.setPosition(5, 5);
    file_path_text.setFillColor(Color(255, 220, 155));
    file_path_text.setString(load_file_path);//sets the initial string to be the path we loaded from the save

    file_info_text.setCharacterSize(15);
    file_info_text.setFont(font);
    file_info_text.setPosition(5, 638);
    file_info_text.setFillColor(Color(255, 255, 255));
    file_info_text.setString("Now playing:\nFile size:\nDuration:"); //Placeholder string

    select_bar.setSize(Vector2f(1000, 20));
    select_bar.setFillColor(Color(255, 155, 55));
    select_bar.setPosition(99999, 99999);

    line[0].setSize(Vector2f(1000, 1));
    line[0].setPosition(0, 30);
    line[1].setSize(Vector2f(1000, 1));
    line[1].setPosition(0, 580);
    line[2].setSize(Vector2f(2, 20));
    line[2].setPosition(file_path_text.getLocalBounds().getSize().x + 10, 5); //Set the blinking cursor's default position next to the file_path_text position
    line[2].setFillColor(Color(255, 220, 155));

    timestamp_bar.setSize(Vector2f(1000, 20));
    timestamp_bar.setPosition(10, 712);
    timestamp_bar.setFillColor(Color(255, 155, 55));

    timestamp_bar_max.setPosition(5, 707);
    timestamp_bar_max.setFillColor(Color(255, 255, 255));

    sidebar.setPosition(0, 740);
    sidebar.setSize(Vector2f(1000, 40));
    sidebar.setFillColor(Color(255, 148, 48));
    sidebar.setOutlineThickness(1);
    sidebar.setOutlineColor(Color::White);

    gear_texture.setSmooth(true);
    gear_texture.loadFromFile((assets_path / "Textures/gear.png").string());

    for (int i = 0;i < 5;i++) {
        text[i].setFillColor(Color::Black);
        text[i].setFont(font);
        text[i].setCharacterSize(15);
    }
    text[0].setString("Options");
    text[0].setPosition(10, 745);
    text[1].setString("Feedback");
    text[1].setPosition(100, 745);
    text[2].setString("Support");
    text[2].setPosition(210, 745);
}

void UI_class::set_new_target_directory(RenderWindow& window, Event& event, string& load_file_path) {
    if (window.hasFocus()) {
        //If the user clicks buttons, we get what button is being pressed and add it to the load_file_path string in order to set the 
        //new directory we want to search
        if (event.type == Event::TextEntered && !Keyboard::isKeyPressed(Keyboard::BackSpace) && !Keyboard::isKeyPressed(Keyboard::Enter))
            load_file_path += event.text.unicode;

        //If the user enters backspace, we remove the last character from the load_file_path string 
        if (event.type == Event::TextEntered && Keyboard::isKeyPressed(Keyboard::BackSpace) && !Keyboard::isKeyPressed(Keyboard::Enter))
            load_file_path = load_file_path.substr(0, load_file_path.size() - 1);

        //Set the final path to file_path_text in order to display it in the screen
        file_path_text.setString(load_file_path);
    }
    //We update the position of the blinking cursor to be next to the file_path_text always
    line[2].setPosition(file_path_text.getLocalBounds().getSize().x + 10, 5);
}
void UI_class::set_file_list_position(RenderWindow& window, Event& event, bool& stop_search) {
    if (window.hasFocus()) {
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
}
void UI_class::control_playback_via_keys(RenderWindow& window, Event& event, RectangleShape& cursor, Sound& sound, float& sound_offset) {
    if (window.hasFocus()) {
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
}

void UI_class::UI_update(RectangleShape& cursor, Sound& sound, SoundBuffer& soundBuffer, int& sound_duration, string& final_file_name, 
    float& final_file_size, vector<Text>& directory_text_vector, Text& directory_text) {

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

    for (int i = 0; i < 5; i++) {
        if (cursor.getGlobalBounds().intersects(text[i].getGlobalBounds())) {
            text[i].setFillColor(Color::White);
            if (Mouse::isButtonPressed(Mouse::Left) && i == 1)
                system("start https://tally.so/r/w2o8Rp");
            if (Mouse::isButtonPressed(Mouse::Left) && i == 2)
                system("start https://www.paypal.com/donate/?hosted_button_id=78QYVASFNBDSJ");
            if (Mouse::isButtonPressed(Mouse::Left) && i == 0) {

            }
        }
        else
            text[i].setFillColor(Color::Black);
    }

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

        if (cursor.getGlobalBounds().intersects(directory_text_vector[i].getGlobalBounds()) && directory_text_vector[i].getGlobalBounds().intersects(view_bounds.getGlobalBounds()))
            directory_text_vector[i].setFillColor(Color::White);
        else
            directory_text_vector[i].setFillColor(Color(65, 65, 65));
    }
    for (int i = 0; i < directory_text_vector.size(); i++) {
        if (cursor.getGlobalBounds().intersects(directory_text_vector[i].getGlobalBounds()) && directory_text_vector[i].getGlobalBounds().intersects(view_bounds.getGlobalBounds())) {
            select_bar.setPosition(directory_text_vector[i].getPosition());
            break;
        }
        else
            select_bar.setPosition(99999, 99999);
    }

    remain_minutes = (sound_duration - int(sound.getPlayingOffset().asSeconds())) / 60;
    remain_seconds = (sound_duration - int(sound.getPlayingOffset().asSeconds())) % 60;
    file_info_text.setString(final_file_name.substr(0, 19) + "\nFile size: " + to_string(final_file_size / 1048576).substr(0, 4) + " Mb\nTime: " + to_string(remain_minutes) + "m " + to_string(remain_seconds) + "s");
}

void UI_class::control_playback_via_UI(RenderWindow& window, RectangleShape& cursor, SoundBuffer& soundBuffer, Sound& sound) {
    if (window.hasFocus()) {
        //When we click inside the timestamp_bar_max, we set the current progress of the audio.
            //We dynamically calculate the audio progress based on the cursor position relative to the position and size of timestamp_bar
        if (cursor.getGlobalBounds().intersects(timestamp_bar_max.getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left)) {
            float playing_offset = ((cursor.getPosition().x - timestamp_bar.getPosition().x) / timestamp_bar_max.getLocalBounds().width) * soundBuffer.getDuration().asSeconds();
            sound.setPlayingOffset(seconds(playing_offset));
        }
    }
}

void UI_class::draw(RenderWindow& window, RectangleShape& cursor, vector<Text>& directory_text_vector, Text& directory_text) {
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
    window.draw(sidebar);
    for (int i = 0; i < 5; i++)
        window.draw(text[i]);
}