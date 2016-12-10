#include "entity.h"

Entity::Entity()
{
  m_is_active = true;
  m_can_draw = true;
}

Entity::~Entity()
{
}
void Entity::update(float dt)
{
  if (body_->IsActive() && m_is_active == false) {
    body_->SetActive(false);
  }
  else if (!body_->IsActive() && m_is_active == true) {
    body_->SetActive(true);
  }
}

void Entity::physicObjectState(bool state)
{
  m_is_active = state;
}

void Entity::drawableObjectState(bool state)
{
  m_can_draw = state;
}
