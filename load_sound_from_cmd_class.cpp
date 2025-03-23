#include "load_sound_from_cmd_class.h"

load_sound_from_cmd_class::load_sound_from_cmd_class() {

}

void load_sound_from_cmd_class::load_sound_from_cmd(SoundBuffer& soundBuffer, Sound& sound, int& sound_duration, string& final_file_name, 
    float& final_file_size, vector<string>& file_name, vector<float>& file_size, bool& has_passed, string& passed_file_path) {
    if (has_passed) {
        cout << "passed" << endl;
        const auto& entry = filesystem::directory_entry(passed_file_path);
        soundBuffer.loadFromFile(passed_file_path);
        sound_duration = int(soundBuffer.getDuration().asSeconds());
        final_file_name = entry.path().filename().string();
        final_file_size = entry.file_size();
        sound.setBuffer(soundBuffer);
        sound.play();
        has_passed = false;
    }
}