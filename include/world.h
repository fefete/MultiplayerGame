#pragma once

/*

  Singleton class working as main world of the app, contains
  the game loop functions of the game.

*/

#include "Entity.h"
#include "Character.h"
#include <Box2D/Box2D.h>
#include <vector>
#include <mutex>
#include "contactlistener.h"
#include "networking.h"
#include "BallCharacter.h"
#include "PlayerCharacter.h"

#include "imgui.h"
#include "imgui-sfml.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>



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
  void worldSync();
  void worldDisconnect();
  int numberOfCharactersInPlay();

  sf::Texture* playerTexture();
  sf::Texture* wallTexture();
  sf::Texture* groundTexture();
  sf::Texture* ballTexture();
  sf::Texture* sideWallTexture();
  sf::Text fontTexture();

  void setBall(Ball*);
  void worldSetSendSystemData(std::string s);
  void addCharacterToWorld(Entity* character);
  void deleteCharacterFromWorld();

  double last_delta;

  sf::RenderWindow* getWindow();
  b2World* getPhysicsWorld();
  Ball* getBall();

  bool logged_in;
  PlayerCharacter* localPlayer;
  
private:

  World();
  ~World();

  std::vector<Entity*> m_entites_to_draw;
  std::vector<PlayerCharacter*>m_characters_in_world;
  //BallCharacter* m_ball_;

  b2World* m_physics_world_;
  
  //networking
  void sendTCPPacket();
  void recieveTCPPacket();
  void sendUDPPacket();
  void recieveUDPPacket();
  void feedUDPSendPackage();

  sf::RenderWindow* m_window_;

  static World* m_current_world;

  netUDPPackage* m_send_p_ball;
  netUDPPackage* m_send_p_player;
  netTCPPackage* m_send_p_system;

  netUDPPackage* m_recieve_UDP;
  netTCPPackage* m_recieve_TCP;


  sf::TcpSocket m_tcp;
  sf::UdpSocket m_udp;


  CollisionCallback* m_cb_;

  Ball* m_b;
  sf::Texture ball_t;

  char username[255];
  char password[255];

  //load textures
  sf::Texture ground_t;
  sf::Texture box_t;
  sf::Texture box_t_2;
  sf::Texture wall_t;
  sf::Texture side_wall_t;
  sf::Text text_score_1;
  sf::Text text_score_2;
  sf::Text text_score_3;
  
  float hid_max_score_time;

  int id;

  std::mutex mutex;

  sf::Font font;

  static bool disconnect;
  static bool has_to_disconnect;
};

