#include "Get_cmd_arguments.h"

Get_cmd_arguments::Get_cmd_arguments() {

}

void Get_cmd_arguments::find_assets_path(int argc, char* argv[], fs::path& assets_path) {
	
	// Get the absolute path of the original "Da music player revamped.exe"
	exe_path = filesystem::absolute(argv[0]);
	// Get the directory of the path
	exe_dir = exe_path.parent_path();

	// Get the assets folder location
	assets_path = exe_dir / "src";
}

void Get_cmd_arguments::get_Audio_Path_From_Args(int argc, char* argv[], bool& has_passed, fs::path& passed_audio_from_args) {
	
	if (argc < 2)
		cout << "No Audio file passed..." << endl;
	else {
		has_passed = true;
		passed_audio_from_args = argv[1];
		cout << "Audio file passed..." << endl;
		cout << argv[1] << endl;
	}
}