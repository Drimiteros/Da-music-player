#include "Preferences.h"

Preferences::Preferences() {

}

void Preferences::load_latest_path(filesystem::path& assest_path, string& search_bar) {
	ifstream loadFile(assest_path / "Saves/save.txt");
	string line;
	while (getline(loadFile, line)) {
		if (line.find("latest directory: ") == 0) {
			search_bar = line.substr(18);
		}
	}
	loadFile.close();
}

void Preferences::save_latest_path(filesystem::path& assest_path, string& search_bar) {
	ofstream saveFile(assest_path / "Saves/save.txt");
	saveFile.clear();
	saveFile << "latest directory: " + search_bar;
	saveFile.close();
}