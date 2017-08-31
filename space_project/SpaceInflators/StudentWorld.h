/*******************************************************************************
 *StudentWorld.h
 *
 * This routine is responsible for setting up the configuration of the game
 * (i.e. (de)initializing actors, updating actors)
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-08-28 11:11:11 -0700 (Mon, 28 Aug 2017) $
 *
 ******************************************************************************/

#ifndef _STUDENTWORLD_H_
#define _STUDENTWORLD_H_

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "GameWorld.h"
#include "Actor.h"
#include <vector>
#include <string>
#include <iostream> /// DEBUGGING

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------GLOBALS--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#define BASE_CHANCE_OF_FIRING 10 

///////////////////////////////////////////////////////////////////////////
///////////////////-----------STUDENTWORLD--------------///////////////////
///////////////////////////////////////////////////////////////////////////

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string asset_dir);
  virtual void init(void);
  virtual int move(void);
  virtual void clean_up(void);
  void add_actor(Actor* actor);                  // Add an actor to the game field
  void add_initial_actor(void);                  // Add initial actors to the game field (i.e. player spaceship)
  void add_additional_actors(void);              // Add additional actors during the move() phase (i.e. alien spaceships, goodies, etc.)
  void update_scoreboard(void);                  // Update the scoreboard at the top of the game screen
  void update_round(void);                       // Increment the round number once a player completes the current round
  void update_aliens_left_this_round(int how_much); // Updates the number of aliens left to kill this round
  void update_current_aliens_on_screen(int how_much); // Updates the number of aliens currently present in the space field
  void update_bullet_count(int how_much);        // Updates the current number of enemy bullets in the space field
  void set_round(unsigned int value);            // Set the current round
  void set_aliens_per_round(void);               // Sets the number of aliens to be killed this round (to advance to the next round)
  void set_aliens_left_this_round(int value);    // Sets the number of aliens left to be killed this round
  void set_max_aliens_on_screen(void);           // Sets the number of aliens that can be present on the screen at any given time
  void set_current_aliens_on_screen(void);       // Sets the number of aliens currently present on screen
  void set_bullet_count(void);                   // Sets the number of enemy bullets currently active in the space field
  unsigned int get_round(void) const;            // Returns the current round number
  int get_aliens_per_round(void) const; // Returns the count of the number of aliens to kill this round (to advance to next round)
  int get_aliens_left_this_round(void) const;    // Returns the count of the number of aliens left to kill this round (to advance)
  int get_max_aliens_on_screen(void) const;      // Returns the max number of aliens that can be present on screen at any given time
  int get_current_aliens_on_screen(void) const;  // Returns the number of aliens currently present in the space field
  int get_bullet_count(void) const;              // Returns the number of enemy bullets currently active in the space field
  // Checks if there were any collisions between actor objects
  void check_collision(Actor* actor, bool is_player=true, bool is_alien=false, bool is_projectile=false);
  bool in_line_with_player_spaceship(int x) const; // Returns true if the alien spaceship has the same x-coordinate as the player spaceship
  int get_player_spaceship_x_coord(void) const;  // Returns the x-coordinate of the player spaceship
  int get_player_spaceship_y_coord(void) const;  // Returns the y-coordinate of the player spaceship
  int rand_int(int x, int y) const;              // Generates a random number between x and y
  int max(int x, int y) const { if (x >= y) { return x; } else { return y; } } // Returns the max of two values
  int min(int x, int y) const { if (x <= y) { return x; } else { return y; } } // Returns the min of two values
  virtual ~StudentWorld(void);

private:
  std::vector<Actor*>   m_actors;                   // Vector containing all of the actor objects
  Spaceship*            m_spaceship;                // Specific member to reference the player's spaceship
  unsigned int          m_round;                    // The current round that the player is on
  int                   m_aliens_per_round;         // The number of aliens that need to be killed this round (to advance to the next round)
  int                   m_aliens_left_this_round;   // The number of aliens left to be killed this round
  int                   m_max_aliens_on_screen;     // The max number of aliens present on a screen at any given time
  int                   m_current_aliens_on_screen; // The number of aliens currently present in the space field
  int                   m_bullet_count;             // The number of enemy bullets in the space field
};

#endif // _GAMEWORLD_H_
