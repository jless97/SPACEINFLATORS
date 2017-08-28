/*******************************************************************************
 *GraphObjects.h
 *
 * This header describes the methods and members of the Game Framework (implemented)
 * by Professor Smallberg to deal with graphics and animation
 *
 *
 ******************************************************************************/

#ifndef _GRAPHOBJ_H_
#define _GRAPHOBJ_H_

#include "SpriteManager.h"
#include "GameConstants.h"

#include <set>
#include <cmath>
 
const int ANIMATION_POSITIONS_PER_TICK = 3;
const int NUM_LAYERS = 4;

inline int round_away_from_zero(double r) {
	double result =  r > 0 ? std::floor(r + 0.5) : std::ceil(r - 0.5);
	return int(result);
	 // If C99 is available, this can be return std::round(r);
}

class GraphObject {
public:
  GraphObject(int image_id, int start_x, int start_y)
   : m_image_id(image_id), m_visible(false), m_x(start_x), m_y(start_y),
     m_dest_x(start_x), m_dest_y(start_y), m_brightness(1.0),
     m_animation_number(0) { get_graph_objects().insert(this); }

  virtual ~GraphObject() { get_graph_objects().erase(this); }
  void set_visible(bool should_i_display) { m_visible = should_i_display; }
  void set_brightness(double brightness) { m_brightness = brightness; }
  // If already moved but not yet animated, use new location anyway.
  int get_x() const { return round_away_from_zero(m_dest_x); }
  // If already moved but not yet animated, use new location anyway.
  int get_y() const { return round_away_from_zero(m_dest_y); }
  void moveTo(int x, int y) { m_dest_x = x; m_dest_y = y; }

  /* Note: The following should be used by only the framework, not the student */
  bool is_visible() const { return m_visible; }
  unsigned int get_id() const { return m_image_id; }
  double get_brightness() const { return m_brightness; }
  unsigned int get_animation_number() const { return m_animation_number; }
  void get_animation_location(double& x, double& y) const { x = m_x; y = m_y; }
  void animate() { m_animation_number++; move_a_little(m_x, m_dest_x); move_a_little(m_y, m_dest_y); }
  static std::set<GraphObject*>& get_graph_objects() {
  static std::set<GraphObject*> graph_objects;
  return graph_objects;
}

private:
  // Prevent copying or assigning graph_objects
  GraphObject(const GraphObject&);
  GraphObject& operator=(const GraphObject&);
  // Actors are derived from the GraphObject (thus they all have the following attributes)
  int    m_image_id;
  bool   m_visible;
  double m_x;
  double m_y;
  double m_dest_x;
  double m_dest_y;
  double m_brightness;
  int    m_animation_number;
  /* Private Routines */
  void move_a_little(double& from, double& to) {
    static const double DISTANCE = 1.0/ANIMATION_POSITIONS_PER_TICK;
    if (to - from >= DISTANCE) { from += DISTANCE; }
    else if (from - to >= DISTANCE) { from -= DISTANCE; }
    else { from = to; }
  }
    
};

#endif // _GRAPHOBJ_H_
