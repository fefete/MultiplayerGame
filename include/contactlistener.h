#pragma once

#include "Box2D\Box2D.h"

class CollisionCallback : public b2ContactListener
{
public:
  CollisionCallback();
  ~CollisionCallback();

  void BeginContact(b2Contact* c);
  void EndContact(b2Contact* c);

private:

};

