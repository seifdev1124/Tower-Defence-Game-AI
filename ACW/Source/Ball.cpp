#include "Ball.h"
#include "World.h"

Ball::Ball()
{

}

void Ball::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);
}

void Ball::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);
}

void Ball::Update(float pDeltaTime)
{
	if (GetPosition().y <= 1.f)
	{
		float y = 1.f;
		SetPosition(vec3(GetPosition().x, y, GetPosition().z));
	}
	Translate(m_Velocity * pDeltaTime);
	UpdateProjectile(pDeltaTime);
	CheckCollisionWithGround();
	CheckCollisionWithAgent();
}

//Initial push to the projectile
void Ball::InitializeProjectile(vec3 towerPosition, Agent* agent)
{
	Translate(towerPosition);
	m_Radius = 0.25f;
	m_TargetAgent = agent;
	vec3 targetDirection = normalize(agent->GetPosition() - towerPosition);
	m_Velocity = vec3(targetDirection.x, targetDirection.y, targetDirection.z) * 25.f;
}


//For Numerical Integrations
void Ball::UpdateProjectile(float deltaTime)
{
	if (World::instance->isEuler)
	{

		vec3 acceleration = m_ForceApplied / m_Mass;
		m_Velocity = m_Velocity + (acceleration * (deltaTime) * 15.f);
		m_Velocity = glm::clamp(m_Velocity, -50.f, 50.f);
		SetPosition(GetPosition() + (m_Velocity * deltaTime));
	}
	else if (World::instance->isImprovedEuler)
	{

		vec3 acceleration = m_ForceApplied / m_Mass;
		vec3 previousVelocity = m_Velocity;
		m_Velocity = m_Velocity + (acceleration - (0.1f / m_Mass) * m_Velocity) * deltaTime * 15.f;
		m_Velocity = glm::clamp(m_Velocity, -50.f, 50.f);
		SetPosition(GetPosition() + ((previousVelocity + m_Velocity) / 2.f) * deltaTime);
	}
	else if (World::instance->isRangeKutta)
	{
		vec3 k1, k2, k3, k4, k5;
		vec3 acceleration = m_ForceApplied / m_Mass;

		k1 = deltaTime * acceleration;
		k2 = deltaTime * (acceleration + k1/2.f);
		k3 = deltaTime * (acceleration + k1/2.f);
		k4 = deltaTime * (acceleration + k3);
		
		m_Velocity += (k1 + 2.0f * k2 + 2.0f * k3 + k4) / 6.0f;
		SetPosition(GetPosition() + (deltaTime * m_Velocity));
	}
}

//Collision with ground and response
void Ball::CheckCollisionWithGround()
{
	if (m_BounceCount >= m_MaxBounceCount)
	{
		m_ForceApplied = vec3(0);
		SetVelocity(m_ForceApplied);
	}
	else
	{
		float distanceFromGround = GetPosition().y - m_Ground.GetPosition().y ;

		if (distanceFromGround < 0.9f)
		{
			m_BounceCount++;
			float dotProduct = dot(m_Normal, m_Velocity);
			m_Velocity = m_Velocity - (1 + m_CoefficientOfRestitution) * (dotProduct)*m_Normal;
			SetVelocity(m_Velocity);
		}
	}
}

void Ball::CheckCollisionWithAgent()
{
	if (World::instance->IsGodMod()) return;

	if (!m_HasHitAgent)
	{
		vec3 distVector = GetPosition() - m_TargetAgent->GetPosition();

		float dotProduct = dot(distVector, m_TargetAgent->GetAxis());


		vec3 closestPointOnCylinder = m_TargetAgent->GetPosition() + m_TargetAgent->GetAxis() * dotProduct;

		if (dotProduct < 0 || dotProduct > m_TargetAgent->GetHeight())
		{
			
		}

		float distSq = dot(closestPointOnCylinder - GetPosition(), closestPointOnCylinder - GetPosition());


		if (distSq <= (m_Radius + m_TargetAgent->GetRadius()) * (m_Radius + m_TargetAgent->GetRadius()))
		{
			auto u = GetVelocity();
			auto v = u - ((2) * (dot(m_Normal, m_Velocity)) * m_Normal);
			SetVelocity(-v * 0.1f);  // negate the velocity vector before setting it as the new velocity
			m_TargetAgent->TakeDamage(m_Damage);
			m_HasHitAgent = true;
			World::instance->RemoveProjectiles(this);
		}
	}
}
