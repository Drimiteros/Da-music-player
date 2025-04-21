#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std;

class Preferences
{
private:

public:
	Preferences();
	void load_preferences(filesystem::path& assest_path, wstring& search_bar_wstring, int& theme, bool& performance_mode, bool& view_waveform);
	void save_preferences(filesystem::path& assest_path, wstring& search_bar_wstring, int& theme, bool& performance_mode, bool& view_waveform);
};

