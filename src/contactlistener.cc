#include "contactlistener.h"
#include "PlayerCharacter.h"
#include "BallCharacter.h"
#include "wall.h"
#include "entity.h"
CollisionCallback::CollisionCallback()
{
}

CollisionCallback::~CollisionCallback()
{
}

void CollisionCallback::BeginContact(b2Contact* c) {
  //if A or B are not entities, something went really really bad
  void* raw_A = c->GetFixtureA()->GetBody()->GetUserData();
  Entity* A = static_cast<Entity*>(raw_A);
  if (!A) {
    return;
  }
  void* raw_B = c->GetFixtureB()->GetBody()->GetUserData();

  Entity* B = static_cast<Entity*>(raw_B);
  if (!B) {
    return;
  }
  //if there's no player involved, nothing to do
  if (A->type_ != kEntityType_player && B->type_ != kEntityType_player) {
    return;
  }
  if (A->type_ == kEntityType_player)
  {
    PlayerCharacter* P_A = static_cast<PlayerCharacter*>(A);
    P_A->beginContact(B);
    return;
  }
  else if(B->type_ == kEntityType_player){
    PlayerCharacter* P_B = static_cast<PlayerCharacter*>(B);
    P_B->beginContact(A);
    return;
  }
}

void CollisionCallback::EndContact(b2Contact* c) {
  //if A or B are not entities, something went really really bad
  void* raw_A = c->GetFixtureA()->GetBody()->GetUserData();
  Entity* A = static_cast<Entity*>(raw_A);
  if (!A) {
    return;
  }
  void* raw_B = c->GetFixtureB()->GetBody()->GetUserData();

  Entity* B = static_cast<Entity*>(raw_B);
  if (!B) {
    return;
  }
  //if there's no player involved, nothing to do
  if (A->type_ != kEntityType_player && B->type_ != kEntityType_player) {
    return;
  }
  if (A->type_ == kEntityType_player)
  {
    PlayerCharacter* P_A = static_cast<PlayerCharacter*>(A);
    P_A->exitContact(B);
    return;
  }
  else if (B->type_ == kEntityType_player) {
    PlayerCharacter* P_B = static_cast<PlayerCharacter*>(B);
    P_B->exitContact(A);
    return;
  }
}