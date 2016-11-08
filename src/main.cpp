//
//  main.cpp
//  t01
//
//  Created by Cordry, Julien on 30/09/2016.
//  Copyright © 2016 Cordry, Julien. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "PlayerCharacter.h"

#define SCALE 30.0f

struct object{
    
    sf::Sprite sp;
    sf::Transform tf;
    
};

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
    
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML test!");
    
    
    b2Vec2 Gravity(0.f, 9.8f);
    b2World world (Gravity);
    
    sf::Texture ground_t;
    sf::Texture box_t;
    if(!ground_t.loadFromFile("/Users/s6098981/Desktop/MAC PROJECT/base/assets/images/ground.png")){
        return EXIT_FAILURE;
    }
    if(!box_t.loadFromFile("/Users/s6098981/Desktop/MAC PROJECT/base/assets/images/box.png")){
        return EXIT_FAILURE;
    }
    

    
    PlayerCharacter pl;
    
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
    }
    
    return 0;
}