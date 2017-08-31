/*******************************************************************************
 *Actor.cpp
 *
 * This routine is responsible for all of the objects (actors) within the game
 * The main function of each actor is to do something each clock tick
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-08-23 11:11:11 -0700 (Wed, 23 Aug 2017) $
 *
 ******************************************************************************/

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
using namespace std;

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------GLOBALS--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------ACTOR--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

Actor::Actor(int image_id, int start_x, int start_y, StudentWorld* world)
: GraphObject(image_id, start_x, start_y), m_world(world), m_alive(true) { set_visible(false); }

StudentWorld* Actor::world(void) const { return m_world; }

void Actor::set_dead(void) { m_alive = false; }

bool Actor::is_alive(void) const { return m_alive == true; }

Actor::~Actor() {}

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------STAR--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

Star::Star(int start_x, int start_y, StudentWorld* world)
: Actor(IID_STAR, start_x, start_y, world) { set_visible(true); world->add_actor(this); }

void Star::do_something(void)
{
  if (!is_alive()) { return; } // Check the current status of the star object
  
  int x = get_x(), y = get_y(); // Get the current coordinates of the star object
  
  if (y <= 0) { set_dead(); } // If a star object goes below the view height (i.e. y = 0), then remove the star object from the field
  
  move_to(x, y - 1); // If the star object is in a valid position, move one step down the game field
}

Star::~Star() { set_visible(false); }

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------SPACESHIP--------------//////////////////
///////////////////////////////////////////////////////////////////////////

Spaceship::Spaceship(StudentWorld* world, int image_id, int start_x, int start_y, int health)
: Actor(image_id, start_x, start_y, world), m_health(health), m_torpedoes(0), m_bullet_shoot(false), m_torpedo_shoot(false)
{ set_visible(true); }

void Spaceship::do_something(void)
{
  if (!is_alive()) { return; } // Check the current status of the player spaceship
  
  StudentWorld* spaceship_world = world(); // Grab a pointer to the StudentWorld

  // Check to see if the player spaceship has collided with any alien spaceships
  spaceship_world->check_collision(this);
  
  // If the player has lost all energy (i.e. health points), then player loses a life
  if (get_health() <= 0) { set_dead(); spaceship_world->play_sound(SOUND_PLAYER_DIE); }
  
  int x = get_x(), y = get_y(); // Get currents coordinates of the player spaceship
  
  // If player spaceship is still active, then check if there was any user input (i.e. movement or projectiles)
  int user_input;
  if (spaceship_world->get_key(user_input))
  {
    switch (user_input)
    {
      // Directional Input
      case KEY_PRESS_UP: if (y < VIEW_HEIGHT - 1) { move_to(x, y + 1); } update_bullet_shoot(false); update_torpedo_shoot(false); break;
      case KEY_PRESS_DOWN: if (y > 0) { move_to(x, y - 1); } update_bullet_shoot(false); update_torpedo_shoot(false); break;
      case KEY_PRESS_LEFT: if (x > 0) { move_to(x - 1, y); } update_bullet_shoot(false); update_torpedo_shoot(false); break;
      case KEY_PRESS_RIGHT: if (x < VIEW_WIDTH - 1) { move_to(x + 1, y); } update_bullet_shoot(false); update_torpedo_shoot(false); break;
      // Projectile Input
      case KEY_PRESS_SPACE:
        if (get_bullet_shoot()) { update_bullet_shoot(false); return; } // If player fired bullet previous tick, can't fire this tick
        // Else, add new SepticBullet to the space field, play sound, and set state to indicate a bullet was just fired
        else
        {
          update_bullet_shoot(true);
          spaceship_world->play_sound(SOUND_PLAYER_FIRE);
          new SepticBullet(x, y + 1, spaceship_world, true);
        }
        update_torpedo_shoot(false);
        break;
      case KEY_PRESS_TAB:
        if (get_torpedo_shoot()) { update_torpedo_shoot(false); return; } // If player fired torpedo previous tick, can't fire this tick
        // Else, add new FlatulenceTorpedo to the space field, play sound, and set state to indicate a torpedo was just fired
        else
        {
          if (get_torpedoes() >= 1)
          {
            update_torpedo_shoot(true);
            update_torpedoes(-1);
            spaceship_world->play_sound(SOUND_PLAYER_TORPEDO);
            new FlatulenceTorpedo(x, y + 1, spaceship_world, true);
          }
        }
        update_bullet_shoot(false);
        break;
      default:
        break;
    }
  }
  
  // Check to see if the player spaceship has collided with any alien spaceships
  spaceship_world->check_collision(this);
    
  // If the player has lost all energy (i.e. health points), then player loses a life
  if (get_health() <= 0) { set_dead(); spaceship_world->play_sound(SOUND_PLAYER_DIE); }
  
  return;
}

void Spaceship::update_health(int how_much) { m_health += how_much; }

void Spaceship::update_torpedoes(unsigned int how_much) { m_torpedoes += how_much; }

void Spaceship::update_bullet_shoot(bool value) { m_bullet_shoot = value; }

void Spaceship::update_torpedo_shoot(bool value) { m_torpedo_shoot = value; }

int Spaceship::get_health(void) const { return m_health; }

unsigned int Spaceship::get_torpedoes(void) const { return m_torpedoes; }

bool Spaceship::get_bullet_shoot(void) const { return m_bullet_shoot; }

bool Spaceship::get_torpedo_shoot(void) const { return m_torpedo_shoot; }

Spaceship::~Spaceship() { set_visible(false); }

///////////////////////////////////////////////////////////////////////////
////////////////////////-----------NACHLING--------------//////////////////
///////////////////////////////////////////////////////////////////////////

Nachling::Nachling(StudentWorld* world, int start_x, int start_y, int health, int image_id)
: Spaceship(world, image_id, start_x, start_y, health), m_active(true), m_state(0), m_horizontal_movement_distance(0), m_horizontal_movement_remaining(0), m_horizontal_movement_direction(Direction::none)
{ set_visible(true); world->add_actor(this); }

void Nachling::do_something(void)
{
  if (!is_alive()) { return; } // Check the current status of the Nachling
  
  // Function for the WealthyNachling to check if it is malfunctioning
  if (check_malfunctioning())
  {
    return;
  }
  
  StudentWorld* nachling_world = world(); // Grab a pointer to the StudentWorld
  
  // If the Nachling has lost all energy (i.e. health points), then remove the Nachling from the space field
  if (get_health() <= 0) { set_dead(); nachling_world->play_sound(SOUND_ENEMY_DIE); }
  
  if (!get_active()) { set_active(true); return; } // Nachling can't do something this tick, allow it to do something next tick, and return
  
  int x = get_x(), y = get_y(); // Get currents coordinates of the Nachling
  
  // If in state 0
  if (get_state() == 0)
  {
    cout << "State 0" << endl;
    // If Nachling is in line with the player spaceship, and Nachling isn't in far-left or far-right column
    if (nachling_world->in_line_with_player_spaceship(x) && (x != 0) && (x != VIEW_WIDTH - 1))
    {
      set_state(1); // Set Nachling state to 1
      // Set the Horizontal Movement Distance that the Nachling will move in a given direction
      unsigned int current_mdb = compute_mdb(x); // Compute the Nachling's Minimum Distance to a border (i.e. x = 0, or x = 29)
      if (current_mdb > 3) { set_horizontal_movement_distance(nachling_world->rand_int(1, 3)); }
      else { set_horizontal_movement_distance(current_mdb); }
      // Set the Horizontal Movement Direction that the Nachling will move in
      if (nachling_world->rand_int(1, 2) == 1) { set_horizontal_movement_direction(Nachling::Direction::left); }
      else { set_horizontal_movement_direction(Nachling::Direction::right); }
      // Set the Horizontal Movement Remaining steps that the Nachling will take in the direction just calculated
      set_horizontal_movement_remaining(get_horizontal_movement_distance());
    }
    // If Nachling is to the left or to the right of the player spaceship
    else if (nachling_world->rand_int(1, 3) == 1)
    {
      // If Nachling is to the left of the player spaceship, move diagonal right-down, don't allow movement next tick, instantly return
      if (x < nachling_world->get_player_spaceship_x_coord())
      { if (x < VIEW_WIDTH - 1) { move_to(x + 1, y - 1); set_active(false); return; } }
      // If Nacling is to the right of the player spaceship, move diagonal left-down, don't allow movement next tick, instantly return
      else if (x > nachling_world->get_player_spaceship_x_coord())
      { if (x > 0) { move_to(x - 1, y - 1); set_active(false); return; } }
    }
    move_to(x, y - 1);
    // Check if the Nachling went below the bottom of the space field, if so remove Nachling from space field
    if (y < 0) { set_dead(); nachling_world->update_aliens_left_this_round(1); }
    set_active(false); return;
  }
  // If in state 1
  else if (get_state() == 1)
  {
    cout << "State 1" << endl;
    if (nachling_world->get_player_spaceship_y_coord() < y) { set_state(2); } //set_active(false); return; }
    if (get_horizontal_movement_remaining() == 0)
    {
      // Flip the current horizontal direction of the Nachling
      if (get_horizontal_movement_distance() == Nachling::Direction::left) { set_horizontal_movement_direction(Nachling::Direction::right); }
      else { set_horizontal_movement_direction(Nachling::Direction::left); }
      // Set HMR to double the HMD computed in state 0
      set_horizontal_movement_remaining(2 * get_horizontal_movement_distance());
    }
    else { update_horizontal_movement_remaining(-1); }
    // Move 1 step in the current horizontal direction
    if (get_horizontal_movement_direction() == Nachling::Direction::right)
    { if (x < VIEW_WIDTH - 1) { move_to(x + 1, y); } }
    else { if (x > 0) { move_to(x - 1, y); } }
    // Compute the change the Nachling will fire a projectile
    if (nachling_world->rand_int(0, (BASE_CHANCE_OF_FIRING / nachling_world->get_round())) == 0)
    {
      // Fire a SepticBullet
      if (nachling_world->get_bullet_count() < (2 * nachling_world->get_round()))
      {
        if (y > nachling_world->get_player_spaceship_y_coord()) { new SepticBullet(x, y - 1, nachling_world, false); }
      }
    }
    if (nachling_world->rand_int(1, 20) == 1) { set_state(2); }
    set_active(false); return;
  }
  // If in state 2
  else if (get_state() == 2)
  {
    cout << "State 2" << endl;
    if (y == VIEW_HEIGHT - 1) { set_state(0); set_active(false); return; } // If at the top of the screen, set state to 0, and return
    // If at the far left of the screen, move diagonal up-right
    if (x == 0)
    {
      set_horizontal_movement_direction(Nachling::Direction::right);
      move_to(x + 1, y + 1);
    }
    // If at the far right of the screen, move diagonal up-left
    else if (x == VIEW_WIDTH - 1)
    {
      set_horizontal_movement_direction(Nachling::Direction::left);
      move_to(x - 1, y + 1);
    }
    // Else compute random horizontal direction, move diagonal up-direction
    else
    {
      if (nachling_world->rand_int(1, 2) == 1) { set_horizontal_movement_direction(Nachling::Direction::left); move_to(x - 1, y + 1); }
      else { set_horizontal_movement_direction(Nachling::Direction::right); move_to(x + 1, y + 1); }
    }
    set_active(false);
    return;
  }
  
  set_active(false); // After doing something this tick, don't allow Nachling to do something next tick'
  
  return;
}

bool Nachling::check_malfunctioning(void) { return false; }

void Nachling::update_horizontal_movement_remaining(int how_much) { m_horizontal_movement_remaining += how_much; }

bool Nachling::get_active(void) const { return m_active; }

unsigned int Nachling::get_state(void) const { return m_state; }

unsigned int Nachling::get_horizontal_movement_distance(void) const { return m_horizontal_movement_distance; }

int Nachling::get_horizontal_movement_remaining(void) const { return m_horizontal_movement_remaining; }

Nachling::Direction Nachling::get_horizontal_movement_direction(void) const { return m_horizontal_movement_direction; }

void Nachling::set_active(bool value) { m_active = value; }

void Nachling::set_state(unsigned int value) { m_state = value; }

void Nachling::set_horizontal_movement_distance(unsigned int value) { m_horizontal_movement_distance = value; }

void Nachling::set_horizontal_movement_remaining(unsigned int value) { m_horizontal_movement_remaining = value; }

void Nachling::set_horizontal_movement_direction(Nachling::Direction dir) { m_horizontal_movement_direction = dir; }

unsigned int Nachling::compute_mdb(int x) const
{
  int distance_to_left_border, distance_to_right_border;
  distance_to_left_border = x - 0;
  distance_to_right_border = (VIEW_WIDTH - 1) - x;
  return (world()->min(distance_to_left_border, distance_to_right_border));
}

Nachling::~Nachling() { set_visible(false); world()->update_current_aliens_on_screen(-1); world()->update_aliens_left_this_round(-1); }

///////////////////////////////////////////////////////////////////////////
///////////////////-----------WEALTHY NACHLING--------------///////////////
///////////////////////////////////////////////////////////////////////////

WealthyNachling::WealthyNachling(StudentWorld* world, int start_x, int start_y, int health, int image_id)
: Nachling(world, start_x, start_y, health, image_id), m_malfunctioning_state(false), m_resting_ticks(1) {}

bool WealthyNachling::check_malfunctioning(void)
{
  // If WealthyNachling is malfunctioning, update resting ticks
  if (get_malfunctioning_state() && (get_resting_ticks() > 0)) { update_resting_ticks(-1); return true; }

  StudentWorld* wealthy_world = world(); // Grab pointer to StudentWorld
  
  // Check if the WealthyNachling will start malfunctioning, and if so, begin rest period
  if (wealthy_world->rand_int(1, 200) == 1) { set_malfunctioning_state(true); set_resting_ticks(30); return true; }
  
  return false;
}

void WealthyNachling::update_resting_ticks(int how_much) { m_resting_ticks += how_much; }

bool WealthyNachling::get_malfunctioning_state(void) const { return m_malfunctioning_state; }

int WealthyNachling::get_resting_ticks(void) const { return m_resting_ticks; }

void WealthyNachling::set_malfunctioning_state(bool value) { m_malfunctioning_state = value; }

void WealthyNachling::set_resting_ticks(int value) { m_resting_ticks = value; }

WealthyNachling::~WealthyNachling() {}

///////////////////////////////////////////////////////////////////////////
////////////////////////-----------SMALLBOT--------------//////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------GOODIE--------------///////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------ENERGY--------------///////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------EXTRA LIVE--------------//////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------EXTRA TORPEDO--------------////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
////////////////////////-----------BULLET--------------////////////////////
///////////////////////////////////////////////////////////////////////////

SepticBullet::SepticBullet(int start_x, int start_y, StudentWorld* world, bool player_spaceship_bullet, int image_id)
: Actor(image_id, start_x, start_y, world), m_spaceship_bullet(player_spaceship_bullet), m_attack_power(2)
{ set_visible(true); world->add_actor(this); if (!get_projectile_viewpoint()) { world->update_bullet_count(1); } }

void SepticBullet::do_something(void)
{
  if (!is_alive()) { return; } // Check the current status of the bullet object
  
  int x = get_x(), y = get_y(); // Get the current coordinates of the bullet object
  
  StudentWorld* bullet_world = world(); // Grab a pointer to the StudentWorld
  
  // Player projectile
  if (get_projectile_viewpoint())
  {
    bullet_world->check_collision(this, true, false, true); // Check if the bullet collided with any spaceships
    if (!is_alive()) { return; } // Check the current status of the bullet
    // If no collision, then update position
    move_to(x, y + 1);
    if (y >= VIEW_HEIGHT - 1) { set_dead(); }
    if (!is_alive()) { return; } // Check the current status of the bullet
    bullet_world->check_collision(this, true, false, true); // Check if the bullet collided with any spaceships
    if (!is_alive()) { return; } // Check the current status of the bullet
  }
  // Enemy projectile
  else
  {
    bullet_world->check_collision(this, false, true, true); // Check if the bullet collided with any spaceships
    if (!is_alive()) { return; } // Check the current status of the bullet
    // If no collision, then update position
    move_to(x, y - 1);
    if (y <= 0) { set_dead(); }
    if (!is_alive()) { return; } // Check the current status of the bullet
    bullet_world->check_collision(this, false, true, true); // Check if the bullet collided with any spaceships
    if (!is_alive()) { return; } // Check the current status of the bullet
  }
}

bool SepticBullet::get_projectile_viewpoint(void) const { return m_spaceship_bullet == true; }

unsigned int SepticBullet::get_attack_power(void) const { return m_attack_power; }

void SepticBullet::set_attack_power(unsigned int value) { m_attack_power = value; }

SepticBullet::~SepticBullet() { set_visible(false); if (!get_projectile_viewpoint()) { world()->update_bullet_count(-1); } }

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------TORPEDO--------------//////////////////
///////////////////////////////////////////////////////////////////////////

FlatulenceTorpedo::FlatulenceTorpedo(int start_x, int start_y, StudentWorld* world, bool player_spaceship_bullet)
: SepticBullet(start_x, start_y, world, player_spaceship_bullet, IID_TORPEDO) { set_attack_power(8); }

FlatulenceTorpedo::~FlatulenceTorpedo() { set_visible(false); }
