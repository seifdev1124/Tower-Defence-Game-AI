#include "CapturingTower.h"
#include "Tower.h"
#include "Navigate.h"
#include "World.h"

void CapturingTower::OnStateEnter()
{
	m_InfluenceValue = 0;
	m_Tower->SetCurrentCaptureData(owningAgent);
	owningAgent->SetCapturedTower(m_Tower);
}

void CapturingTower::OnStateExit()
{
	
}

void CapturingTower::OnStateUpdate(const float pSeconds)
{
	if (m_InfluenceValue >= 10) return;

	m_InfluenceTimer += pSeconds;
	if (m_InfluenceTimer >= 1.f)
	{
		m_InfluenceValue += 1;
		m_InfluenceTimer = 0;
	}
	if (m_InfluenceValue >= 2.f)
	{
		m_Tower->CaptureComplete();
		owningAgent->SetState(new Navigate(World::instance->FindNearestTower(owningAgent->GetPosition()), TargetType::Tower));
	}
}
