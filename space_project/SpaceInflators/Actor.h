/*******************************************************************************
 *Actor.h
 *
 * This routine is responsible for all of the objects (actors) within the game
 * The main function of each actor is to do something each clock tick
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-08-23 11:11:11 -0700 (Wed, 23 Aug 2017) $
 *
 ******************************************************************************/

#ifndef _ACTOR_H_
#define _ACTOR_H_

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "GraphObject.h"

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------GLOBALS--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------ACTOR--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

class StudentWorld;

class Actor:public GraphObject
{
   public:
	  Actor(StudentWorld* sw, int image_id, int start_x, int start_y);
	  virtual ~Actor();
	  virtual void do_something(void) = 0;

   private:

};


#endif // _ACTOR_H_