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
  void update_health(int how_much);         // Update health when spaceships either gain/lose health points
  void update_torpedoes(int how_much);      // Update the number of torpedoes a spaceship currently has
  void update_bullet_shoot(bool value);     // Update the state of whether the player fired a bullet on the previous tick
  int get_health(void) const;               // Returns the current health of the spaceship
  int get_torpedoes(void) const;            // Returns the current torpedo count of the spaceship
  bool get_bullet_shoot(void) const;        // Returns true if the player just fired a bullet on the previous tick
  void set_health(int value);               // Sets the health of the spaceship
  virtual ~Spaceship();
  
private:
  int m_health;
  int m_torpedoes;
  bool m_bullet_shoot;
};

///////////////////////////////////////////////////////////////////////////
////////////////////////-----------NACHLING--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class Nachling : public Spaceship
{
public:
  enum Direction {
    none = 0,
    up,
    down,
    left,
    right
  };
  Nachling(StudentWorld* world, int start_x, int start_y, int health, int image_id=IID_NACHLING);
  virtual void do_something(void);
  virtual bool is_wealthy(void) const;             // Returns true if the Nachling is wealthy
  virtual bool is_smallbot(void) const;            // Returns true if the alien is a Smallbot
  virtual bool check_malfunctioning(void);         // For the WealthyNachling
  virtual void do_smallbot_movement(void);      // Implements the Smallbot movement algorithm
  void update_horizontal_movement_remaining(int how_much); // Updates the horizontal steps the Nachling has left to take
  bool get_active(void) const;                     // Returns true if the Nachling can do something this tick
  unsigned int get_state(void) const;              // Returns the current state of the Nachling
  unsigned int get_horizontal_movement_distance(void) const; // Returns the number of horizontal steps that a Nachling can take
  int get_horizontal_movement_remaining(void) const; // Returns the number of horizontal steps the Nachling has left to take
  Nachling::Direction get_horizontal_movement_direction(void) const; // Returns the direction that the Nachling will move in
  void set_active(bool value);                     // Flips the status of the Nachling's status to do a move this tick
  void set_state(unsigned int value);              // Sets the state of the Nachling to do a specified value
  void set_horizontal_movement_distance(unsigned int value); // Sets the number of horizontal steps that a Nachling can take
  void set_horizontal_movement_remaining(unsigned int value); // Sets the number of horizontal steps the Nachling has left to take
  void set_horizontal_movement_direction(Nachling::Direction dir); // Sets the horizontal direction that the Nachling will move in
  unsigned int compute_mdb(int x) const;           // Computes the Minimum Distance to Border (MDB) value for the Nachling
  virtual ~Nachling();
  
private:
  bool         m_active;                           // Nachlings can only do something every other tick
  unsigned int m_state;                            // Nachlings do specific actions depending on their current state
  unsigned int m_horizontal_movement_distance;     // The number of horizontal steps the Nachling will take
  int m_horizontal_movement_remaining;    // The number of horizontal steps the Nachling has left to take (before re-evaluating)
  Direction m_horizontal_movement_direction;       // The horizontal direction that the Nachling will move in
};

///////////////////////////////////////////////////////////////////////////
///////////////////-----------WEALTHY NACHLING--------------///////////////
///////////////////////////////////////////////////////////////////////////

class WealthyNachling : public Nachling
{
public:
  WealthyNachling(StudentWorld* world, int start_x, int start_y, int health, int image_id=IID_WEALTHY_NACHLING);
  virtual bool is_wealthy(void) const;          // Returns true if the Nachling is wealthy
  virtual bool is_smallbot(void) const;         // Returns true if the alien is a Smallbot
  virtual bool check_malfunctioning(void);      // Check if the WealthyNachling is malfunctioning
  virtual void do_smallbot_movement(void);      // Implements the Smallbot movement algorithm
  void update_resting_ticks(int how_much);      // Updates the number of resting ticks
  bool get_malfunctioning_state(void) const;    // Returns if the WealthyNachling is malfunctioning
  int get_resting_ticks(void) const;            // Returns the number of resting ticks
  void set_malfunctioning_state(bool value);    // Sets the state of whether the WealthyNachling is malfunctioning
  void set_resting_ticks(int value);            // Sets the number of resting ticks
  virtual ~WealthyNachling();
  
private:
  bool m_malfunctioning_state; // Check if the WealthyNachling is malfunctioning
  int  m_resting_ticks;        // If WealthyNachling starts malfunctioning, it has to wait this amount of ticks
};

///////////////////////////////////////////////////////////////////////////
////////////////////////-----------SMALLBOT--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class Smallbot : public Nachling
{
public:
  Smallbot(StudentWorld* world, int start_x, int start_y, int health, int image_id=IID_SMALLBOT);
  virtual bool is_wealthy(void) const;          // Returns true if the Nachling is wealthy
  virtual bool is_smallbot(void) const;         // Returns true if the alien is a Smallbot
  virtual void do_smallbot_movement(void);      // Implements the Smallbot movement algorithm
  bool get_hit_by_player_status(void) const;    // Returns if the Smallbot was just hit by a player projectile
  void set_hit_by_player_status(bool value);    // Sets the status if the Smallbot was hit by a player projectile
  virtual ~Smallbot();
  
private:
  bool m_hit_by_player;   // Tells the Smallbot if it was just hit by a player projectile
};

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------GOODIE--------------///////////////////
///////////////////////////////////////////////////////////////////////////

class Goodie : public Actor
{
public:
  Goodie(int image_id, int start_x, int start_y, StudentWorld* world);
  virtual void do_something(void);
  void update_ticks(int how_much);       // Increment/decrement the ticks before the goodie disappears
  void update_active(int how_much);      // Decrements the count before the goodie can move down the space field
  int get_total_ticks(void) const;       // Returns the total number of ticks a goodie can stay in the space field for a given round
  int get_ticks(void) const;             // Returns the number of ticks a goodie has before it disappears
  int get_active(void) const;            // Returns the current count of the countdown before goodie can move down
  void set_total_ticks(int value);       // Sets the number of ticks a goodie can stay for a given round before disappearing
  void set_ticks(int value);             // Sets the number of ticks before a goodie disappears
  void set_active(int value);            // Resets the counter before the goodie can move down again
  virtual ~Goodie();
  
private:
  int m_total_ticks;             // The total number of ticks a goodie can stay in the field for a given round
  int m_nticks_before_disappear; // The number of ticks left before the goodie disappears
  int m_active;                  // A goodie can only move every n ticks (so as to move slowly down the space field)
};

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------FREE SHIP--------------//////////////////
///////////////////////////////////////////////////////////////////////////

class FreeShip : public Goodie
{
public:
  FreeShip(int start_x, int start_y, StudentWorld* world);
  virtual ~FreeShip();
  
private:
};

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------ENERGY--------------///////////////////
///////////////////////////////////////////////////////////////////////////

class Energy : public Goodie
{
public:
  Energy(int start_x, int start_y, StudentWorld* world);
  virtual ~Energy();
  
private:
};

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------EXTRA TORPEDO--------------////////////////
///////////////////////////////////////////////////////////////////////////

class FreeTorpedo : public Goodie
{
public:
  FreeTorpedo(int start_x, int start_y, StudentWorld* world);
  virtual ~FreeTorpedo();
  
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
