#pragma once
#include "GameEntity.h"
#include "Floor.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace glm;

class State;
class Tower;

class Agent :public GameEntity
{
private:
	float m_Health = 0;
	float m_MaxHealth = 1000;
	float m_Height;
	State* m_CurrentState;
	vec3 m_Axis;
	AgentType m_AgentType;
	Tower* m_CapturedTower;

public:
	Agent();
	~Agent();
	void Update(const float pSeconds);
	void Render(const IRenderHelpers&) const;
	void RenderGui(const IGuiHelpers&);
	void TakeDamage(const float damage);
	void Init(const TeamType teamType, const AgentType agentType);
	void SetState(State* state);
	float GetHeight() { return m_Height; }
	float GetRadius() { return m_Radius; }
	float GetCurrentHealth() { return m_Health; }
	vec3 GetAxis() { return m_Axis; }
	AgentType GetType() { return m_AgentType; }

	void SetCapturedTower(Tower* capturingTower);
	Tower* GetCapturedTower() { return m_CapturedTower; }

private:
};

