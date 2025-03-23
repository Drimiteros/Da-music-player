#include "load_sound_info_class.h"

load_sound_info_class::load_sound_info_class() {

}

void load_sound_info_class::load_sound_info(RenderWindow& window, RectangleShape& cursor, float& height, int& scroll_value, 
    vector<string>& file_paths, SoundBuffer& soundBuffer, Sound& sound, int& sound_duration, string& final_file_name, 
    float& final_file_size, vector<string>& file_name, vector<float>& file_size, 
    vector<Text>& directory_text_vector) {

    for (int i = 0; i < directory_text_vector.size(); i++) {
        directory_text_vector[i].setPosition(5, 45 + height + (scroll_value * 20) + (i * 20));
        if (cursor.getGlobalBounds().intersects(directory_text_vector[i].getGlobalBounds())) {
            if (Mouse::isButtonPressed(Mouse::Left) && window.hasFocus()) {
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