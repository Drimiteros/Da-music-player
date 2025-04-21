#include "Preferences.h"

Preferences::Preferences() {

}

void Preferences::load_preferences(filesystem::path& assest_path, wstring& search_bar_wstring, int& theme, bool& performance_mode, bool& view_waveform) {
	wifstream loadFile(assest_path / "Saves/save.txt");
	wstring line;

	wstring default_theme;
	wstring default_performance_mode;
	wstring default_view_waveform;

	while (getline(loadFile, line)) {
		if (line.find(L"latest directory: ") == 0)
			search_bar_wstring = line.substr(18);
		if (line.find(L"theme: ") == 0)
			default_theme = line.substr(7);
		if (line.find(L"performance mode: ") == 0)
			default_performance_mode = line.substr(18);
		if (line.find(L"waveform: ") == 0)
			default_view_waveform = line.substr(10);


		if (default_theme.length() > 0)
			theme = stoi(default_theme);
		else
			theme = 0;
		if (default_performance_mode.length() > 0)
			performance_mode = stoi(default_performance_mode);
		else
			performance_mode = 1;
		if (default_view_waveform.length() > 0)
			view_waveform = stoi(default_view_waveform);
		else
			view_waveform = 1;
	}
	loadFile.close();
}

void Preferences::save_preferences(filesystem::path& assest_path, wstring& search_bar_wstring, int& theme, bool& performance_mode, bool& view_waveform) {
	wofstream saveFile(assest_path / "Saves/save.txt");
	saveFile.clear();
	saveFile << "latest directory: " << search_bar_wstring;
	saveFile << "\ntheme: " << theme;
	saveFile << "\nperformance mode: " << performance_mode;
	saveFile << "\nwaveform: " << view_waveform;
	saveFile.close();
}