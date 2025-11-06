#pragma once
#include "GameEntity.h"
#include "Ball.h"
#include "Pipe.h"
#include "Agent.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;
using namespace glm;

class Tower : public Pipe
{
private:
    float m_FireRadius = 10.f;
    float m_Timer = 0;
    float m_ReloadTime = 0.5f;
    bool m_IsCaptured;
    Agent* m_CapturingAgent;
    float m_InfulenceRangeValue;

public:
    float GetCurrentInfluenceRangeValue() { return m_InfulenceRangeValue; }
    Agent* GetCapturingAgent() { return m_CapturingAgent; }

private:
    void ScanForAgent();

public:
    Tower();
    ~Tower();
    void Update(const float pSeconds);
    void Render(const IRenderHelpers&) const;
    void RenderGui(const IGuiHelpers&);
    void InitializeTower(const glm::vec3 position);
    void ShootProjectile(Agent* target);
    void SetCurrentCaptureData(Agent* capturingAgent);
    void CaptureComplete();
    void FreeTower();
};