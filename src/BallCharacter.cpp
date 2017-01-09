#include "BallCharacter.h"
#include "world.h"
Ball::Ball()
{
}

Ball::~Ball()
{
}

void Ball::init(vec2D position, float radius, sf::Texture &texture)
{

  type_ = kEntityType_ball;

  body_def_.position = b2Vec2((position.x) / SCALE, (position.y) / SCALE);
  body_def_.type = b2_dynamicBody;
  body_ = World::getWorld()->getPhysicsWorld()->CreateBody(&body_def_);
  World::getWorld()->addCharacterToWorld(this);

  c_shape_.m_p.Set(0,0);
  c_shape_.m_radius = radius / SCALE;
  fixture_def.density = 1.f;
  fixture_def.friction = 0.7f;
  fixture_def.shape = &c_shape_;
  body_->SetUserData(this);
  body_->CreateFixture(&fixture_def);

  sprite_.setTexture(texture);
  sprite_.setOrigin(radius, radius);


}

void Ball::draw()
{
  sprite_.setPosition(SCALE * body_->GetPosition().x, SCALE * body_->GetPosition().y);
  sprite_.setRotation(body_->GetAngle() * 180 / b2_pi);
}
