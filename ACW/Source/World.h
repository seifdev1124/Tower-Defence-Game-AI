#pragma once
#include "GameEntity.h"
#include <algorithm>
#include <queue>
#include <vector>
#include "Agent.h"
#include "Tower.h"
#include "Ball.h"

using namespace std;
using namespace glm;


class World : public GameEntity
{
public:
	static World* instance;
	const static int GRID_WIDTH = 50;
	const static int GRID_DEPTH = 50;

	bool isImprovedEuler = false;
	bool isEuler = false;
	bool isRangeKutta = false;


	struct CollisonDatAgentWall {
		bool m_Collision;
		vec3 m_Normal;
	};

	const static int NUMBER_OF_AGENTS = 4;
	const static int NUMBER_OF_TOWERS = 3;

private:
	bool m_AgentGodMode = false;
	bool m_FleePatrolMode = false;
	int m_GridHeights[GRID_WIDTH][GRID_DEPTH];
	vec3 m_GridColours[GRID_WIDTH][GRID_DEPTH];
	Agent m_Agents[NUMBER_OF_AGENTS];
	Tower m_Towers[NUMBER_OF_TOWERS];
	vector<Ball*> m_Projectiles;
	vector<vec3> m_AgentSpawnPos;
	void InitObjects();
	void BallToBallCollision();
	void BallToWallCollision();
	void AgentToWallCollision(Agent* agent);


	vector<Floor*> openList;
	vector<Floor*> closedList;

public:

	World();
	~World();

	Floor _grid[GRID_WIDTH][GRID_DEPTH];

	int GetGridHeight(int pX, int pZ) const
	{
		return m_GridHeights[pX][pZ];
	}

	vec3 GetGridColour(int pX, int pZ) const
	{
		return m_GridColours[pX][pZ];
	}

	void Update(float pSeconds);
	void Render(const IRenderHelpers&) const;
	void RenderGui(const IGuiHelpers&);
	Agent* FindNearestAgent(const vec3 fromPos, const float radiusFactor, const TeamType towerTeamType);
	void AddProjectiles(Ball* ball);
	void RemoveProjectiles(Ball* ball);

	//AI

	vec3 GetRandomSpawnPos();
	Tower* FindNearestTower(const vec3 fromPos);
	Agent* FindNearestEnemyCapturer(const vec3 fromPos, const TeamType agentTeamType);
	vector<Floor*> PathFinding(const vec3& start, const vec3& end);
	void SetGodMode(bool val);
	void SetFleeMode(bool val);
	bool IsGodMod() { return m_AgentGodMode; }
	bool IsFleeMode() { return m_FleePatrolMode; }


	// Spatial segmentation for AI
	static const int GRID_CELL_SIZE = 1;
	typedef std::vector<Floor*> GridCell;
	std::vector<std::vector<GridCell>> _spatialGrid;
	void SpatailSegmentation();

	// Spatial segmentation for Ball
	using GridCell1 = std::vector<Ball*>;
	std::vector<std::vector<GridCell1>> _spatialGridForCollision;
	void SpatialSegmentationForCollision();

};

