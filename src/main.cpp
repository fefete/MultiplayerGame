//
//
//

#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "PlayerCharacter.h"
#include "world.h"

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
    if(!ground_t.loadFromFile("")){
        return EXIT_FAILURE;
    }
    if(!box_t.loadFromFile("")){
        return EXIT_FAILURE;
    }
    
    PlayerCharacter* p1 = new PlayerCharacter();
    PlayerCharacter* p2 = new PlayerCharacter();


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

    float t = 0.0f;
    float dt = 1 / 30; //FIXED TIME STEP
    float current_time = clock.getElapsedTime().asSeconds();

    while (game_runing) {
    
      float new_time = clock.getElapsedTime().asSeconds();
      float frame_time = new_time - current_time;
      current_time = new_time;

      World::getWorld()->wordReadInput();

      while (frame_time > 0){
      
        float delta_time =(float)fmin(frame_time, dt);
        World::getWorld()->worldUpdate(delta_time);
        frame_time -= delta_time;
        t += delta_time;

      }
    
      World::getWorld()->worldDraw();

    }// :D

    return 0;
}


/*PlayerCharacter pl;

vec2D v;
v.x = 400;
v.y = 300;
vec2D v2;
v2.x = 16;
v2.y = 16;

pl.init(v, v2 , box_t, world);
createGround(world, 400, 500);

while (window.isOpen())
{
sf::Event event;

//input

if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
{

int coord_x = sf::Mouse::getPosition().x;
int coord_y = sf::Mouse::getPosition().y;

CreateBox(world, coord_x, coord_y);

}

//update
while (window.pollEvent(event))
{
if (event.type == sf::Event::Closed)
window.close();
}
world.Step(1/60.0f, 8, 3);
window.clear(sf::Color::White);
// :)

for (b2Body* bodyIterator = world.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext()){
if (bodyIterator->GetType() == b2_dynamicBody)
{

pl.readInput();
pl.update();
pl.draw(bodyIterator);
printf("PY: %f , %f \n", bodyIterator->GetPosition().x, bodyIterator->GetPosition().y);
window.draw(pl.sprite_);


}
else
{
sf::Sprite GroundSprite;
GroundSprite.setTexture(ground_t);
GroundSprite.setOrigin(400.f, 8.f);
GroundSprite.setPosition(bodyIterator->GetPosition().x * SCALE, bodyIterator->GetPosition().y * SCALE);
GroundSprite.setRotation(180/b2_pi * bodyIterator->GetAngle());
window.draw(GroundSprite);
}

}
//draw

//window.draw();
window.display();
}*/