#pragma once

#include "character.h"
#include "types.h"
#include <vector>

class PlayerCharacter : public Character
{
public:
  PlayerCharacter();
  ~PlayerCharacter();
  void init(vec2D position, vec2D size, sf::Texture&, b2World&);
  void readInput();
  void update();
  void draw(b2Body*);
  void action();


private:
  std::vector<kPkayerInputState> movement_state_;
};

