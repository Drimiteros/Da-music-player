#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <filesystem>

using namespace std;
using namespace sf;

class Get_cmd_arguments
{
private:

protected:
	filesystem::path exe_path;
	filesystem::path exe_dir;

public:
	Get_cmd_arguments();
	void find_assets_path(int argc, char* argv[], filesystem::path& assets_path);
	void get_Audio_Path_From_Args(int argc, char* argv[], bool& has_passed, filesystem::path& passed_audio_from_args);
};

