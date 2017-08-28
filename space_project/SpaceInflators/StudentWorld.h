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
  // Constructor
  StudentWorld(std::string asset_dir);
  // Destructor
  virtual ~StudentWorld(void);
  /* GameWorld Virtual Functions */
  virtual void init(void);
  virtual int move(void);
  virtual void clean_up(void);

private:

};

#endif // _GAMEWORLD_H_
