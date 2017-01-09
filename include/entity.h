#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "types.h"
class World;
class Entity
{
  friend World;
public:
  Entity();
  ~Entity();

  b2BodyDef body_def_;
  b2Body* body_;
  b2PolygonShape shape_;
  b2CircleShape c_shape_;
  b2FixtureDef fixture_def;
  sf::Sprite sprite_;
  int m_size;


  kEntityType type_;

  std::string name;

  virtual void draw() = 0;
  virtual void readInput(){};
  virtual void update(float dt);

  void physicObjectState(bool state);
  void drawableObjectState(bool state);

protected:
  bool m_can_draw;
  bool m_is_active;
};

