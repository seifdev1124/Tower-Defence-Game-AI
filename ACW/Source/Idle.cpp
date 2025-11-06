#include "Idle.h"
#include <iostream>
#include "Agent.h"
#include "Navigate.h"
#include "Patrol.h"
#include "World.h"

void Idle::OnStateEnter() 
{
	owningAgent->Translate(m_StartPos);
	if(owningAgent->GetType() == AgentType::Capturer)
		owningAgent->SetState(new Navigate(World::instance->FindNearestTower(m_StartPos), TargetType::Tower));
	else
		owningAgent->SetState(new Patrol());
}

void Idle::OnStateExit() 
{
	
}

void Idle::OnStateUpdate(const float pSeconds)
{
	
}