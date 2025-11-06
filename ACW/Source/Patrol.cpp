#include "Patrol.h"
#include <iostream>
#include "Agent.h"
#include "Attack.h"
#include "World.h"

void Patrol::OnStateEnter()
{
	
}

void Patrol::OnStateExit()
{
	
}

void Patrol::OnStateUpdate(const float pSeconds)   
{
	if (m_Target == nullptr)
	{
		m_Target = World::instance->FindNearestEnemyCapturer(owningAgent->GetPosition(), owningAgent->GetTeamType());
	}
	else
	{
		//Seek
		if (!World::instance->IsFleeMode())
		{
			vec3 desiredvelocity = normalize(m_Target->GetPosition() - owningAgent->GetPosition()) * 12.f;
			owningAgent->SetVelocity(desiredvelocity - owningAgent->GetVelocity());

			if (length(m_Target->GetPosition() - owningAgent->GetPosition()) < 0.5f)
			{
				owningAgent->SetState(new Attack(dynamic_cast<Agent*>(m_Target)));
			}
		}
		else
		{
			//Flee
			vec3 desiredvelocity = normalize(owningAgent->GetPosition() - m_Target->GetPosition()) * 12.f;
			owningAgent->SetVelocity(desiredvelocity - owningAgent->GetVelocity());
		}
	}
}