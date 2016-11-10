#pragma once

#include "entity.h"

class Character : public Entity
{
public:
  Character();
  ~Character();


  

  virtual void update(float dt) = 0;
  virtual void draw();
  virtual void readInput() = 0;


protected:
};

