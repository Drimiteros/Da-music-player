#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <vector>
#include <fstream>

using namespace std;
using namespace sf;

class dynamic_asset_load_class
{
private:
	filesystem::path exe_path; //Get the path from which the current .exe was openend
	filesystem::path exe_dir; //Get the root from the path from which the current .exe was opened

public:
	dynamic_asset_load_class();
	void dynamic_asset_load(int argc, char* argv[], filesystem::path& assets_path, bool& has_passed, string& passed_file_path);
};

