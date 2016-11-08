#include "world.h"

World* World::m_current_world;

World::World()
{
}

World::~World()
{
}

void World::worldInit(b2Vec2 gravity)
{
  m_window_ = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML test!");
  m_physics_world_ = new b2World(gravity);
}

void World::wordReadInput()
{
  for (int i = 0; i < m_characters_in_world.size(); i++) {
    m_characters_in_world[i]->readInput();
  }
}

void World::addCharacterToWorld(PlayerCharacter * character)
{
  m_characters_in_world.push_back(character);
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
}

void World::worldDraw()
{
  for (int i = 0; i < m_characters_in_world.size(); i++) {
    m_characters_in_world[i]->readInput();
  }
  m_window_->display();
}

