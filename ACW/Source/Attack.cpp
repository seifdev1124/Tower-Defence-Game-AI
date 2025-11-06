#include "Attack.h"
#include "Agent.h"
#include "Navigate.h"
#include "Patrol.h"
#include "World.h"

void Attack::OnStateEnter()
{
	if (m_AttackedAgent->GetType() == AgentType::Capturer && m_AttackedAgent->GetCapturedTower() != nullptr)
	{
		m_AttackedAgent->GetCapturedTower()->FreeTower();
		m_AttackedAgent->TakeDamage(10000);

		if (owningAgent->GetType() == AgentType::Capturer)
		{
			owningAgent->SetState(new Navigate(World::instance->FindNearestTower(owningAgent->GetPosition()), TargetType::Tower));
		}

		else
		{
			owningAgent->SetState(new Patrol());
		}
	}

}

void Attack::OnStateExit()
{

}

void Attack::OnStateUpdate(const float pSeconds)
{
	if (owningAgent->GetType() == AgentType::Seeker)
	{
		owningAgent->SetState(new Patrol());
	}
}
