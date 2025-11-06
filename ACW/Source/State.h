#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;
using namespace std;

class Agent;

class State {

public:
    virtual void OnStateEnter() = 0;
    virtual void OnStateExit() = 0;
    virtual void OnStateUpdate(const float pSeconds) = 0;
    Agent* owningAgent;
};