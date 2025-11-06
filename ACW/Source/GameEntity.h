#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace std;
using namespace glm;

class IRenderHelpers;
class IGuiHelpers;

enum class TeamType
{
	RED,
	BLUE,
	NEUTRAL
};

// Define an enum for days of the week
enum class AgentType {
	Seeker,
	Capturer
};


class GameEntity
{
private:


protected:
	float m_Mass = 1;
	float m_Radius = 0;
	unsigned int m_ID;
	static unsigned int s_NextID;
	mat4 m_Model;
	vec3 m_Colour;
	vec3 m_ForceVector = vec3(0.0f, 0.0f, 0.0f);
	const vec3 m_GravityVector = vec3(0.0f, -9.8f, 0.0f);
	const float m_GravityValue = 9.8f;
	vec3 m_AccelerationVector = vec3(0.0f, 0.0f, 0.0f);
	vec3 m_Velocity = vec3(0.0f, 0.0f, 0.0f);
	vec3 m_Position = vec3(0.0f, 0.0f, 0.0f);
	float m_Speed;
	TeamType m_TeamType;

public:
	GameEntity() : m_Colour(1.0f, 1.0f, 1.0f), m_ID(s_NextID)
	{
		s_NextID++;
	}

	unsigned int ID() const { return m_ID; }
	mat4 GetModel() const { return m_Model; }
	mat4& GetModel() { return m_Model; }
	vec3 GetColour() const { return m_Colour; }
	vec3 & GetColour() { return m_Colour; }
	float GetRadius() const{ return m_Radius; }
	vec3 GetPosition() const { return m_Position; }
	vec3 & GetForceVector() { return m_ForceVector; }
	vec3 GetVelocity() const { return m_Velocity; }
	float GetMass() const {return m_Mass;}
	float GetSpeed() const { return m_Speed; }
	TeamType GetTeamType() { return m_TeamType; }
	void Scale(glm::vec3 pScale)
	{
		m_Model = scale(m_Model, pScale);
	}

	void Translate(vec3 pTranslation)
	{
		m_Model = translate(m_Model, pTranslation);
		m_Position = pTranslation;
	}

	void Rotate(float pRadians, vec3 pAxis)
	{
		m_Model = rotate(m_Model, pRadians, pAxis);
	}

	void SetPosition(glm::vec3 pPosition)
	{
		glm::mat4& model = GetModel();
		model[3][0] = pPosition.x;
		model[3][1] = pPosition.y;
		model[3][2] = pPosition.z;
		m_Position = pPosition;
	}

	void SetVelocity(vec3 velocity)
	{
		m_Velocity = velocity;
	}

	void SetMass(float mass)
	{
		m_Mass = mass; 
	}

	void ShutdowmMovement()
	{
		m_Velocity = vec3(0);
		m_AccelerationVector = vec3(0);
		m_ForceVector = vec3(0);
		//m_Speed = 0;
	}
	
	float RandomFloat(float min, float max)
	{
		// this  function assumes max > min, you may want 
		// more robust error checking for a non-debug build
		assert(max > min);
		float random = ((float)rand()) / (float)RAND_MAX;

		// generate (in your case) a float between 0 and (4.5-.78)
		// then add .78, giving you a float between .78 and 4.5
		float range = max - min;
		return (random * range) + min;
	}

	virtual void Render(const IRenderHelpers&) const = 0;
	virtual void RenderGui(const IGuiHelpers&);
	void UpdateGamePhysics(float deltaTime);
};
