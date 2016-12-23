#include "PlayerCharacter.h"
#include "world.h"

#define DASH_CD_TIME 5.0f
#define TIME_DASHING 2.5f

PlayerCharacter::PlayerCharacter()
{
}

PlayerCharacter::~PlayerCharacter()
{
}

void PlayerCharacter::init(vec2D position, vec2D size, sf::Texture& texture) {

  type_ = kEntityType_player;

  body_def_.position = b2Vec2((position.x) / SCALE, (position.y) / SCALE);
  body_def_.type = b2_kinematicBody;
  body_ = World::getWorld()->getPhysicsWorld()->CreateBody(&body_def_);
  World::getWorld()->addCharacterToWorld(this);

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
}

void PlayerCharacter::update(float dt) {
  int speedx = 0;
  int speedy = 0;
  for (unsigned int i = 0; i < movement_state_.size(); i++)
  {
    switch (movement_state_[i])
    {
    case kPlayerInputState_right:
      //body_->ApplyForce(b2Vec2(5, 0), body_->GetWorldCenter(), true);
      speedx = 10;
      break;
    case kPlayerInputState_left:
      //body_->ApplyForce(b2Vec2(-5, 0), body_->GetWorldCenter(), true);
      speedx = -10;
      break;
    case kPlayerInputState_up:
      //body_->ApplyLinearImpulse(b2Vec2(0, -15), body_->GetWorldCenter(), true);
      speedy = -10;
      break;
    case kPlayerInputState_down:
      //dash();
      speedy = 10;
      break;
    case kPlayerInputState_dash:
      speedx *= 4;
      speedy *= 4;
      break;
    default:
      break;
    }
  }
  body_->SetLinearVelocity(b2Vec2(speedx, speedy));

  if (this == World::getWorld()->localPlayer) {
    movement_state_.clear();
    if (has_ball) {
      printf("has ball");
    }
  }
  if (dash_cd > 0) {
    dash_cd -= dt;
  }
  if (time_dashing > 0) {
    time_dashing -= dt;
  }
}

void PlayerCharacter::draw() {
  sprite_.setPosition(SCALE * body_->GetPosition().x, SCALE * body_->GetPosition().y);
  sprite_.setRotation(body_->GetAngle() * 180 / b2_pi);

  //printf("DR: %f , %f \n", SCALE * body_->GetPosition().x, body_->GetPosition().y);
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
    if (has_ball) {
      //movement_state_.push_back(kPlayerInputState_throw);
      return;
    }
  }
}



void PlayerCharacter::throwBall() {
  World::getWorld()->getBall()->drawableObjectState(true);
  b2Vec2 ball_new_pos;
  b2Vec2 velocity = body_->GetLinearVelocity();
  velocity.Normalize();
  ball_new_pos.x = velocity.x * 5 + body_->GetPosition().x;
  ball_new_pos.y = velocity.y * 5 + body_->GetPosition().y;
  World::getWorld()->getBall()->body_->SetTransform(ball_new_pos, body_->GetAngle());
  World::getWorld()->getBall()->body_->ApplyForce(b2Vec2(ball_new_pos.x * 10, ball_new_pos.y * 10),
    World::getWorld()->getBall()->body_->GetWorldCenter(),
    true);
  World::getWorld()->getBall()->physicObjectState(true);

  has_ball = false;
  dash_cd = DASH_CD_TIME;

}

void PlayerCharacter::dash() {
  body_->ApplyForce(b2Vec2(body_->GetLinearVelocity().x * 150, body_->GetLinearVelocity().y * 150),
    body_->GetWorldCenter(), true);
  dash_cd = DASH_CD_TIME;
  time_dashing = TIME_DASHING;
}

void PlayerCharacter::beginContact(Entity * contacted)
{
  switch (contacted->type_) {
  case kEntityType_ball:
    //joint
    contacted->drawableObjectState(false);
    contacted->physicObjectState(false);
    has_ball = true;
    break;
  case kEntityType_player:
    //check steal ball
    break;
  case kEntityType_wall:
    //recover jump
    can_jump = true;
    break;
  default:
    break;
  }
}

void PlayerCharacter::exitContact(Entity * contacted)
{
  switch (contacted->type_) {
  case kEntityType_ball:
    //joint
    break;
  case kEntityType_player:
    //check steal ball
    break;
  case kEntityType_wall:
    //loose jump?
    can_jump = false;
    break;
  default:
    break;
  }
}
