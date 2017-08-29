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
:GameWorld(asset_dir), m_round(0) {}

StudentWorld::~StudentWorld() {

}

void StudentWorld::init(void) {
  add_initial_actors();                                      // Add initial actors
  
  /// TODO: fix (just set to 1 for now)
  set_alien_count(1);                                        // Set the initial alien count to 0
}

int StudentWorld::move(void) {
  update_scoreboard();                                       // Update the scoreboard display
  
  m_spaceship->do_something();                               // Ask the player spaceship to do something this tick
  
  for (int i = 0; i < m_actors.size(); i++) { m_actors[i]->do_something(); }  // Ask each actor in the game to do something this tick
  
  add_additional_actors();                                   // Add additional actors

  if (get_lives() <= 0) { return GWSTATUS_PLAYER_DIED; }     // Check if the player still has lives remaining
  
  if (get_alien_count() <= 0) { update_round(); }            // If the player killed all aliens, then advance to the next round
  
  return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::clean_up(void) {
  // Remove Player Spaceship
  delete m_spaceship;
  
  // Remove actor objects from the game field
  for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); ) {
    delete *it;
    it = m_actors.erase(it);
  }
}

void StudentWorld::add_actor(Actor* actor) { m_actors.push_back(actor); }

void StudentWorld::add_initial_actors(void) {
  // Instantiate the player's spaceship
  m_spaceship = new Spaceship(15, 1, this);
}

void StudentWorld::add_additional_actors(void) {
  int N = 4 * get_round();                                // Represents the number of aliens to add to the game field each round
  int S = int(2 + 0.5 * get_round());                     // Represents the number of aliens present at a given time in the game field
}

void StudentWorld::update_scoreboard(void) {
  // Update scoreboard fields
  unsigned int score = get_score();
  unsigned int round = get_round();
  unsigned int energy = m_spaceship->get_health();
  unsigned int torpedoes = m_spaceship->get_torpedoes();
  unsigned int ships = get_lives();
  // Convert scoreboard fields to strings
  string score_text = "Score: " + std::string(7 - std::to_string(score).size(), '0') + std::to_string(score) + " ";
  string round_text = "Round: " + std::string(2 - std::to_string(round).size(), '0') + std::to_string(round) + " ";
  string energy_text = "Energy: " + std::string(3 - std::to_string(energy * 2).size(), ' ') + std::to_string(energy * 2) + "%" + " ";
  string torpedoes_text = "Torpedoes: " + std::string(3 - std::to_string(torpedoes).size(), '0') + std::to_string(torpedoes) + " ";
  string ships_text = "Ships: " + std::string(2 - std::to_string(ships).size(), '0') + std::to_string(ships) + " ";
  string text = score_text + round_text + energy_text + torpedoes_text + ships_text;
  
  // Update the scoreboard display
  set_game_stat_text(text);
}

void StudentWorld::update_round(void) { m_round++; }

void StudentWorld::update_alien_count(void) { m_alien_count--; }

void StudentWorld::set_round(unsigned int value) { m_round = value; }

void StudentWorld::set_alien_count(unsigned int value) { m_alien_count = value; }

unsigned int StudentWorld::get_round(void) const { return m_round; }

unsigned int StudentWorld::get_alien_count(void) const { return m_alien_count; }








