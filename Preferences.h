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
	void load_latest_path(filesystem::path& assest_path, string& search_bar);
	void save_latest_path(filesystem::path& assest_path, string& search_bar);
};

