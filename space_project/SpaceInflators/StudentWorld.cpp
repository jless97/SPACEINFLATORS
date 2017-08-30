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
#include <algorithm>
#include <random>
#include <string>
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

StudentWorld::~StudentWorld()
{
  /// TODO: IMPLEMENT
}

void StudentWorld::init(void)
{
  add_initial_actor(); // Add initial actors (i.e. player spaceship)
  
  /// TODO: fix (just set to 1 for now)
  set_alien_count(1); // Set the initial alien count to 0
}

int StudentWorld::move(void)
{
  add_additional_actors(); // Add additional actors (i.e. stars, aliens, goodies)

  update_scoreboard(); // Update the scoreboard display
  
  m_spaceship->do_something(); // Ask the player spaceship to do something this tick
  
  // Ask each actor in the game to do something this tick
  for (int i = 0; i < m_actors.size(); i++)
  {
    if (m_actors[i]->is_alive()) { m_actors[i]->do_something(); }
  }
  
  // Remove newly-dead actors after each tick
  for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); ) {
    if (!(*it)->is_alive()) { delete *it; it = m_actors.erase(it); }
    else { it++; }
  }
  
  if (!m_spaceship->is_alive()) { return GWSTATUS_PLAYER_DIED; }  // If player dies, then restart round (or end game if no lives remaining)
  
  if (get_alien_count() <= 0) { update_round(); } // If the player killed all aliens, then advance to the next round
  
  return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::clean_up(void)
{
  // Remove Player Spaceship
  delete m_spaceship;
  
  // Remove actor objects from the game field
  for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); )
  {
    delete *it;
    it = m_actors.erase(it);
  }
}

void StudentWorld::add_actor(Actor* actor) { m_actors.push_back(actor); }

// Instantiate the player's spaceship
void StudentWorld::add_initial_actor(void) { m_spaceship = new Spaceship(15, 1, this); }

void StudentWorld::add_additional_actors(void) {
  int N = 4 * get_round(); // Represents the number of aliens to add to the game field each round
  int S = int(2 + 0.5 * get_round()); // Represents the number of aliens present at a given time in the game field
  
  if (get_alien_count() >= S) {} // If the number of aliens is greater than or equal to the number allowed per round, don't add more
  //else { int V = get_alien_count() -
  
  // Generate star objects at random x coordinates in the star field
  if (rand_int(1, 3) == 1) { new Star(rand_int(0, VIEW_WIDTH - 1), VIEW_HEIGHT - 1, this); }
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

void StudentWorld::check_collision(Actor* actor, bool is_player, bool is_alien, bool is_projectile) {
  for (int i = 0; i < m_actors.size(); i++)
  {
    // Check if player spaceship collided with an alien spaceship or was hit by an alien projectile
    if (is_player)
    {
      // If collision with an alien spaceship
      if (m_actors[i]->get_id() == IID_NACHLING || m_actors[i]->get_id() == IID_WEALTHY_NACHLING || m_actors[i]->get_id() == IID_SMALLBOT)
      {
        // Player Projectile
        if (is_projectile)
        {
          // Player projectile hit an alien spaceship
          if (actor->get_x() == m_actors[i]->get_x() && actor->get_y() == m_actors[i]->get_y())
          {
            actor->set_dead();
            play_sound(SOUND_ENEMY_HIT);
            dynamic_cast<Spaceship*>(m_actors[i])->update_health(dynamic_cast<SepticBullet*>(actor)->get_attack_power());
          }
        }
        // Player spaceship
        else
        {
          // Player spaceship collided with alien spaceship
          if (m_spaceship->get_x() == m_actors[i]->get_x() && m_spaceship->get_y() == m_actors[i]->get_y())
          {
            m_actors[i]->set_dead();
            play_sound(SOUND_ENEMY_PLAYER_COLLISION);
            m_spaceship->update_health(-15);
            /// TODO: This will never result in Alien dropping goodie (IMPLEMENT)
            /// TODO: This will never count towards the number of destroyed aliens required to complete the current round
          }
        }
      }
    }
  }
  
  // Check if alien spaceship collided with the player spaceship or was hit by a player projectile
  if (is_alien)
  {
    // Alien projectile
    if (is_projectile)
    {
      // Alien projectile hit player spaceship
      if (actor->get_x() == m_spaceship->get_x() && actor->get_y() == m_spaceship->get_y())
      {
        actor->set_dead();
        play_sound(SOUND_PLAYER_HIT);
        m_spaceship->update_health(dynamic_cast<SepticBullet*>(actor)->get_attack_power());
      }
    }
    // Alien spaceship
    else
    {
      // Alien spaceship collided with player spaceship
      if (actor->get_x() == m_spaceship->get_x() && actor->get_y() == m_spaceship->get_y())
      {
        actor->set_dead();
        play_sound(SOUND_ENEMY_PLAYER_COLLISION);
        m_spaceship->update_health(-15);
      }
    }
  }
}

//Generate a random number (Equation used from Project 1 (no need to reinvent the wheel))
int StudentWorld::rand_int(int min, int max) const {
  if (max < min) { swap(max, min); }
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<int> distro(min, max);
  return distro(generator);
}






