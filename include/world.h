#pragma once

/*

  Singleton class working as main world of the app, contains
  the game loop functions of the game.

*/

#include "Entity.h"
#include "Character.h"
#include <Box2D/Box2D.h>
#include <vector>
#include "networking.h"

class World
{
public:
  //gets the world if exists, if not, creates one
  static World* getWorld();


  //inits the box2d physics world with a specified gravity
  void worldInit(b2Vec2 gravity);

  //loop functions
  void worldUpdate(float dt);
  void worldDraw();
  void wordReadInput();
  void worldPollEvents();
  void addCharacterToWorld(Entity* character);
  void deleteCharacterFromWorld();
  sf::RenderWindow* getWindow();
  b2World* getPhysicsWorld();

private:

  World();
  ~World();

  std::vector<Entity*> m_entites_to_draw;
  std::vector<Character*>m_characters_in_world;
  //BallCharacter* m_ball_;
  b2World* m_physics_world_;
  
  //networking
  void sendPacket();
  void recievePacket();


  sf::RenderWindow* m_window_;

  static World* m_current_world;

  netPackage* m_send_p;
  netPackage* m_recieve_p;

  SOCKET m_tcp;
  SOCKET m_udp;
};

