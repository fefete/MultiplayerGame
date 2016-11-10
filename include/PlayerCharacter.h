#pragma once

#include "character.h"
#include "types.h"
#include <vector>

class PlayerCharacter : public Character
{
public:
  PlayerCharacter();
  ~PlayerCharacter();
  void init(vec2D position, vec2D size, sf::Texture&);
  void readInput();
  void update(float dt);
  void draw();
  void action();


private:
  std::vector<kPkayerInputState> movement_state_;
};

