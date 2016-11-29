#include "wall.h"
#include "world.h"

Wall::Wall()
{

}

Wall::~Wall()
{

}

void Wall::draw()
{
  sprite_.setPosition(SCALE * body_->GetPosition().x, SCALE * body_->GetPosition().y);
  sprite_.setRotation(body_->GetAngle() * 180 / b2_pi);
}

void Wall::init(vec2D position, vec2D size, sf::Texture& texture)
{
  body_def_.position = b2Vec2((position.x) / SCALE, (position.y) / SCALE);
  body_def_.type = b2_staticBody;
  body_ = World::getWorld()->getPhysicsWorld()->CreateBody(&body_def_);
  World::getWorld()->addCharacterToWorld(this);

  shape_.SetAsBox((size.x / 2) / SCALE, (size.y / 2) / SCALE);
  fixture_def.density = 1.f;
  fixture_def.shape = &shape_;
  body_->CreateFixture(&fixture_def);

  sprite_.setTexture(texture);
  sprite_.setOrigin(size.x / 2, size.y / 2);
  body_->SetUserData(this);
}
