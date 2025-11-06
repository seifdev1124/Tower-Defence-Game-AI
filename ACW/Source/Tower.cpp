#include "Tower.h"
#include "World.h"

Tower::Tower()
{
	
}

Tower::~Tower()
{

}

void Tower::Update(float pSeconds)
{
	m_Timer += pSeconds;

	//Scans only in every x seconds to save usage time
	if (m_Timer > m_ReloadTime)
	{
		ScanForAgent();
		m_Timer = 0;
	}
}

void Tower::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Tower::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

//Initialize Tower
void Tower::InitializeTower(const glm::vec3 position)
{
	m_TeamType = TeamType::NEUTRAL;
	m_BaseHeight = 3.0f;
	Translate(position);
}

//Shoots Projectile
void Tower::ShootProjectile(Agent* target)
{
	Ball* newBall = new Ball();
	newBall->InitializeProjectile(glm::vec3(GetPosition().x, GetPosition().y + 4, GetPosition().z), target);
	World::instance->AddProjectiles(newBall);
}

//Scans for Agents in Radius
void Tower::ScanForAgent()
{
	Agent* agent = World::instance->FindNearestAgent(m_Position, m_FireRadius, m_TeamType);
	if (agent != nullptr /*&& agent->GetTeamType()!= m_TeamType*/)
	{
		ShootProjectile(agent);
	}
}

void Tower::SetCurrentCaptureData(Agent* capturingAgent)
{
	m_CapturingAgent = capturingAgent;
}

void Tower::CaptureComplete()
{
	m_IsCaptured = true;
	m_TeamType = m_CapturingAgent->GetTeamType();
	GetColour() = m_CapturingAgent->GetColour();
}

void Tower::FreeTower()
{
	m_CapturingAgent = nullptr;
	GetColour() = vec3(0, 1, 0);
	m_TeamType = TeamType::NEUTRAL;

}
