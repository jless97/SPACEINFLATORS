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

class StudentWorld : public GameWorld {
public:
  StudentWorld(std::string asset_dir);
  virtual void init(void);
  virtual int move(void);
  virtual void clean_up(void);
  virtual ~StudentWorld(void);

private:
  std::vector<Actor*> m_actors;                               // Vector containing all of the actor objects
  Spaceship* m_spaceship;                                     // Specific member to reference the player's spaceship
};

#endif // _GAMEWORLD_H_
