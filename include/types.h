#pragma once

#include "Box2D\Box2D.h"

#define SCALE 30 //pass from pixels to box2D positions


enum kPkayerInputState
{
  kPkayerInputState_right = 0,
  kPkayerInputState_left,
  kPkayerInputState_jump,
};

struct netPackage{

  int id;
  std::vector<kPkayerInputState> input;
  b2Velocity vel;
  b2Position pos;

};

struct vec2D {

  float x;
  float y;

};