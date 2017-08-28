/*******************************************************************************
 *GameConstants.h
 *
 * This header was created by Professor Smallberg and contains all necessary
 * constants that are used for the game (i.e. animation, audio, keyboard input, etc.)
 *
 *
 ******************************************************************************/

#ifndef _GAMECONSTANTS_H_
#define _GAMECONSTANTS_H_

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------GLOBALS--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

// IDs for the game objects

const int IID_PLAYER_SHIP      = 0;
const int IID_NACHLING         = 1;
const int IID_WEALTHY_NACHLING = 2;
const int IID_SMALLBOT         = 3;
const int IID_BULLET           = 4;
const int IID_TORPEDO          = 5;
const int IID_FREE_SHIP_GOODIE = 6;
const int IID_ENERGY_GOODIE    = 7;
const int IID_TORPEDO_GOODIE   = 8;
const int IID_STAR             = 9;

// sounds

const int SOUND_ENEMY_DIE              = 0;
const int SOUND_PLAYER_DIE             = 1;
const int SOUND_PLAYER_FIRE            = 2;
const int SOUND_ENEMY_FIRE             = 3;
const int SOUND_GOT_GOODIE             = 4;
const int SOUND_PLAYER_HIT             = 5;
const int SOUND_ENEMY_HIT              = 6;
const int SOUND_ENEMY_PLAYER_COLLISION = 7;
const int SOUND_PLAYER_TORPEDO         = 8;
const int SOUND_THEME				   = 9;

const int SOUND_NONE				   = -1;

// keys the user can hit

const int KEY_PRESS_LEFT  = 1000;
const int KEY_PRESS_RIGHT = 1001;
const int KEY_PRESS_UP    = 1002;
const int KEY_PRESS_DOWN  = 1003;
const int KEY_PRESS_SPACE = ' ';
const int KEY_PRESS_TAB   = '\t';

// board dimensions 

const int VIEW_WIDTH  = 30;
const int VIEW_HEIGHT = 40;

const int SPRITE_WIDTH = VIEW_WIDTH/16;
const int SPRITE_HEIGHT = VIEW_HEIGHT/16;

const double SPRITE_WIDTH_GL = .5; // note - this is tied implicitly to SPRITE_WIDTH due to carey's sloppy openGL programming
const double SPRITE_HEIGHT_GL = .5; // note - this is tied implicitly to SPRITE_HEIGHT due to carey's sloppy openGL programming

// status of each tick (did the player die?)

const int GWSTATUS_PLAYER_DIED	 = 0;
const int GWSTATUS_CONTINUE_GAME = 1;

#endif // _GAMECONSTANTS_H_
