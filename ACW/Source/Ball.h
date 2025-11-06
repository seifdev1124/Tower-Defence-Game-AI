#pragma once
#include "GameEntity.h"
#include "Floor.h"
#include "Agent.h"
#include <iostream>

using namespace glm;

class Ball : public GameEntity
{
private:
    float m_CoefficientOfRestitution = 0.85f;
    int m_MaxBounceCount = 4;
    int m_BounceCount = 0;
    float m_Damage = 30.f;
    bool m_HasHitAgent = false;
    vec3 m_ForceApplied = vec3(0, -9.81f, 0);  //Gravitational Pull
    vec3 m_Normal = vec3(0, 1, 0);
    Floor m_Ground;
    Agent *m_TargetAgent;

   

public:
    Ball();

    vec3 GetPosition() const
    {
        mat4 model = GetModel();
        vec3 translation(model[3][0], model[3][1], model[3][2]);
        return translation;
    }

private:
    void UpdateProjectile(const float deltaSeconds);
    void CheckCollisionWithGround();
    void CheckCollisionWithAgent();

public:
    void Render(const IRenderHelpers&) const;
    void RenderGui(const IGuiHelpers&);
    void Update(float pDeltaTime);
    void InitializeProjectile(const vec3 towerPosition, Agent* agent);
};

