#include <iostream>
#include <SFML/Graphics.hpp>
#include "App.h"
#include "Preferences.h"

using namespace std;
using namespace sf;

int main(int argc, char* argv[])
{
	//Main app
	App app(argc, argv);
	app.main_loop();
}