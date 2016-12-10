//
//
//

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "PlayerCharacter.h"
#include "wall.h"
#include "world.h"
#include "assets.h"
#include "entity.h"
#include <iostream>

void logingLoop() {

}
void gameLoop() {

}

int main()
{

  b2Vec2 Gravity(0.f, 9.8f);
  World::getWorld()->worldInit(Gravity);

  //players
  //sf::Sprite sprite(box_t);

  PlayerCharacter* p1 = new PlayerCharacter();
  {
    vec2D pos;
    pos.x = 400;
    pos.y = 300;
    vec2D size;
    size.x = 32;
    size.y = 32;
    p1->init(pos, size, *World::getWorld()->playerTexture());
    World::getWorld()->localPlayer = p1;
    pos.x = 350;
  }

  //walls
  //createGround(*World::getWorld()->getPhysicsWorld(), 400, 500);
  Wall* floor = new Wall();
  {
    vec2D pos, size;
    pos.x = 400;
    pos.y = 600;
    size.x = 1920;
    size.y = 128;
    floor->init(pos, size, *World::getWorld()->groundTexture());
  }

  Wall* cellar = new Wall();
  {
    vec2D pos, size;
    pos.x = 400;
    pos.y = 0;
    size.x = 1920;
    size.y = 128;
    cellar->init(pos, size, *World::getWorld()->groundTexture());
  }
  Wall* w1 = new Wall();
  {
    vec2D pos, size;
    pos.x = 50;
    pos.y = 300;
    size.x = 480;
    size.y = 32;
    w1->init(pos, size, *World::getWorld()->wallTexture());
  }

  Wall* w2 = new Wall();
  {
    vec2D pos, size;
    pos.x = 750;
    pos.y = 300;
    size.x = 480;
    size.y = 32;
    w2->init(pos, size, *World::getWorld()->wallTexture());
  }

  Wall* w3 = new Wall();
  {
    vec2D pos, size;
    pos.x = 0;
    pos.y = WINDOW_HEIGHT / 2;
    size.x = 128;
    size.y = 1920;
    w3->init(pos, size, *World::getWorld()->sideWallTexture());
  }

  Wall* w4 = new Wall();
  {
    vec2D pos, size;
    pos.x = WINDOW_WIDTH + 170;
    pos.y = WINDOW_HEIGHT / 2;
    size.x = 128;
    size.y = 1920;
    w4->init(pos, size, *World::getWorld()->sideWallTexture());
  }

  //deltatime
  sf::Clock clock;
  sf::Clock clock_imgui;

  bool game_runing = true;

  double t = 0.0f;
  double dt = 1.0f / 30.0f; //FIXED TIME STEP
  double current_time = (double)clock.getElapsedTime().asSeconds();
  while (game_runing && World::getWorld()->getWindow()->isOpen()) {

    ImGui::SFML::Update(*World::getWorld()->getWindow(), clock_imgui.restart());

    double new_time = (double)clock.getElapsedTime().asSeconds();
    double frame_time = (new_time - current_time);
    current_time = new_time;
    float time_elapsed = 0.0f;
    World::getWorld()->worldPollEvents();
    if (World::getWorld()->logged_in) {
      World::getWorld()->wordReadInput();
      float delta_time;
      while (frame_time > 0) {
        delta_time = (float)fmin(frame_time, dt);
        World::getWorld()->worldUpdate(delta_time);
        frame_time -= delta_time;
        t += delta_time;
        World::getWorld()->last_delta = delta_time;
        World::getWorld()->worldSync();
        time_elapsed += delta_time;
      }
      World::getWorld()->getPhysicsWorld()->Step(time_elapsed, 5, 5);
      time_elapsed = 0.0f;
    }
    else {

    }

    World::getWorld()->worldDraw();

  }
  return 0;
}