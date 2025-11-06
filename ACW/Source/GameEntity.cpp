#include "GameEntity.h"
#include "helpers/IGuiHelpers.h"

unsigned int GameEntity::s_NextID = 0;

void GameEntity::RenderGui(const IGuiHelpers& pGuiHelper)
{
	pGuiHelper.RenderGui(this);
}

void GameEntity::UpdateGamePhysics(float deltaTime)
{
    m_ForceVector += (/*m_GravityValue **/ 500.f * m_GravityVector);
    glm::vec3 currentAcceleration = m_AccelerationVector + m_ForceVector / 500.f;
    m_Velocity += currentAcceleration * deltaTime;
    m_Velocity *= pow(0.3f, deltaTime);

    m_Position += m_Velocity * deltaTime;

    SetPosition(m_Position);

    m_ForceVector = glm::vec3(0.0f);
    m_AccelerationVector = glm::vec3(0.0f);
    if (m_Position.y < 0)
    {
        m_Position.y = 0;
        m_Velocity.y = 0;
    }
}
