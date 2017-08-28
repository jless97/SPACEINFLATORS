/*******************************************************************************
 *main.cpp
 *
 * The main routine that sets up the game world, and extracts the assets from the 
 * assets folder
 *
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-08-28 11:11:11 -0700 (Mon, 28 Aug 2017) $
 *
 ******************************************************************************/

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "GameController.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------GLOBALS--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

// Note: The path of the assets directory needs to be specified to access images and sound effects
const string asset_directory = "/Users/JasonLess/Desktop/SPACEINFLATORS/space_project/DerivedData/SpaceInflators/Build/Products/Debug/Assets";

class GameWorld;

GameWorld* create_student_world(string asset_dir = "");

///////////////////////////////////////////////////////////////////////////
//////////////////-----------MAIN ROUTINE--------------////////////////////
///////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
    string path = asset_directory;
    if (!path.empty()) { path += '/'; }
    const string some_asset = "frack1.tga";
    ifstream ifs(path + some_asset);
    if (!ifs) {
        cout << "Cannot find " << some_asset << " in ";
        cout << (asset_directory.empty() ? "current directory"
                 : asset_directory) << endl;
        return 1;
    }
    
    srand(static_cast<unsigned int>(time(nullptr)));

    GameWorld* gw = create_student_world(asset_directory);
    Game().run(argc, argv, gw, "Space Inflators");
}
