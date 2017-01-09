#include "PlayerCharacter.h"
#include "world.h"

#define DASH_CD_TIME 5.0f
#define TIME_DASHING 0.5f

PlayerCharacter::PlayerCharacter()
{
}

PlayerCharacter::~PlayerCharacter()
{
}

void PlayerCharacter::init(vec2D position, vec2D size, sf::Texture& texture) {

  //BODY
  type_ = kEntityType_player;

  body_def_.position = b2Vec2((position.x) / SCALE, (position.y) / SCALE);
  body_def_.type = b2_dynamicBody;
  body_ = World::getWorld()->getPhysicsWorld()->CreateBody(&body_def_);
  body_->SetFixedRotation(true);
  body_->SetGravityScale(0);
  World::getWorld()->addCharacterToWorld(this);
  m_size = size.x;

  shape_.SetAsBox((size.x / 2) / SCALE, (size.y / 2) / SCALE);
  fixture_def.density = 1.f;
  fixture_def.friction = 0.7f;
  fixture_def.shape = &shape_;
  body_->SetUserData(this);
  body_->CreateFixture(&fixture_def);

  sprite_.setTexture(texture);
  sprite_.setOrigin(size.x / 2, size.y / 2);

  can_jump = true;
  has_ball = false;
  dash_cd = 0.0f;
  score = 0.0f;
}

void PlayerCharacter::update(float dt) {
  int speedx = 0;
  int speedy = 0;
  for (unsigned int i = 0; i < movement_state_.size(); i++)
  {
    switch (movement_state_[i])
    {
    case kPlayerInputState_right:
      speedx = 10;
      break;
    case kPlayerInputState_left:
      speedx = -10;
      break;
    case kPlayerInputState_up:
      speedy = -10;
      break;
    case kPlayerInputState_down:
      speedy = 10;
      break;
    case kPlayerInputState_dash:
      time_dashing = TIME_DASHING;
      dash_cd = DASH_CD_TIME;
      break;
    default:
      break;
    }
  }

  if (has_ball) {
    score += 1;
  }

  if (this == World::getWorld()->localPlayer) {
    movement_state_.clear();
  }
  if (dash_cd > 0 && !has_ball) {
    dash_cd -= dt;
  }
  if (time_dashing > 0) {
    time_dashing -= dt;
    speedx *= 4;
    speedy *= 4;
  }
  body_->SetLinearVelocity(b2Vec2(speedx, speedy));
  body_->SetTransform(body_->GetPosition(), 0);
}

void PlayerCharacter::draw() {
  sprite_.setPosition(SCALE * body_->GetPosition().x, SCALE * body_->GetPosition().y);
  sprite_.setRotation(body_->GetAngle() * 180 / b2_pi);
}



void PlayerCharacter::readInput() {

  if (this != World::getWorld()->localPlayer)return;
  if (!World::getWorld()->getWindow()->hasFocus()) return;
  //set flag variables to false again;

  input_jump = false;
  input_left = false;
  input_right = false;
  input_dash = false;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    movement_state_.push_back(kPlayerInputState_up);
    input_jump = true;
    can_jump = false;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    movement_state_.push_back(kPlayerInputState_down);
    input_jump = true;
    can_jump = false;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    movement_state_.push_back(kPlayerInputState_left);
    input_left = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    movement_state_.push_back(kPlayerInputState_right);
    input_right = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
    if (!has_ball && dash_cd <= 0) {
      movement_state_.push_back(kPlayerInputState_dash);
      input_dash = true;
      return;
    }
  }
}



void PlayerCharacter::throwBall() {
}

void PlayerCharacter::dash() {
  dash_cd = DASH_CD_TIME;
  time_dashing = TIME_DASHING;
}

void PlayerCharacter::beginContact(Entity * contacted)
{
  PlayerCharacter* contact;
  switch (contacted->type_) {
  case kEntityType_ball:
    contacted->drawableObjectState(false);
    contacted->physicObjectState(false);
    has_ball = true;
    break;
  case kEntityType_player:
    //check steal ball
    contact = dynamic_cast<PlayerCharacter*>(contacted);
    if (contact->has_ball && dash_cd > 0) {
      contact->has_ball = false;
      has_ball = true;
    }else if (has_ball && contact->dash_cd > 0) {
      has_ball = false;
      contact->has_ball = true;
    }
    break;
  default:
    break;
  }
}

void PlayerCharacter::exitContact(Entity * contacted)
{
}
