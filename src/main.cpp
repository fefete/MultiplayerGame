//
//
//

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "PlayerCharacter.h"
#include "world.h"
#include "assets.h"

void createGround(b2World& world, float x, float y){
    
    
    b2BodyDef body_def;
    body_def.position = b2Vec2(x/SCALE, y/SCALE);
    body_def.type = b2_staticBody;
    b2Body* body = world.CreateBody(&body_def);
    b2PolygonShape shape_;
    shape_.SetAsBox((800.f/2)/SCALE, (16.f/2)/SCALE);
    b2FixtureDef fix_def;
    fix_def.density = 0.f;
    fix_def.shape = &shape_;
    body->CreateFixture(&fix_def);
}

void CreateBox(b2World& World, int MouseX, int MouseY)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(MouseX/SCALE, MouseY/SCALE);
    BodyDef.type = b2_dynamicBody;
    b2Body* Body = World.CreateBody(&BodyDef);
    b2PolygonShape Shape;
    Shape.SetAsBox((32.f/2)/SCALE, (32.f/2)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    printf("creating box \n");
}



int main()
{
    

    b2Vec2 Gravity(0.f, 9.8f);
    World::getWorld()->worldInit(Gravity);
    
    sf::Texture ground_t;
    sf::Texture box_t;
    if(!ground_t.loadFromFile(ASSETS::ImagePath("ground.png"))){
        return EXIT_FAILURE;
    }
    if(!box_t.loadFromFile(ASSETS::ImagePath("box.png"))){
        return EXIT_FAILURE;
    }
    
    sf::Sprite sprite(box_t);

    PlayerCharacter* p1 = new PlayerCharacter();
    PlayerCharacter* p2 = new PlayerCharacter();

    createGround(*World::getWorld()->getPhysicsWorld(), 400, 500);
    vec2D pos;
    pos.x = 400;
    pos.y = 300;
    vec2D size;
    size.x = 16;
    size.y = 16;
    p1->init(pos, size, box_t);

    pos.x = 350;
    
    p2->init(pos, size, box_t);

    //deltatime
    sf::Clock clock;

    bool game_runing = true;

    double t = 0.0f;
    double dt = 1.0f/30.0f; //FIXED TIME STEP
    double current_time = (double)clock.getElapsedTime().asSeconds();

    while (game_runing && World::getWorld()->getWindow()->isOpen()) {

      double new_time = (double)clock.getElapsedTime().asSeconds();
      double frame_time = (new_time - current_time);
      current_time = new_time;

      World::getWorld()->worldPollEvents();
      World::getWorld()->wordReadInput();

      while (frame_time > 0) {

        float delta_time = (float)fmin(frame_time, dt);
        World::getWorld()->worldUpdate(delta_time);
        frame_time -= delta_time;
        t += delta_time;

      }
     World::getWorld()->getPhysicsWorld()->Step(1.0f / 30.0f, 5, 5);
     World::getWorld()->worldDraw();

    }
    return 0;
}
