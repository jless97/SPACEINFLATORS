/*******************************************************************************
 *StudentWorld.cpp
 *
 * This routine is responsible for setting up the configuration of the game
 * (i.e. (de)initializing actors, updating actors)
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-08-28 11:11:11 -0700 (Mon, 28 Aug 2017) $
 *
 ******************************************************************************/

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "StudentWorld.h"
#include "GameConstants.h"
#include <vector>
#include<string>
#include <iostream> 
#include <sstream> 
#include <iomanip> 
using namespace std;

///////////////////////////////////////////////////////////////////////////
///////////////////-----------STUDENTWORLD--------------///////////////////
///////////////////////////////////////////////////////////////////////////

GameWorld* create_student_world(string asset_dir) { return new StudentWorld(asset_dir); }

StudentWorld::StudentWorld(std::string asset_dir)
:GameWorld(asset_dir) {}

StudentWorld::~StudentWorld() {

}

void StudentWorld::init() {

}

int StudentWorld::move() {
    
  
  return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::clean_up() {

}
