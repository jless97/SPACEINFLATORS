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

Spaceship::Spaceship(StudentWorld* world, int image_id, int start_x, int start_y, unsigned int health)
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
      case KEY_PRESS_UP: if (y <= VIEW_HEIGHT - 1) { move_to(x, y + 1); } update_bullet_shoot(false); update_torpedo_shoot(false); break;
      case KEY_PRESS_DOWN: if (y >= 0) { move_to(x, y - 1); } update_bullet_shoot(false); update_torpedo_shoot(false); break;
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

void Spaceship::update_health(unsigned int how_much) { m_health += how_much; }

void Spaceship::update_torpedoes(unsigned int how_much) { m_torpedoes += how_much; }

void Spaceship::update_bullet_shoot(bool value) { m_bullet_shoot = value; }

void Spaceship::update_torpedo_shoot(bool value) { m_torpedo_shoot = value; }

unsigned int Spaceship::get_health(void) const { return m_health; }

unsigned int Spaceship::get_torpedoes(void) const { return m_torpedoes; }

bool Spaceship::get_bullet_shoot(void) const { return m_bullet_shoot; }

bool Spaceship::get_torpedo_shoot(void) const { return m_torpedo_shoot; }

Spaceship::~Spaceship() { set_visible(false); }

///////////////////////////////////////////////////////////////////////////
////////////////////////-----------NACHLING--------------//////////////////
///////////////////////////////////////////////////////////////////////////

Nachling::Nachling(StudentWorld* world, int start_x, int start_y)
: Spaceship(world, IID_NACHLING, start_x, start_y, (world->get_round() * 5)), m_state(0) { set_visible(true); world->add_actor(this); }

void Nachling::do_something(void)
{
  
  return;
}

Nachling::~Nachling() { set_visible(false); }

///////////////////////////////////////////////////////////////////////////
///////////////////-----------WEALTHY NACHLING--------------///////////////
///////////////////////////////////////////////////////////////////////////

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
{ set_visible(true); world->add_actor(this); }

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

SepticBullet::~SepticBullet() { set_visible(false); }

///////////////////////////////////////////////////////////////////////////
/////////////////////////-----------TORPEDO--------------//////////////////
///////////////////////////////////////////////////////////////////////////

FlatulenceTorpedo::FlatulenceTorpedo(int start_x, int start_y, StudentWorld* world, bool player_spaceship_bullet)
: SepticBullet(start_x, start_y, world, player_spaceship_bullet, IID_TORPEDO) { set_attack_power(8); }

FlatulenceTorpedo::~FlatulenceTorpedo() { set_visible(false); }
