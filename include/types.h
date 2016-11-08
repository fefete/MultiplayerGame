#pragma once

#define SCALE 30 //pass from pixels to box2D positions

enum kPkayerInputState
{
  kPkayerInputState_right = 0,
  kPkayerInputState_left,
  kPkayerInputState_jump,
};

struct vec2D {

  float x;
  float y;

};