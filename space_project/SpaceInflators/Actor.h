/*******************************************************************************
 *Actor.h
 *
 * This routine is responsible for all of the objects (actors) within the game
 * The main function of each actor is to do something each clock tick
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-08-29 11:11:11 -0700 (Tues, 29 Aug 2017) $
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

class Actor: public GraphObject {
public:
  Actor(int image_id, int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void) = 0;
  StudentWorld* world(void) const;                                  // Returns a pointer to the StudentWorld class
  void set_dead(void);                                              // If an actor dies, set state to dead (so it can be removed from field)
  bool is_alive(void) const;                                        // Returns the current state of the actor object
  virtual ~Actor();

private:
  bool m_alive;
  StudentWorld* m_world;
};

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------STAR--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

class Star: public Actor {
public:
  Star(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~Star();
  
private:
};

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------SPACESHIP--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class Spaceship : public Actor {
public:
  Spaceship(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  void update_health(unsigned int how_much);      // Update health when spaceships either gain/lose health points
  void update_torpedoes(unsigned int how_much);   // Update the number of torpedoes a spaceship currently has
  unsigned int get_health(void) const;            // Returns the current health of the spaceship
  unsigned int get_torpedoes(void) const;         // Returns the current torpedo count of the spaceship
  virtual ~Spaceship();
  
private:
  unsigned int m_health;
  unsigned int m_torpedoes;
};

///////////////////////////////////////////////////////////////////////////
////////////////////////-----------NACHLING--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class Nachling : public Spaceship {
public:
  Nachling(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~Nachling();
  
private:
};


///////////////////////////////////////////////////////////////////////////
///////////////////-----------WEALTHY NACHLING--------------///////////////
///////////////////////////////////////////////////////////////////////////

class WealthyNachling : public Nachling {
public:
  WealthyNachling(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~WealthyNachling();
  
private:
};

///////////////////////////////////////////////////////////////////////////
////////////////////////-----------SMALLBOT--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class Smallbot : public Spaceship {
public:
  Smallbot(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~Smallbot();
  
private:
};

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------GOODIE--------------///////////////////
///////////////////////////////////////////////////////////////////////////

class Goodie : public Actor {
public:
  Goodie(int start_x, int start_y, StudentWorld* world, int ticks);
  virtual void do_something(void);
  void update_ticks(int how_much);                        // Increment/decrement the ticks before the goodie disappears
  unsigned int get_ticks(void) const;                     // Returns the number of ticks a goodie has before it disappears
  virtual ~Goodie();
  
private:
  unsigned int m_nticks_before_disappear;
};

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------ENERGY--------------///////////////////
///////////////////////////////////////////////////////////////////////////

class Energy : public Goodie {
public:
  Energy(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~Energy();
  
private:
};

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------EXTRA LIVE--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class ExtraLive : public Goodie {
public:
  ExtraLive(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~ExtraLive();
  
private:
};

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------EXTRA TORPEDO--------------////////////////
///////////////////////////////////////////////////////////////////////////

class ExtraTorpedo : public Goodie {
public:
  ExtraTorpedo(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~ExtraTorpedo();
  
private:
};

///////////////////////////////////////////////////////////////////////////
////////////////////////-----------BULLET--------------////////////////////
///////////////////////////////////////////////////////////////////////////

class Bullet : public Actor {
public:
  Bullet(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~Bullet();
  
private:
};

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------TORPEDO--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class Torpedo : public Bullet {
public:
  Torpedo(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~Torpedo();
private:
};

#endif // _ACTOR_H_
