/*******************************************************************************
 *GameController.cpp
 *
 * The implementation of the game framework for Space Inflators
 *
 *
 ******************************************************************************/

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "freeglut.h"
#include "GameController.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "SoundFX.h"
#include "SpriteManager.h"
#include <string>
#include <map>
#include <utility>
#include <cstdlib>
#include <algorithm>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------GLOBALS--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

static const double SCORE_Y = 3.8;
static const double SCORE_Z = -10;

static const int MS_PER_FRAME = 10;

static const double VISIBLE_MIN_X = -3.25;
static const double VISIBLE_MAX_X = 3.25;
static const double VISIBLE_MIN_Y = -2;
static const double VISIBLE_MAX_Y = 2;
static const double VISIBLE_MIN_Z = -20;
static const double VISIBLE_MAX_Z = -6;

static const double FONT_SCALEDOWN = 760.0;

static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;

static const int PERSPECTIVE_NEAR_PLANE = 4;
static const int PERSPECTIVE_FAR_PLANE  = 22;

static const double PI = 4 * atan(1.0);

struct Point
{
	double dx;
	double dy;
};

static void drawPrompt(string mainMessage, string secondMessage);
static void drawScoreAndLives(string);

static void drawPlayer(GraphObject* go);
static void drawNachling(GraphObject* go);
static void drawWealthyNachling(GraphObject* go);
static void drawSmallbot(GraphObject* go);
static void drawBullet(GraphObject* go);
static void drawTorpedo(GraphObject* go);
static void drawGoodie(GraphObject* go);
static void drawStar(GraphObject* go);

///////////////////////////////////////////////////////////////////////////
///////////////-----------FUNCTION DEFINITIONS--------------///////////////
///////////////////////////////////////////////////////////////////////////

void GameController::init_drawers_and_sounds()
{
	DrawMapType::value_type drawers[] = {
		make_pair(IID_PLAYER_SHIP      , &drawPlayer),
		make_pair(IID_NACHLING         , &drawNachling),
		make_pair(IID_WEALTHY_NACHLING , &drawWealthyNachling),
		make_pair(IID_SMALLBOT         , &drawSmallbot),
		make_pair(IID_BULLET           , &drawBullet),
		make_pair(IID_TORPEDO          , &drawTorpedo),
		make_pair(IID_FREE_SHIP_GOODIE , &drawGoodie),
		make_pair(IID_ENERGY_GOODIE    , &drawGoodie),
		make_pair(IID_TORPEDO_GOODIE   , &drawGoodie),
		make_pair(IID_STAR             , &drawStar),
	};

	SoundMapType::value_type sounds[] = {
		make_pair(SOUND_ENEMY_DIE              , "explode2.wav"),
		make_pair(SOUND_PLAYER_DIE             , "explode2.wav"),
		make_pair(SOUND_PLAYER_FIRE            , "laser.wav"),
		make_pair(SOUND_ENEMY_FIRE             , "laser.wav"),
		make_pair(SOUND_GOT_GOODIE             , "goodie.wav"),
		make_pair(SOUND_PLAYER_HIT             , "clank.wav"),
		make_pair(SOUND_ENEMY_HIT              , "damage.wav"),
		make_pair(SOUND_ENEMY_PLAYER_COLLISION , "explode.wav"),
		make_pair(SOUND_PLAYER_TORPEDO         , "torpedo.wav"),
        make_pair(SOUND_THEME                  , "theme.wav"),
	};
	
	for (int k = 0; k < sizeof(drawers)/sizeof(drawers[0]); k++)
		m_draw_map[drawers[k].first] = drawers[k].second;
	for (int k = 0; k < sizeof(sounds)/sizeof(sounds[0]); k++)
		m_sound_map[sounds[k].first] = sounds[k].second;
}

static void do_somethingCallback()
{
	Game().do_something();
}

static void reshapeCallback(int w, int h)
{
	Game().reshape(w, h);
}

static void keyboard_eventCallback(unsigned char key, int x, int y)
{
	Game().keyboard_event(key, x, y);
}

static void specialkeyboard_eventCallback(int key, int x, int y)
{
	Game().specialkeyboard_event(key, x, y);
}

static void timerFuncCallback(int val)
{
    Game().do_something();
    glutTimerFunc(MS_PER_FRAME, timerFuncCallback, 0);
}

void GameController::run(int argc, char* argv[], GameWorld* gw, std::string window_title)
{
	gw->set_controller(this);
	m_gw = gw;
	set_game_state(welcome);
	m_last_key_hit = INVALID_KEY;
	m_single_step = false;
	m_cur_intra_frame_tick = 0;
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(window_title.c_str());
    
	init_drawers_and_sounds();

	glutKeyboardFunc(keyboard_eventCallback);
	glutSpecialFunc(specialkeyboard_eventCallback);
	glutReshapeFunc(reshapeCallback);
	glutDisplayFunc(do_somethingCallback);
	glutTimerFunc(MS_PER_FRAME, timerFuncCallback, 0);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
    delete m_gw;
}

void GameController::keyboard_event(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a': case '4': m_last_key_hit = KEY_PRESS_LEFT;  break;
		case 'd': case '6': m_last_key_hit = KEY_PRESS_RIGHT; break;
		case 'w': case '8': m_last_key_hit = KEY_PRESS_UP;    break;
		case 's': case '2': m_last_key_hit = KEY_PRESS_DOWN;  break;
		case 't':           m_last_key_hit = KEY_PRESS_TAB;   break;
		case 'f':           m_single_step = true;            break;
		case 'r':           m_single_step = false;           break;
        case 'q': case 'Q': set_game_state(quit);				break;
		default:            m_last_key_hit = key;             break;
	}
}

void GameController::specialkeyboard_event(int key, int /* x */, int /* y */)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:  m_last_key_hit = KEY_PRESS_LEFT;  break;
		case GLUT_KEY_RIGHT: m_last_key_hit = KEY_PRESS_RIGHT; break;
		case GLUT_KEY_UP:    m_last_key_hit = KEY_PRESS_UP;    break;
		case GLUT_KEY_DOWN:  m_last_key_hit = KEY_PRESS_DOWN;  break;
		default:             m_last_key_hit = INVALID_KEY;     break;
	}
}

void GameController::play_sound(int sound_id)
{
    if (sound_id == SOUND_NONE)
        return;
    
    SoundMapType::const_iterator p = m_sound_map.find(sound_id);
    if (p != m_sound_map.end())
    {
        string path = m_gw->asset_directory();
        if (!path.empty())
            path += '/';
        SoundFX().playClip(path + p->second);
    }
}

void GameController::do_something()
{
    switch (m_game_state)
    {
        case welcome:
            play_sound(SOUND_THEME);
            m_main_message = "Welcome to Space Inflators!";
            m_second_message = "Press Enter to begin play...";
            m_game_state = prompt;
            m_next_state_after_prompt = init;
            break;
        case contgame:
            m_main_message = "You lost a life!";
            m_second_message = "Press Enter to continue playing...";
            m_game_state = prompt;
            m_next_state_after_prompt = clean_up;
            break;
        case makemove:
            m_cur_intra_frame_tick = ANIMATION_POSITIONS_PER_TICK;
            if (m_gw->move() != GWSTATUS_PLAYER_DIED)
                m_game_state = animate;
            else if (m_gw->is_game_over())
                m_game_state = gameover;
            else
                m_game_state = contgame;
            break;
        case animate:
            display_game_play();
            if (m_cur_intra_frame_tick-- <= 0)
            {
                int key;
                if (!m_single_step  ||  get_last_key(key))
                    m_game_state = makemove;
            }
            break;
        case clean_up:
            m_gw->clean_up();
            m_game_state = init;
            break;
        case gameover:
        {
            ostringstream oss;
            oss << "Game Over! Your final score was " << m_gw->get_score() << "!";
            m_main_message = oss.str();
        }
            m_second_message = "Press Enter to quit...";
            m_game_state = prompt;
            m_next_state_after_prompt = quit;
            break;
        case prompt:
            drawPrompt(m_main_message, m_second_message);
        {
            int key;
            if (get_last_key(key) && key == '\r')
                m_game_state = m_next_state_after_prompt;
        }
            break;
        case init:
            m_gw->init();
            SoundFX().abortClip();
            m_game_state = makemove;
            break;
		case quit:
            glutLeaveMainLoop();
            break;
        case not_applicable:
            break;
	}
}

void GameController::display_game_play()
{
	glEnable(GL_DEPTH_TEST); // must be done each time before displaying graphics or gets disabled for some reason
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	
	std::set<GraphObject*>& graph_objects = GraphObject::get_graph_objects();
	for (std::set<GraphObject*>::iterator it = graph_objects.begin(); it != graph_objects.end(); it++)
	{
		GraphObject* cur = *it;
		if (cur->is_visible())
		{
			cur->animate();
			DrawMapType::const_iterator p = m_draw_map.find(cur->get_id());
			if (p != m_draw_map.end())
				(*p->second)(cur);  // draw routine for the current object
		}
	}
	
	drawScoreAndLives(m_game_stat_text);
	
	glutSwapBuffers();
}

void GameController::reshape (int w, int h) 
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION); 
	glLoadIdentity ();
	gluPerspective(45.0, double(WINDOW_WIDTH) / WINDOW_HEIGHT, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
	glMatrixMode (GL_MODELVIEW); 
} 

static void convertToGlutCoords(double x, double y, double& gx, double& gy, double& gz)
{
	x /= VIEW_WIDTH;
	y /= VIEW_HEIGHT;
	gx = 2 * VISIBLE_MIN_X + .3 + x * 2 * (VISIBLE_MAX_X - VISIBLE_MIN_X);
	gy = 2 * VISIBLE_MIN_Y +      y * 2 * (VISIBLE_MAX_Y - VISIBLE_MIN_Y);
	gz = .6 * VISIBLE_MIN_Z;
}

static void doOutputStroke(GLfloat x, GLfloat y, GLfloat z, GLfloat size, const char* str, bool centered)
{
	if (centered)
	{
		int len = glutStrokeLength(GLUT_STROKE_ROMAN, reinterpret_cast<const unsigned char*>(str)) / FONT_SCALEDOWN;
		x = -len / 2;
		size = 1;
	}
	GLfloat scaledSize = size / FONT_SCALEDOWN;
	glPushMatrix();
	glLineWidth(1);
	glLoadIdentity();
	glTranslatef(x, y, z);
	glScalef(scaledSize, scaledSize, scaledSize);
	for ( ; *str != '\0'; str++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
	glPopMatrix();
}

static void outputStroke(GLfloat x, GLfloat y, GLfloat z, GLfloat size, const char* str)
{
	doOutputStroke(x, y, z, size, str, false);
}

static void outputStrokeCentered(GLfloat y, GLfloat z, const char* str)
{
	doOutputStroke(0, y, z, 1, str, true);
}

static void drawPolyFromBaseXY(double x, double y, Point points[], int nPoints)
{
	double gx, gy, gz;
	convertToGlutCoords(x, y, gx, gy, gz);
	
	double xmult = double(VISIBLE_MAX_X - VISIBLE_MIN_X) / VIEW_WIDTH;
	double ymult = double(VISIBLE_MAX_Y - VISIBLE_MIN_Y) / VIEW_HEIGHT;
	
	glBegin(GL_POLYGON);
	for (int i = 0; i < nPoints; i++)
		glVertex3f(gx + xmult*(points[i].dx-.5), gy + ymult*(points[i].dy-.5), gz);
	glEnd();
}

static void drawLineFromBaseXY(double x, double y, Point points[], int nPoints)
{
	double gx, gy, gz;
	convertToGlutCoords(x, y, gx, gy, gz);
	
	double xmult = double(VISIBLE_MAX_X - VISIBLE_MIN_X) / VIEW_WIDTH;
	double ymult = double(VISIBLE_MAX_Y - VISIBLE_MIN_Y) / VIEW_HEIGHT;
	
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < nPoints; i++)
		glVertex3f(gx + xmult*(points[i].dx-.5), gy + ymult*(points[i].dy-.5), gz);
	glEnd();
}

static void drawPolyFromBaseXYFlat(double x, double y, Point points[], int nPoints, double scale = 1.0)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < nPoints; i++)
	{
		double gx, gy, gz;
		convertToGlutCoords(x+points[i].dx*scale-.5,y+ points[i].dy*scale-.5, gx, gy, gz);
		glVertex3f(gx, gy, gz);
	}
	glEnd();
}

static void drawPrompt(string mainMessage, string secondMessage)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
	glLoadIdentity ();
	outputStrokeCentered(1, -5, mainMessage.c_str());
	outputStrokeCentered(-1, -5, secondMessage.c_str());
	glutSwapBuffers();
}

static void drawScoreAndLives(string gameStatText)
{
	static int RATE = 1;
	static GLfloat rgb[3] = { .6, .6, .6 };
	for (int k = 0; k < 3; k++)
	{
		rgb[k] += (-RATE + rand() % (2*RATE+1)) / 100.0;
		if (rgb[k] < .6)
			rgb[k] = .6;
		else if (rgb[k] > 1.0)
			rgb[k] = 1.0;
	}
	glColor3f(rgb[0], rgb[1], rgb[2]);
	outputStrokeCentered(SCORE_Y, SCORE_Z, gameStatText.c_str());
}

static void drawPlayer(GraphObject* go)
{
	double x, y;
	go->get_animation_location(x,y);
	
	glPushMatrix();
	
	glColor3f(.8, .7, .7);
	glLineWidth(1);
	
	Point playertop[] = { { .35, .3 }, { .5, 1 }, { .65, .3 } };
	drawPolyFromBaseXYFlat(x, y, playertop, sizeof(playertop)/sizeof(playertop[0]));
	Point playerbot[] = { { .2, .0 }, { .5, .6 }, { .8, .0 } };
	drawPolyFromBaseXYFlat(x, y, playerbot, sizeof(playerbot)/sizeof(playerbot[0]));
	
	glColor3f(1, .1, .1);
	Point laser1[] = { { .1, .1 }, { .1, .6 } };
	Point laser2[] = { { .9, .1 }, { .9, .6 } };
	drawLineFromBaseXY(x, y, laser1, sizeof(laser1)/sizeof(laser1[0]));
	drawLineFromBaseXY(x, y, laser2, sizeof(laser2)/sizeof(laser2[0]));
	
	glColor3f(rand() % 100 / 100.0, rand() % 100 / 100.0, rand() % 100 / 100.0);
	double length = rand() % 100 / 100.0 * .8;
	Point exhaust1[] = { { .20, -length }, { .30, 0 }, { .40, -length } };
	Point exhaust2[] = { { .80, -length }, { .70, 0 }, { .60, -length } };
	drawLineFromBaseXY(x, y, exhaust1, sizeof(exhaust1)/sizeof(exhaust1[0]));
	drawLineFromBaseXY(x, y, exhaust2, sizeof(exhaust2)/sizeof(exhaust2[0]));
	
	glPopMatrix();
}

static void drawNachling(GraphObject* go)
{
	double x, y;
	go->get_animation_location(x, y);
	
	glPushMatrix();
	
	glColor3f (1.0, .3, .5);
	glLineWidth(1);
	
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i += 5)
	{
		double theta = i * PI/180;
		double scale = .5;
		double cx = cos(theta) * scale;
		double cy = sin(theta) * scale * .5;
		double gx, gy, gz;
		convertToGlutCoords(x+cx, y+cy, gx, gy, gz);
		glVertex3f(gx, gy, gz);
	}
	glEnd();
	
	glColor3f(rand()%10 * 1.0 / 10, rand()%10 * 1.0 / 10, rand()%10 * 1.0 / 10);
	Point base1[] = { { .15, .4 }, { .25, .4 }, { .25, .6 }, { .15, .6 } };
	drawPolyFromBaseXY(x, y, base1, sizeof(base1)/sizeof(base1[0]));
	
	glColor3f(rand()%10 * 1.0 / 10, rand()%10 * 1.0 / 10, rand()%10 * 1.0 / 10);
	Point base2[] = { { .5, .4 }, { .6, .4 }, { .6, .6 }, { .5, .6 } };
	drawPolyFromBaseXY(x, y, base2, sizeof(base2)/sizeof(base2[0]));
	
	glColor3f(rand()%10 * 1.0 / 10, rand()%10 * 1.0 / 10, rand()%10 * 1.0 / 10);
	Point base3[] = { { .8, .4 }, { .9, .4 }, { .9, .6 }, { .8, .6} };
	drawPolyFromBaseXY(x, y, base3, sizeof(base3)/sizeof(base3[0]));
	
	glPopMatrix();
}

static void drawWealthyNachling(GraphObject* go)
{
	double x, y;
	go->get_animation_location(x, y);

	glPushMatrix();
	
	glColor3f (.3, .3, 1.0);
	glLineWidth(2);
	
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i += 5)
	{
		double theta = i * PI/180;
		double scale = .5;
		double cx = cos(theta) * scale;
		double cy = sin(theta) * scale * .5;
		double gx, gy, gz;
		convertToGlutCoords(x+cx, y+cy, gx, gy, gz);
		glVertex3f(gx, gy, gz);
	}
	glEnd();
	
	glColor3f(1.0, 1.0, 1.0);
	
	double shift;
	int anim = (go->get_animation_number() % 24) / 2;
	if (anim < 6)
		shift = .1 * anim;
	else
		shift = .5 - .1 * (anim-6);
	Point base[] = { { .1+shift, .4 }, { .4+shift, .4 } , { .4+shift, .6 }, { .1+shift, .6 } };
	drawPolyFromBaseXY(x, y, base, sizeof(base)/sizeof(base[0]));
	
	glPopMatrix();
}

static void drawSmallbot(GraphObject* go)
{
	double x, y;
	go->get_animation_location(x, y);

	glPushMatrix();
	
	glColor3f (0, .8, 0.8);
	glLineWidth(1);
	
	Point ship[] = { { 0, 1 }, { .5, 0 }, { 1, 1 } };
	drawPolyFromBaseXY(x, y, ship, sizeof(ship)/sizeof(ship[0]));
	
	glColor3f (rand() % 100 / 100.0, rand() % 100 / 100.0, rand() % 100 / 100.0);
	double length = rand() % 100 / 100.0 * .7;
	Point exhaust1[] = { { .20, 1+length }, { .30, 1.0 }, { .40, 1+length } };
	Point exhaust2[] = { { .80, 1+length }, { .70, 1.0 }, { .60, 1+length } };
	drawLineFromBaseXY(x, y, exhaust1, sizeof(exhaust1)/sizeof(exhaust1[0]));
	drawLineFromBaseXY(x, y, exhaust2, sizeof(exhaust2)/sizeof(exhaust2[0]));
	
	glPopMatrix();
}

static void drawBullet(GraphObject* go)
{
	double x, y;
	go->get_animation_location(x, y);

	glPushMatrix();

	glColor3f (1.0, 0, 0);
	glLineWidth(1);
	
	Point bullet[] = { { .4, .5 }, { .5, .6 }, { .6, .5 }, { .5, .4 } };
	drawPolyFromBaseXY(x, y, bullet, sizeof(bullet)/sizeof(bullet[0]));

	glPopMatrix();
}

static void drawGoodie(GraphObject* go)
{
	double x, y;
	go->get_animation_location(x, y);
	double gx, gy, gz;
	convertToGlutCoords(x, y, gx, gy, gz);
	
	double brightness = go->get_brightness();
	
	glPushMatrix();
	
	glTranslatef(gx, gy, gz);
	glColor3f(0.0*brightness, 0.0*brightness, 1.0*brightness);
	
	glLineWidth(1);

	glBegin( GL_POLYGON );
	double r = .2;
	for( float i = 0; i < 10; i++)
	{
		double theta = 2*PI * i / 10.0;
		double cx = cos(theta) * r;
		double cy = sin(theta) * r;
		glVertex3f(cx, cy, -.01);
	}
	glEnd();
	
	glPopMatrix();
	
	glPushMatrix();
	
	glColor3f(1.0*brightness, 0.2*brightness, 0.3*brightness);
	
	char goodieChar[2] = "";
	switch (go->get_id())
	{
		case IID_FREE_SHIP_GOODIE: goodieChar[0] = 'F'; break;
		case IID_ENERGY_GOODIE:    goodieChar[0] = 'E'; break;
		case IID_TORPEDO_GOODIE:   goodieChar[0] = 'T'; break;
	}
	
	outputStroke(gx, gy, gz, 1, goodieChar);

	glPopMatrix();
}

static void drawStarOrTorpedo(GraphObject* go, bool torpedo)
{
	double x, y;
	go->get_animation_location(x, y);
	
	glPushMatrix();
	
	double length;

	if (torpedo)
	{
		glColor3f(1.0, 0, 0);
		length = (rand() % 100) / 100.0;
	}
	else
	{
		double brightness = (rand()%100) / 100.0;
		if (brightness < .2)
			brightness = .2;
		glColor3f(brightness, brightness, brightness);
		length = (rand() % 100) / 500.0;
	}
	
	glLineWidth(1);
	
	for (int i = 0; i < 5; i++)
	{
		double theta = 2*PI * (rand() % 1000) / 1000.0;
		double dx = cos(theta) * length;
		double dy = sin(theta) * length;
		Point line[] = { { .5, .5 }, { dx+.5, dy+.5 } };
		drawLineFromBaseXY(x, y, line, sizeof(line)/sizeof(line[0]));
	}
	glPopMatrix();
}

static void drawStar(GraphObject* go)
{
	drawStarOrTorpedo(go, false);
}

static void drawTorpedo(GraphObject* go)
{
	drawStarOrTorpedo(go, true);
}
