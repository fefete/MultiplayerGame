#pragma once

#include "Box2D\Box2D.h"

#define SCALE 30 //pass from pixels to box2D positions

#define TIME_STEP 1.0f / 60.0f

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 600


enum kPlayerInputState
{
  kPlayerInputState_right = 0,
  kPlayerInputState_left,
  kPlayerInputState_jump,
  kPlayerInputState_dash,
  kPlayerInputState_throw,
};

struct netUDPPackage {

  std::string system_message;

};

struct netTCPPackage {

  std::string system_message;

};



struct vec2D {

  float x;
  float y;

};

enum kEntityType
{
  kEntityType_player = 0,
  kEntityType_ball,
  kEntityType_wall,
};