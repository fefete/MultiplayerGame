#pragma once
#include "entity.h"
#include <vector>
#include "types.h"

class Wall : public Entity
{
public:
  Wall();
  ~Wall();

  virtual void draw();
  void init(vec2D position, vec2D size, sf::Texture&);

private:

};


