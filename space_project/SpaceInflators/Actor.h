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

class Actor: public GraphObject
{
public:
  Actor(int image_id, int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void) = 0;
  StudentWorld* world(void) const;               // Returns a pointer to the StudentWorld class
  void set_dead(void);                           // If an actor dies, set state to dead (so it can be removed from field)
  bool is_alive(void) const;                     // Returns the current state of the actor object
  // Returns true if a spaceship (player or enemy) has collided with another spaceship
  virtual ~Actor();

private:
  bool m_alive;
  StudentWorld* m_world;
};

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------STAR--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

class Star: public Actor
{
public:
  Star(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~Star();
  
private:
};

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------SPACESHIP--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class Spaceship : public Actor
{
public:
  Spaceship(StudentWorld* world, int image_id=IID_PLAYER_SHIP, int start_x=15, int start_y=1, int health=50);
  virtual void do_something(void);
  void update_health(int how_much);               // Update health when spaceships either gain/lose health points
  void update_torpedoes(unsigned int how_much);   // Update the number of torpedoes a spaceship currently has
  void update_bullet_shoot(bool value);           // Update the state of whether the player fired a bullet on the previous tick
  void update_torpedo_shoot(bool value);          // Update the state of whether the player fired a torpedo on the previous tick
  int get_health(void) const;            // Returns the current health of the spaceship
  unsigned int get_torpedoes(void) const;         // Returns the current torpedo count of the spaceship
  bool get_bullet_shoot(void) const;              // Returns true if the player just fired a bullet on the previous tick
  bool get_torpedo_shoot(void) const;             // Returns true if the player just fired a torpedo on the previous tick
  virtual ~Spaceship();
  
private:
  int m_health;
  unsigned int m_torpedoes;
  bool m_bullet_shoot;
  bool m_torpedo_shoot;
};

///////////////////////////////////////////////////////////////////////////
////////////////////////-----------NACHLING--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class Nachling : public Spaceship
{
public:
  Nachling(StudentWorld* world, int start_x, int start_y, int health);
  virtual void do_something(void);
  virtual ~Nachling();
  
private:
  unsigned int m_state;
};


///////////////////////////////////////////////////////////////////////////
///////////////////-----------WEALTHY NACHLING--------------///////////////
///////////////////////////////////////////////////////////////////////////

class WealthyNachling : public Nachling
{
public:
  WealthyNachling(int start_x, int start_y, StudentWorld* world, int health);
  virtual void do_something(void);
  virtual ~WealthyNachling();
  
private:
};

///////////////////////////////////////////////////////////////////////////
////////////////////////-----------SMALLBOT--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class Smallbot : public Spaceship
{
public:
  Smallbot(int start_x, int start_y, StudentWorld* world, int health);
  virtual void do_something(void);
  virtual ~Smallbot();
  
private:
};

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------GOODIE--------------///////////////////
///////////////////////////////////////////////////////////////////////////

class Goodie : public Actor
{
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

class Energy : public Goodie
{
public:
  Energy(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~Energy();
  
private:
};

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------EXTRA LIVE--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class ExtraLive : public Goodie
{
public:
  ExtraLive(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~ExtraLive();
  
private:
};

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------EXTRA TORPEDO--------------////////////////
///////////////////////////////////////////////////////////////////////////

class ExtraTorpedo : public Goodie
{
public:
  ExtraTorpedo(int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  virtual ~ExtraTorpedo();
  
private:
};

///////////////////////////////////////////////////////////////////////////
////////////////////////-----------BULLET--------------////////////////////
///////////////////////////////////////////////////////////////////////////

class SepticBullet : public Actor
{
public:
  SepticBullet(int start_x, int start_y, StudentWorld* world, bool player_spaceship_bullet, int image_id=IID_BULLET);
  virtual void do_something(void);
  bool get_projectile_viewpoint(void) const;        // Returns if the bullet (or torpedo) belongs to the player spaceship or an alien
  unsigned int get_attack_power(void) const;        // Returns the attack power of the bullet object
  void set_attack_power(unsigned int value);        // Sets the attack power of the bullet object
  virtual ~SepticBullet();
  
private:
  bool m_spaceship_bullet;
  unsigned int m_attack_power;
};

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------TORPEDO--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class FlatulenceTorpedo : public SepticBullet
{
public:
  FlatulenceTorpedo(int start_x, int start_y, StudentWorld* world, bool player_spaceship_bullet);
  virtual ~FlatulenceTorpedo();
  
private:
};

#endif // _ACTOR_H_
