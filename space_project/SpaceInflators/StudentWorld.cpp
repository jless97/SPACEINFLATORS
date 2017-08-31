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
:GameWorld(asset_dir), m_round(1) {}

StudentWorld::~StudentWorld() { clean_up(); }

void StudentWorld::init(void)
{
  add_initial_actor(); // Add initial actors (i.e. player spaceship)
  
  // Initial alien count setup for round 1 (subsequent updates take place in move())
  set_round(get_round()); // Set the first round to 1
  set_aliens_per_round(); // Sets the number of aliens to kill for round 0 (to advance to next round)
  set_aliens_left_this_round(get_aliens_per_round()); // Sets the number of aliens left to kill for round 1
  set_max_aliens_on_screen(); // Sets the max number of aliens that can be present on screen at any given time during round 1
  set_current_aliens_on_screen(); // Upon initializing the game field, there are no aliens present in the space field
  set_bullet_count(); // Sets the initial enemy bullet count to 0
}

int StudentWorld::move(void)
{  
  add_additional_actors(); // Add additional actors (i.e. stars, aliens, goodies)
  
  update_scoreboard(); // Update the scoreboard display
  
  // Simple new feature (every 5 rounds, add 10 torpedoes to the player's inventory)
  static unsigned int multiple = 1;
  if (get_round() >= (BONUS_TORPEDOES * multiple)) { m_spaceship->update_torpedoes(10); multiple++; }
  
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
  
  // If player dies, then restart round (or end game if no lives remaining)
  if (!m_spaceship->is_alive())
  {
    dec_lives();
    return GWSTATUS_PLAYER_DIED;
  }
  
  // If the player killed all aliens, then advance to the next round
  if (get_aliens_left_this_round() <= 0)
  {
    update_round();
    set_aliens_per_round(); // Sets the number of aliens to kill this rount to advance
    set_aliens_left_this_round(get_aliens_per_round()); // Sets the aliens left to kill this round to the number of aliens to start round
    set_max_aliens_on_screen(); // Sets the max number of aliens that can be present on screen at any given time
    set_current_aliens_on_screen(); // Sets the current number of aliens present on screen to 0
  }
  
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
void StudentWorld::add_initial_actor(void) { m_spaceship = new Spaceship(this); }

void StudentWorld::add_additional_actors(void) {
  // Generate star objects at random x coordinates in the space field
  if (rand_int(1, 3) == 1) { new Star(rand_int(0, VIEW_WIDTH - 1), VIEW_HEIGHT - 1, this); }
  
  // If the number of aliens is greater than or equal to the number allowed per round, don't add more aliens
  if (get_current_aliens_on_screen() >= get_max_aliens_on_screen()) { return; }
  
  // If the number of aliens left to be killed this round are all present on screen, don't add more aliens
  if (get_aliens_left_this_round() <= get_current_aliens_on_screen()) { return; }
  
  // If passed previous checks, then add a specific type of alien spaceship to the space field
    // Add a type of Nachling
  if (rand_int(1, 10) < 7)
  {
    // Add Wealthy Nachling
    if (rand_int(1, 10) < 2)
    {
      new WealthyNachling(this, rand_int(0, 29), 39, 8 * get_round());
      update_current_aliens_on_screen(1);
    }
    // Add Nachling
    else
    {
      new Nachling(this, rand_int(0, 29), 39, 5 * get_round());
      update_current_aliens_on_screen(1);
    }
  }
    // Add a Smallbot
  else
  {
    new Smallbot(this, rand_int(0, 29), 39, 12 * get_round());
    update_current_aliens_on_screen(1);
  }
}

void StudentWorld::update_scoreboard(void) {
  // Update scoreboard fields
  unsigned int score = get_score();
  unsigned int round = get_round();
  int energy = m_spaceship->get_health();
  if (energy < 0) { energy = 0; }
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

void StudentWorld::update_aliens_left_this_round(int how_much) { m_aliens_left_this_round += how_much; }

void StudentWorld::update_current_aliens_on_screen(int how_much) { m_current_aliens_on_screen += how_much; }

void StudentWorld::update_bullet_count(int how_much) { m_bullet_count += how_much; }

void StudentWorld::set_round(unsigned int value) { m_round = value; }

void StudentWorld::set_aliens_per_round(void) { m_aliens_per_round = 4 * get_round(); }

void StudentWorld::set_aliens_left_this_round(int value) { m_aliens_left_this_round = value; }

void StudentWorld::set_max_aliens_on_screen(void) { m_max_aliens_on_screen = int(2 + 0.5 * get_round()); }

void StudentWorld::set_current_aliens_on_screen(void) { m_current_aliens_on_screen = 0; }

void StudentWorld::set_bullet_count() { m_bullet_count = 0; }

unsigned int StudentWorld::get_round(void) const { return m_round; }

int StudentWorld::get_aliens_per_round(void) const { return m_aliens_per_round; }

int StudentWorld::get_aliens_left_this_round(void) const { return m_aliens_left_this_round; }

int StudentWorld::get_max_aliens_on_screen(void) const { return m_max_aliens_on_screen; }

int StudentWorld::get_current_aliens_on_screen(void) const { return m_current_aliens_on_screen; }

int StudentWorld::get_bullet_count(void) const { return m_bullet_count; }

void StudentWorld::check_collision(Actor* actor, bool is_player, bool is_alien, bool is_projectile, bool is_goodie) {
  for (int i = 0; i < m_actors.size(); i++)
  {
    if (!m_actors[i]->is_alive()) { continue; } // To avoid doubling of one of the effects below (temporary fix)
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
            dynamic_cast<Spaceship*>(m_actors[i])->update_health(-(dynamic_cast<SepticBullet*>(actor)->get_attack_power()));
            if (m_actors[i]->get_id() == IID_SMALLBOT)
            {
              dynamic_cast<Smallbot*>(m_actors[i])->set_hit_by_player_status(true); // Inform the Smallbot object it was just hit
              if (dynamic_cast<Spaceship*>(m_actors[i])->get_health() <= 0) { increase_score(1000); }
            }
            else if (m_actors[i]->get_id() == IID_WEALTHY_NACHLING)
            {
              if (dynamic_cast<Spaceship*>(m_actors[i])->get_health() <= 0) { increase_score(1200); }
            }
            else
            {
              if (dynamic_cast<Spaceship*>(m_actors[i])->get_health() <= 0) { increase_score(1500); }
            }
            actor->set_dead();
            play_sound(SOUND_ENEMY_HIT);
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
            update_aliens_left_this_round(1); // Collision kills never count towards the number of destroyed aliens required to end the round
            /// TODO: This will never result in Alien dropping goodie (IMPLEMENT)
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
        // I wanted to add the torpedo to kill alien ships on contact, but only deal the normal damage of 8 to the player spaceship
        if (actor->get_id() == IID_BULLET) { m_spaceship->update_health(-2); }
        else { m_spaceship->update_health(-8); }
        //m_spaceship->update_health(-(dynamic_cast<SepticBullet*>(actor)->get_attack_power()));
      }
    }
  }
  // Check if a goodie collided with the player spaceship, and if so update (per the specific goodie)
  if (is_goodie)
  {
    if (actor->get_x() == m_spaceship->get_x() && actor->get_y() == m_spaceship->get_y())
    {
      actor->set_dead();
      play_sound(SOUND_GOT_GOODIE);
      increase_score(5000);
      // If goodie is a free ship, then increase player score, and increase player's lives by 1
      if (actor->get_id() == IID_FREE_SHIP_GOODIE) { inc_lives(); }
      if (actor->get_id() == IID_ENERGY_GOODIE) { m_spaceship->set_health(50); }
      if (actor->get_id() == IID_TORPEDO_GOODIE) { m_spaceship->update_torpedoes(5); }
    }
  }
}

bool StudentWorld::in_line_with_player_spaceship(int x) const { return (m_spaceship->get_x() == x); }

int StudentWorld::get_player_spaceship_x_coord(void) const { return m_spaceship->get_x(); }

int StudentWorld::get_player_spaceship_y_coord(void) const { return m_spaceship->get_y(); }

//Generate a random number (Equation used from Project 1 (no need to reinvent the wheel))
int StudentWorld::rand_int(int min, int max) const {
  if (max < min) { swap(max, min); }
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<int> distro(min, max);
  return distro(generator);
}






