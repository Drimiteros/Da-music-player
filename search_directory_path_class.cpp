#include "search_directory_path_class.h"

search_directory_path_class::search_directory_path_class() {

}

void search_directory_path_class::search_directory_path(bool& stop_search, string& load_file_path, vector<string>& file_name, 
    vector<float>& file_size, vector<string>& file_paths, vector<Text>& directory_text_vector, Text& directory_text) {

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