#include "dynamic_asset_load_class.h"

dynamic_asset_load_class::dynamic_asset_load_class() {

}

void dynamic_asset_load_class::dynamic_asset_load(int argc, char* argv[], filesystem::path& assets_path, bool& has_passed, string& passed_file_path) {

    exe_path = filesystem::absolute(argv[0]); //Get the path from the parent .exe 
    exe_dir = exe_path.parent_path(); //Get the root from the path from the parent .exe 
    //Define the assests path
    assets_path = exe_dir / "src"; //Define the assets folder, which is located to the parent directory

    //Load the sound path at startup if the user clicks directly to an audio file
    //Check if an audio file is passed
    if (argc < 2)
        cout << "No audio file passed" << endl;
    else {
        has_passed = true; //Successfuly passed cmd arguments
        passed_file_path = argv[1]; //Assign the argument that contains the path of the current .exe
        cout << passed_file_path << endl;
    }
}