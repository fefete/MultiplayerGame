#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class Entity
{
public:
  Entity();
  ~Entity();

  b2BodyDef body_def_;
  b2Body* body_;
  b2PolygonShape shape_;
  b2FixtureDef fixture_def;
  sf::Sprite sprite_;

  virtual void draw() = 0;

private:

};

