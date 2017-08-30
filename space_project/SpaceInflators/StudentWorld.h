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
  void add_initial_actor(void);                 // Add initial actors to the game field (i.e. player spaceship)
  void add_additional_actors(void);              // Add additional actors during the move() phase (i.e. alien spaceships, goodies, etc.)
  void update_scoreboard(void);                  // Update the scoreboard at the top of the game screen
  void update_round(void);                       // Increment the round number once a player completes the current round
  void update_alien_count(void);                 // Decrements the count for the number of aliens present in the current round
  void set_round(unsigned int value);            // Set the current round
  void set_alien_count(unsigned int value);      // Set the alien count
  unsigned int get_round(void) const;            // Returns the current round number
  unsigned int get_alien_count(void) const;      // Returns the count of the number of aliens present in the current round
  // Checks if there were any collisions between actor objects
  void check_collision(Actor* actor, bool is_player=true, bool is_alien=false, bool is_projectile=false);
  int rand_int(int x, int y) const;              // Generates a random number between x and y
  virtual ~StudentWorld(void);

private:
  std::vector<Actor*>   m_actors;                // Vector containing all of the actor objects
  Spaceship*            m_spaceship;             // Specific member to reference the player's spaceship
  unsigned int          m_round;                 // The current round that the player is on
  unsigned int          m_alien_count;           // The number of aliens still alive in the current round
};

#endif // _GAMEWORLD_H_
