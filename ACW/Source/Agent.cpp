#include "Agent.h"
#include "World.h"
#include "Idle.h"
#include "Navigate.h"

Agent::Agent()
{

}

Agent::~Agent()
{
    delete m_CurrentState;
}

void Agent::Update(float pSeconds)
{
    if (m_CurrentState == nullptr) return;
    m_CurrentState->OnStateUpdate(pSeconds);
    UpdateGamePhysics(pSeconds);
}

void Agent::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Agent::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

//Reduce AgentHealth
void Agent::TakeDamage(const float damage)
{
	m_Health = m_Health - damage;
	if (m_Health <= 0)
	{
        //Reset, Respaw Agent
        Navigate* navState = dynamic_cast<Navigate*>(m_CurrentState);
        if (navState != nullptr) navState->EndNavigation();

        Init(m_TeamType, m_AgentType);
	} 
}

void Agent::Init(const TeamType teamType, const AgentType agentType)
{
    m_CapturedTower = nullptr;
    m_AgentType = agentType;
    m_TeamType = teamType;
    GetColour() = (m_TeamType == TeamType::BLUE) ? vec3(0,0,1) : vec3(1, 0, 0);
    m_Health = m_MaxHealth;
    m_Radius = 0.5f;
    m_Height = 2.f;
    m_Speed = 1000;
    m_Axis = normalize(vec3(0, m_Height, 0));
    vec3 pos = World::instance->GetRandomSpawnPos();
    SetPosition(pos);
    SetState(new Idle(pos));
}

void Agent::SetState(State* newState)
{
    if (m_CurrentState)
        m_CurrentState->OnStateExit();

    m_CurrentState = newState;
    m_CurrentState->owningAgent = this;

    if (m_CurrentState)
        m_CurrentState->OnStateEnter();
}

void Agent::SetCapturedTower(Tower* tower)
{
    m_CapturedTower = tower;
}
