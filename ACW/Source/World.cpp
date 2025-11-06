#include "World.h"
#include "helpers/IRenderHelpers.h"
#include "helpers/IGuiHelpers.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include <algorithm>
#include <random>
#include <chrono>

World* World::instance = nullptr;

World::World()
{
	if (instance == nullptr) {
		instance = this;
	}
	InitObjects();
}

World::~World()
{

}

void World::InitObjects()
{
	isEuler = true;

	// Store SpawnPos
	m_AgentSpawnPos.push_back(vec3(15, 0, 2));
	m_AgentSpawnPos.push_back(vec3(40, 0, 2));
	m_AgentSpawnPos.push_back(vec3(47, 0, 35));
	m_AgentSpawnPos.push_back(vec3(47, 0, 5));
	m_AgentSpawnPos.push_back(vec3(47, 0, 22));
	m_AgentSpawnPos.push_back(vec3(5, 0, 5));
	m_AgentSpawnPos.push_back(vec3(5, 0, 20));
	m_AgentSpawnPos.push_back(vec3(5, 0, 30));
	m_AgentSpawnPos.push_back(vec3(5, 0, 35));
	m_AgentSpawnPos.push_back(vec3(45, 0, 5));
	m_AgentSpawnPos.push_back(vec3(45, 0, 20));
	m_AgentSpawnPos.push_back(vec3(45, 0, 30));
	m_AgentSpawnPos.push_back(vec3(45, 0, 35));
	m_AgentSpawnPos.push_back(vec3(35, 0, 45));


	// Create Grids
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		m_GridHeights[i][0] = 2;
		m_GridHeights[i][GRID_DEPTH - 1] = 2;
	}

	for (int i = 0; i < GRID_DEPTH; i++)
	{
		m_GridHeights[0][i] = 2;
		m_GridHeights[GRID_WIDTH - 1][i] = 2;
	}

	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_DEPTH; j++)
		{
			_grid[i][j].SetPosition(vec3(i, 0, j));
			if ((i % 2 == 0 && j % 2 != 0) || (i % 2 != 0 && j % 2 == 0))
			{
				m_GridColours[i][j] = vec3(1.0f, 1.0f, 1.f);
				_grid[i][j].StoreGridData(i, j, vec3(0.0f, 0.0f, 0.f));
			}
			else
			{
				m_GridColours[i][j] = vec3(1.0f, 1.f, 1.f);
				_grid[i][j].StoreGridData(i, j, vec3(0.0f, 0.f, 0.f));
			}
		}
	}

	// Create Towers
	m_Towers[0].InitializeTower(vec3(25, 0, 10));
	_grid[25][10].isWalkable = false;
	m_Towers[1].InitializeTower(vec3(25, 0, 25));
	_grid[25][25].isWalkable = false;
	m_Towers[2].InitializeTower(vec3(25, 0, 40));
	_grid[25][40].isWalkable = false;
		

	// Create Agents
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		TeamType team = (i < 2) ? TeamType::RED : TeamType::BLUE;
		AgentType type = (i % 2 == 0) ? AgentType::Seeker : AgentType::Capturer;

		m_Agents[i].Init(team, type);
	}
}

void World::Update(float pSeconds)
{
	//For Updating Agents
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		m_Agents[i].Update(pSeconds);
		AgentToWallCollision(&m_Agents[i]);
	}

	//For Updating Towers
	for (int i = 0; i < NUMBER_OF_TOWERS; i++)
	{
		m_Towers[i].Update(pSeconds);
	}

	//For Updating Projectiles
	for (int i = 0; i < m_Projectiles.size(); i++)
	{
		m_Projectiles[i]->Update(pSeconds);
	}

	//Collisions
	BallToBallCollision();
	BallToWallCollision();
}

void World::Render(const IRenderHelpers& pHelper) const
{
	pHelper.Render(this);

	//For Rendering Agents
	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		pHelper.Render(&m_Agents[i]);
	}

	//For Rendering Towers
	for (int i = 0; i < NUMBER_OF_TOWERS; i++)
	{
		pHelper.Render(&m_Towers[i]);
		m_Towers[i].Render(pHelper);
	}

	//For Rendering Balls
	for (int i = 0; i < m_Projectiles.size(); i++)
	{
		pHelper.Render(m_Projectiles[i]);
	}
}

void World::RenderGui(const IGuiHelpers& pHelper)
{
	pHelper.RenderGui(this);

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		pHelper.RenderGui(&m_Agents[i]);
	}
}

Agent* World::FindNearestAgent(const vec3 fromPos, const float radiusFactor, const TeamType towerTeamType)
{
	Agent* nearestAgent = nullptr;
	float shortestDistance = FLT_MAX;

	for (int i = 0; i < NUMBER_OF_AGENTS; i++)
	{
		Agent* currentAgent = &m_Agents[i];
		float distance = length(fromPos - currentAgent->GetPosition());

		if (distance <= radiusFactor && distance < shortestDistance && currentAgent->GetTeamType()!= towerTeamType)
		{
			shortestDistance = distance;
			nearestAgent = currentAgent;
		}
	}
	return nearestAgent;
}

//Checks collision between all the walls
void World::AgentToWallCollision(Agent* agent)
{
	bool collision = false;
	vec3 normal = vec3(0);

	vec3 agent_pos_v;
	if ((agent->GetPosition().x - agent->GetRadius()) < 0.0f || (agent->GetPosition().x + agent->GetRadius()) > GRID_WIDTH - 1)
	{
		collision = true;
		if ((agent->GetPosition().x - agent->GetRadius()) < 0.0f)
		{
			agent_pos_v = agent->GetPosition() - vec3((agent->GetPosition().x - agent->GetRadius()), agent->GetPosition().y, agent->GetPosition().z);
		}
		if ((agent->GetPosition().x + agent->GetRadius()) > GRID_WIDTH - 1)
		{
			agent_pos_v = agent->GetPosition() - vec3((agent->GetPosition().x + agent->GetRadius()), agent->GetPosition().y, agent->GetPosition().z);
		}
		normal = normalize(agent_pos_v);
	}

	if ((agent->GetPosition().z - agent->GetRadius()) < 0.0f || (agent->GetPosition().z + agent->GetRadius()) > GRID_DEPTH - 1)
	{
		collision = true;
		if ((agent->GetPosition().z - agent->GetRadius()) < 0.0f)
		{
			agent_pos_v = agent->GetPosition() - vec3(agent->GetPosition().x, agent->GetPosition().y, (agent->GetPosition().z - agent->GetRadius()));
		}
		if ((agent->GetPosition().z + agent->GetRadius()) > GRID_DEPTH - 1)
		{
			agent_pos_v = agent->GetPosition() - vec3(agent->GetPosition().x, agent->GetPosition().y, (agent->GetPosition().z + agent->GetRadius()));
		}
		normal = normalize(agent_pos_v);
	}

	if (collision)
	{
		float e = 1.0f; //coefficient_of_restitution
		auto initialVelocity = agent->GetVelocity();
		auto finalVelocity = initialVelocity - ((1 + e) * (dot(initialVelocity, normal)) * normal);
		agent->SetVelocity(finalVelocity);
	}
}

void World::AddProjectiles(Ball* ball)
{
	m_Projectiles.push_back(ball);
}

void World::RemoveProjectiles(Ball* ball)
{
	m_Projectiles.erase(remove(m_Projectiles.begin(), m_Projectiles.end(), ball), m_Projectiles.end());
}

//Checks all the ball collision with walls
void World::BallToWallCollision()
{
	for (int i = 0; i < m_Projectiles.size(); i++)
	{
		if (m_Projectiles[i]->GetPosition().x < 1 || (m_Projectiles[i]->GetPosition().x > GRID_WIDTH - 1))
		{
			vec3 normal = vec3(1.0, 0.0, 0.0);
			m_Projectiles[i]->SetVelocity((m_Projectiles[i]->GetVelocity() - 2.f * (normal * m_Projectiles[i]->GetVelocity()) * normal));
		}
		else if (m_Projectiles[i]->GetPosition().z < 1 || m_Projectiles[i]->GetPosition().z > GRID_DEPTH - 1)
		{
			vec3 normal = vec3(0.0, 0.0, 1.0);
			m_Projectiles[i]->SetVelocity((m_Projectiles[i]->GetVelocity() - 2.f * (normal * m_Projectiles[i]->GetVelocity()) * normal));
		}
	}
}

void World::BallToBallCollision()
{
	
	SpatialSegmentationForCollision();

	for (const auto& gridRow : _spatialGridForCollision)
	{
		for (const GridCell1& gridCell : gridRow)
		{
			for (size_t i = 0; i < gridCell.size(); ++i)
			{
				for (size_t j = i + 1; j < gridCell.size(); ++j)
				{
					Ball* ball1 = gridCell[i];
					Ball* ball2 = gridCell[j];

					glm::vec3 v = ball1->GetPosition() - ball2->GetPosition();
					bool collision = false;
					glm::vec3 normal;

					if (glm::length(v) < ball1->GetRadius() + ball2->GetRadius())
					{
						collision = true;
						normal = glm::normalize(v);
					}

					if (collision)
					{
						glm::vec3 VN1 = (((ball1->GetMass() - 1 * ball2->GetMass()) * (glm::dot(ball1->GetVelocity(), normal) * normal) + (ball2->GetMass()
							+ 1 * ball2->GetMass()) * (glm::dot(ball2->GetVelocity(), normal) * normal))) / (ball1->GetMass() + ball2->GetMass());

						glm::vec3 VN2 = (((ball1->GetMass() + 1 * ball1->GetMass()) * (glm::dot(ball1->GetVelocity(), normal) * normal) + (ball2->GetMass()
							- 1 * ball1->GetMass()) * (glm::dot(ball2->GetVelocity(), normal) * normal))) / (ball1->GetMass() + ball2->GetMass());

						glm::vec3 V1 = (ball1->GetVelocity() - (((glm::dot(ball1->GetVelocity(), normal) * normal) + VN1)));
						glm::vec3 V2 = (ball2->GetVelocity() - (((glm::dot(ball2->GetVelocity(), normal) * normal) + VN2)));

						ball1->SetVelocity(-V1);
						ball2->SetVelocity(-V2);
					}
				}
			}
		}
	}
}

// AI Code from here................

vec3 World::GetRandomSpawnPos()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(m_AgentSpawnPos.begin(), m_AgentSpawnPos.end(), std::default_random_engine(seed));

	// Retrieve the first (random) position from the shuffled vector
	return m_AgentSpawnPos[0];
}


Tower* World::FindNearestTower(const vec3 fromPos)
{
	float shortDist = INFINITY;
	Tower* targetTower = nullptr;

	for (int i = 0; i < NUMBER_OF_TOWERS; ++i)
	{
		float dist = length(m_Towers[i].GetPosition() - fromPos);

		if (dist < shortDist && m_Towers[i].GetCapturingAgent() == nullptr)
		{
			targetTower = &m_Towers[i];
			shortDist = dist;
		}
	}
	return targetTower;
}

Agent* World::FindNearestEnemyCapturer(const vec3 fromPos, const TeamType agentTeamType)
{
	Agent* nearestAgent = nullptr;
	float shortestDistance = FLT_MAX;
	for (int i = 0; i < NUMBER_OF_TOWERS; i++)
	{
		Tower* currentTower = &m_Towers[i];
		float distance = length(fromPos - currentTower->GetPosition());

		if (distance < shortestDistance && currentTower->GetCapturingAgent() != nullptr && currentTower->GetCapturingAgent()->GetTeamType() != agentTeamType)
		{
			shortestDistance = distance;
			nearestAgent = currentTower->GetCapturingAgent();
		}
	}
	return nearestAgent;
}

vector<Floor*> World::PathFinding(const vec3& start, const vec3& end)
{
	openList.clear();
	closedList.clear();

	auto checkBoundaries = [&](int x, int z) {
		return (x >= 0 && x < GRID_WIDTH&& z >= 0 && z < GRID_DEPTH);
	};


	auto checkVectorRange = [&](const vector<Floor*>& vector, const Floor* element) {
		return find(vector.begin(), vector.end(), element) != vector.end();
	};

	int startX = (int)start.x;
	int startZ = (int)start.z;
	int endX = (int)end.x;
	int endZ = (int)end.z;


	// Check if the start and end positions are within the valid range
	if (!checkBoundaries(startX, startZ) || !checkBoundaries(endX, endZ))
	{
		return std::vector<Floor*>();
	}

	SpatailSegmentation();

	_grid[startX][startZ].gCost = 0;
	_grid[startX][startZ].hCost = abs(endX - startX) + abs(endZ - startZ);
	_grid[startX][startZ].parent = nullptr;
	openList.push_back(&_grid[startX][startZ]);

	while (!openList.empty()) {

		int lowFcost = 0;

		for (int i = 0; i < openList.size(); i++)
		{
			if (openList[i]->getCost() < openList[lowFcost]->getCost()) {
				lowFcost = i;
			}
		}

		Floor* currentFloor = openList[lowFcost];
		openList.erase(openList.begin() + lowFcost);
		closedList.push_back(currentFloor);

		if (currentFloor->GetPosition() == end) {
			vector<Floor*> path;
			Floor* current = currentFloor;
			while (current != nullptr) {
				path.push_back(current);
				current = current->parent;
			}
			return path;
		}

		for (int x = -1; x <= 1; x++) {
			for (int z = -1; z <= 1; z++) {
				int newX = (int)currentFloor->GetPosition().x + x;
				int newZ = (int)currentFloor->GetPosition().z + z;

				if (!checkBoundaries(newX, newZ) || !_grid[newX][newZ].isWalkable) {
					continue;
				}

				Floor* neighborFloor = &_grid[newX][newZ];
				if (checkVectorRange(closedList, neighborFloor)) {
					continue;
				}

				float gCost = currentFloor->gCost + 1;
				float hCost = abs(end.x - (currentFloor->GetPosition().x + x)) + abs(end.z - (currentFloor->GetPosition().z + z));

				bool isInOpenList = false;
				for (int i = 0; i < openList.size(); i++) {
					if (openList[i] == neighborFloor) {
						isInOpenList = true;

						if (gCost < neighborFloor->gCost) {
							neighborFloor->gCost = gCost;
							neighborFloor->parent = currentFloor;
						}
						break;
					}
				}

				if (!isInOpenList) {
					neighborFloor->gCost = gCost;
					neighborFloor->hCost = hCost;
					neighborFloor->parent = currentFloor;
					openList.push_back(neighborFloor);
				}
			}
		}
	}

	return vector<Floor*>();
}

void World::SetGodMode(bool val)
{
	m_AgentGodMode = val;
}

void World::SetFleeMode(bool val)
{
	m_FleePatrolMode = val;
}

void World::SpatailSegmentation()
{
	// Clear the existing spatial grid
	_spatialGrid.clear();

	// Calculate the number of grid cells in the x and z directions based on the cell size
	int numCellsX = GRID_WIDTH / GRID_CELL_SIZE;
	int numCellsZ = GRID_DEPTH / GRID_CELL_SIZE;

	// Resize the spatial grid to accommodate the calculated number of cells
	_spatialGrid.resize(numCellsX, std::vector<GridCell>(numCellsZ));

	// Iterate through all Floor objects and insert them into the corresponding grid cell
	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		for (int j = 0; j < GRID_DEPTH; ++j)
		{
			int gridCellX = i / GRID_CELL_SIZE;
			int gridCellZ = j / GRID_CELL_SIZE;

			// Check if the gridCellX and gridCellZ are within the valid range
			if (gridCellX >= 0 && gridCellX < numCellsX && gridCellZ >= 0 && gridCellZ < numCellsZ)
			{
				_spatialGrid[gridCellX][gridCellZ].push_back(&_grid[i][j]);
			}
			else
			{

			}
		}
	}
}

void World::SpatialSegmentationForCollision()
{
	_spatialGridForCollision.clear();

	// Calculate the number of grid cells in the x and z directions based on the cell size
	int numCellsX = GRID_WIDTH / GRID_CELL_SIZE;
	int numCellsZ = GRID_DEPTH / GRID_CELL_SIZE;

	// Resize the spatial grid to accommodate the calculated number of cells
	_spatialGridForCollision.resize(numCellsX, std::vector<GridCell1>(numCellsZ));

	// Iterate through all Ball objects and insert them into the corresponding grid cell
	for (Ball* ball : m_Projectiles)
	{
		int gridCellX = (int)ball->GetPosition().x / GRID_CELL_SIZE;
		int gridCellZ = (int)ball->GetPosition().z / GRID_CELL_SIZE;

		// Check if the gridCellX and gridCellZ are within the valid range
		if (gridCellX >= 0 && gridCellX < numCellsX && gridCellZ >= 0 && gridCellZ < numCellsZ)
		{
			_spatialGridForCollision[gridCellX][gridCellZ].push_back(ball);
		}
		else
		{
			
		}
	}
}
