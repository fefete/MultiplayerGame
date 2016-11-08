#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class Character
{
public:
  Character();
  ~Character();

  b2BodyDef body_def_;
  b2Body* body_;
  b2PolygonShape shape_;
  b2FixtureDef fixture_def;
  sf::Sprite sprite_;
  

  virtual void update(float dt) = 0;
  virtual void draw(b2Body*) = 0;

protected:
};

