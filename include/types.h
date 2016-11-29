#pragma once

#include "Box2D\Box2D.h"

#define SCALE 30 //pass from pixels to box2D positions

#define TIME_STEP 1.0f / 60.0f

enum kPkayerInputState
{
  kPkayerInputState_right = 0,
  kPkayerInputState_left,
  kPkayerInputState_jump,
};

struct netPackage{

  int id;
  int32 actions[3];
  float32 vel_x;
  float32 vel_y;
  float32 pos_x;
  float32 pos_y;

};

struct vec2D {

  float x;
  float y;

};