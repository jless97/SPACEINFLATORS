/*******************************************************************************
 *GameController.h
 *
 * This header was created by Professor Smallberg to handle and contains the 
 * implementation of the game framework and control
 *
 *
 ******************************************************************************/

#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "SpriteManager.h"
#include <string>
#include <map>
#include <iostream>
#include <sstream>

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------GLOBALS--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

enum GameControllerState {
	welcome, contgame, init, clean_up, makemove, animate, gameover, prompt, quit, not_applicable
};

const int INVALID_KEY = 0;

///////////////////////////////////////////////////////////////////////////
///////////////////-----------GAMECONTROLLER--------------/////////////////
///////////////////////////////////////////////////////////////////////////

class GraphObject;
class GameWorld;

class GameController
{
public:
  void run(int argc, char* argv[], GameWorld* gw, std::string window_title);
	bool get_last_key(int& value) {
		if (m_last_key_hit != INVALID_KEY) {
			value = m_last_key_hit;
			m_last_key_hit = INVALID_KEY;
			return true;
		}
    
		return false;
	}
  void play_sound(int sound_id);
  void set_game_stat_text(std::string text) { m_game_stat_text = text; }
  void do_something();
  void reshape(int w, int h);
  void keyboard_event(unsigned char key, int x, int y);
  void specialkeyboard_event(int key, int x, int y);
  void quit_game() { set_game_state(quit); }
	  // Meyers singleton pattern
	static GameController& get_instance() { static GameController instance; return instance; }

private:
  GameWorld*	            m_gw;
  GameControllerState	    m_game_state;
  GameControllerState	    m_next_state_after_prompt;
  int 		                m_last_key_hit;
  bool                      m_single_step;
  std::string	            m_game_stat_text;
  std::string	            m_main_message;
  std::string	            m_second_message;
  int                       m_cur_intra_frame_tick;
  typedef std::map<int, std::string>           SoundMapType;
  typedef std::map<int, void(*)(GraphObject*)> DrawMapType;
  SoundMapType m_sound_map;
  
  /* Private Functions */
  void set_game_state(GameControllerState s) {
    if (m_game_state != quit) { m_game_state = s; }
  }
  void init_drawers_and_sounds();
  void display_game_play();
  DrawMapType  m_draw_map;
};

inline GameController& Game() { return GameController::get_instance(); }

#endif // _GAMECONTROLLER_H_
