#include "Navigate.h"
#include <iostream>
#include "Agent.h"
#include "World.h"
#include "CapturingTower.h"
#include "Patrol.h"
#include "Attack.h"

void Navigate::OnStateEnter()
{
	if (m_Target == nullptr) return;


	vec3 TilePos = m_Target->GetPosition();

	if (owningAgent->GetPosition().x > TilePos.x) { TilePos.x += 1; }
	else if (owningAgent->GetPosition().x < TilePos.x) { TilePos.x -= 1; }

	if (owningAgent->GetPosition().z > TilePos.z) { TilePos.z += 1; }
	else if (owningAgent->GetPosition().z < TilePos.z) { TilePos.z -= 1; }

	m_PathList = World::instance->PathFinding(owningAgent->GetPosition(), TilePos);
	m_ShouldCalculate = true;
}

void Navigate::OnStateExit()
{
	if (m_Target == nullptr && owningAgent->GetType() == AgentType::Seeker)
	{
		owningAgent->SetState(new Patrol());
		return;
	}
	m_PathList.clear();
	m_PathIndex = 0;
	m_ShouldCalculate = false;
}

void Navigate::OnStateUpdate(const float pSeconds)
{
	if (m_Target == nullptr) return;

	if (m_PathIndex != m_PathList.size() && m_ShouldCalculate)
	{
		vec3 newTargetTilePos = m_PathList[m_PathList.size() - 1 - m_PathIndex]->GetPosition();
		vec3 moveDirection = newTargetTilePos - owningAgent->GetPosition();
		float distSquared = dot(moveDirection, moveDirection);
		if (moveDirection != vec3(0, 0, 0))
			moveDirection = normalize(moveDirection);

		owningAgent->GetForceVector() += (moveDirection * owningAgent->GetSpeed());

		if (distSquared <= 0.50f)
		{
			m_PathIndex++;
		}
	}

	else
	{
		DestinationReached();
	}
}

void Navigate::DestinationReached()
{
	EndNavigation();

	switch (m_TargetType)
	{
	case TargetType::Tower:
	{
		Tower* tower = dynamic_cast<Tower*>(m_Target);
		Agent* capturingAgent = tower->GetCapturingAgent();
		if (capturingAgent == nullptr)
		{
			owningAgent->SetState(new CapturingTower(dynamic_cast<Tower*>(m_Target)));

		}
		else
		{
			if (capturingAgent->GetTeamType() != owningAgent->GetTeamType())
			{
				owningAgent->SetState(new Navigate(capturingAgent, TargetType::Agent));
			}
			else
			{
				cout << "Playing Dumb" << endl;
				//Play Dumb or look for other towers.
			}
		}
	}
	break;

	case TargetType::Agent:
		owningAgent->SetState(new Attack(dynamic_cast<Agent*>(m_Target)));
		break;

	case TargetType::None:
		break;

	default:
		break;
	}
}

void Navigate::EndNavigation()
{
	owningAgent->ShutdowmMovement();
	/*World::instance->HighlightPath(m_PathList, false);
	m_PathList.clear();
	m_PathIndex = 0;
	m_ShouldCalculate = false;*/
}
