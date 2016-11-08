#include "PlayerCharacter.h"
#include "world.h"

PlayerCharacter::PlayerCharacter()
{
}

PlayerCharacter::~PlayerCharacter()
{
}

void PlayerCharacter::init(vec2D position, vec2D size, sf::Texture& texture) {
  body_def_.position = b2Vec2((position.x) / SCALE, (position.y) / SCALE);
  body_def_.type = b2_dynamicBody;
  body_ = World::getWorld()->getPhysicsWorld()->CreateBody(&body_def_);

  shape_.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
  fixture_def.density = 1.f;
  fixture_def.friction = 0.7f;
  fixture_def.shape = &shape_;
  body_->CreateFixture(&fixture_def);

  sprite_.setTexture(texture);
  sprite_.setOrigin(16, 16);
  body_->SetUserData(this);

}

void PlayerCharacter::update(float dt) {

  for (int i = 0; i < movement_state_.size(); i++)
  {
    switch (movement_state_[i])
    {
    case kPkayerInputState_right:
      body_->ApplyForce(b2Vec2(5, 0), body_->GetWorldCenter(), true);
      break;
    case kPkayerInputState_left:
      body_->ApplyForce(b2Vec2(-5, 0), body_->GetWorldCenter(), true);
      break;
    case kPkayerInputState_jump:
      body_->ApplyLinearImpulse(b2Vec2(0, -15), body_->GetWorldCenter(), true);
      break;
    default:
      break;
    }
  }
  movement_state_.clear();
}

void PlayerCharacter::draw(b2Body* b) {
  sprite_.setPosition(SCALE * b->GetPosition().x, SCALE * b->GetPosition().y);
  sprite_.setRotation(b->GetAngle() * 180 / b2_pi);

  //printf("DR: %f , %f \n", sprite_.getPosition().x, sprite_.getPosition().y);
}

bool jump = false;

void PlayerCharacter::readInput() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && jump == false) {
    movement_state_.push_back(kPkayerInputState_jump);
    jump = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    movement_state_.push_back(kPkayerInputState_left);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    movement_state_.push_back(kPkayerInputState_right);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
  }
}



void PlayerCharacter::action() {


}