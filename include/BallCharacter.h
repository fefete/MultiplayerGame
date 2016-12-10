#pragma once

#include "character.h"

class Ball : public Entity
{
public:
  Ball();
  ~Ball();

  //radius should be the half of the width / height of the image
  void init(vec2D position, float radius, sf::Texture&);
  void draw();
private:

};

