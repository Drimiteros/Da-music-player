#include "Search_directory.h"

Search_directory::Search_directory() {

	select_bar.setSize(Vector2f(995, 20));
	select_bar.setFillColor(Color(255, 207, 150));
}

void Search_directory::search_current_directory(string& search_bar, vector<Text>& found_files_vector_text, Text& found_files_text, bool& finished, vector<float>& file_size) {
	if (!finished) {
		found_files_vector_text.clear();
		try {
			for (const auto& entry : filesystem::directory_iterator(search_bar)) {
				string file_name = entry.path().filename().string();
				found_files_text.setString(file_name);
				found_files_vector_text.push_back(found_files_text);
				file_size.push_back(entry.file_size());
			}
			finished = true;
		}
		catch (const filesystem::filesystem_error& e) {
			cerr << e.what() << endl;
			finished = true;
		}
	}
}

void Search_directory::draw(RenderWindow& window, vector<Text>& found_files_vector_text, int& scroll_value, RectangleShape& cursor, RectangleShape& view_bounds) {
	for (int i = 0; i < found_files_vector_text.size(); i++) {
		found_files_vector_text[i].setPosition(5, 50 + (i * 27) + scroll_value * 27);
		if (view_bounds.getGlobalBounds().intersects(found_files_vector_text[i].getGlobalBounds())) {
			if (cursor.getGlobalBounds().intersects(found_files_vector_text[i].getGlobalBounds())) {
				window.draw(select_bar);
				select_bar.setPosition(found_files_vector_text[i].getPosition().x - 5, found_files_vector_text[i].getPosition().y - 1);
				found_files_vector_text[i].setFillColor(Color::Black);
			}
			else
				found_files_vector_text[i].setFillColor(Color(155, 155, 155));
			window.draw(found_files_vector_text[i]);
		}
	}
}