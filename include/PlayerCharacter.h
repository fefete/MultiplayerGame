#pragma once

#include "character.h"
#include "types.h"
#include <vector>
class World;
class PlayerCharacter : public Entity
{
  friend World;
public:
  PlayerCharacter();
  ~PlayerCharacter();
  void init(vec2D position, vec2D size, sf::Texture&);
  void readInput();
  void update(float dt);
  void draw();
  void throwBall();
  void dash();

  void beginContact(Entity* contacted);
  void exitContact(Entity* contacted);

  bool can_jump;
  bool has_ball;

  float dash_cd;
  float time_dashing;

  bool input_right;
  bool input_left;
  bool input_jump;
  bool input_dash;


  int id;
protected:
  std::vector<kPlayerInputState> movement_state_;

};

