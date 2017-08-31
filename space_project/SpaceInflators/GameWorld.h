/*******************************************************************************
 *GameWorld.h
 *
 * This routine was created by Professor Smallberg to handle the game engine,
 * setting the frequency that the StudentWorld class is accessed and updated,
 * and accessing the assets (i.e. sound effects, images), and updating graphics
 *
 *
 ******************************************************************************/

#ifndef _GAMEWORLD_H_
#define _GAMEWORLD_H_

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "GameConstants.h"
#include <string>

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------GLOBALS--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

const int START_PLAYER_LIVES = 3;

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------GAMEWORLD--------------////////////////////
///////////////////////////////////////////////////////////////////////////

class GameController;

class GameWorld
{
public:

    GameWorld(std::string asset_dir) :m_lives(START_PLAYER_LIVES),m_score(0),m_controller(nullptr),m_asset_dir(asset_dir){}
	virtual ~GameWorld() {}
    virtual void init(void) = 0;                        // Initialize the game world
	virtual int move(void) = 0;                         // Control the game actors once a game begins
	virtual void clean_up(void) = 0;                    // Clean up actors and game field
	void set_game_stat_text(std::string text);
    bool get_key(int& value);
    void play_sound(int sound_id);
    
    /* Functions to be utilized by StudentWorld (and to update game) */
	unsigned int get_lives() const { return m_lives; }
	void dec_lives() { m_lives--; }
	void inc_lives() { m_lives++; }
	unsigned int get_score() const { return m_score; }
	void increase_score(unsigned int how_much) { m_score += how_much; }

    /* Note: The following should be used by only the framework, not the student */
    bool is_game_over() const { return m_lives == 0; } // Check if the player has run out of lives
    void set_controller(GameController* controller) { m_controller = controller; } // Game engine stuff
    std::string asset_directory() const { return m_asset_dir; } // Access of the images and sound effects in the assets directory
private:
	unsigned int	m_lives;
	unsigned int	m_score;
	GameController* m_controller;
    std::string		m_asset_dir;
};

#endif // _GAMEWORLD_H_
