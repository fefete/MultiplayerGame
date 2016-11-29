#include "world.h"
#include <thread>
World* World::m_current_world;

World::World()
{
}

World::~World()
{
}


void World::sendPacket() {
  while(true){
    if(m_send_p != nullptr) {
      printf("\nIN\n");
      if (sendTCP(m_tcp, "a", *m_send_p) != 0) {
        printf("\nFAILURE SENDING\n");
      };
      delete m_send_p;
      m_send_p = nullptr;
    }
  }
}

void World::recievePacket() {

}

void World::worldInit(b2Vec2 gravity)
{
  m_window_ = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML test!");
  m_window_->setFramerateLimit(30);
  m_physics_world_ = new b2World(gravity);

  m_send_p = nullptr;
  m_recieve_p = nullptr;

  if (!initWS()) {
    printf("\nFAILURE AT INIT WINSOCK\n");
  }
  if (connect(&m_tcp, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, "127.0.0.1") != 0) {
    printf("FAILURE CREATING SOCKET TCP");
  }
  new std::thread(&World::recievePacket, this);
  new std::thread(&World::sendPacket, this);

  m_send_p = new netPackage();
  
}

void World::wordReadInput()
{
  for (int i = 0; i < m_characters_in_world.size(); i++) {
    m_characters_in_world[i]->readInput();
  }
}

void World::worldPollEvents()
{
  sf::Event event;
  while (m_window_->pollEvent(event))
  {
    // Close window: exit
    if (event.type == sf::Event::Closed)
      m_window_->close();
  }
}

void World::addCharacterToWorld(Entity * entity)
{
  m_entites_to_draw.push_back(entity);
  if (dynamic_cast<Character*>(entity)) {
    m_characters_in_world.push_back((Character*)entity);
  }
}

void World::deleteCharacterFromWorld()
{
}

sf::RenderWindow* World::getWindow()
{
  return m_window_;
}

b2World * World::getPhysicsWorld()
{
  return m_physics_world_;
}

World * World::getWorld()
{
  if (!m_current_world) {
    m_current_world = new World();
  }
  return m_current_world;
}

void World::worldUpdate(float dt)
{
  for (int i = 0; i < m_characters_in_world.size(); i++) {
    m_characters_in_world[i]->update(dt);
  }
  if(m_send_p == nullptr)
    m_send_p = new netPackage();
}


void World::worldDraw()
{
  m_window_->clear(sf::Color::White);
  for (int i = 0; i < m_entites_to_draw.size(); i++) {
    m_entites_to_draw[i]->draw();
    m_window_->draw(m_entites_to_draw[i]->sprite_);
  }
  m_window_->display();
}

