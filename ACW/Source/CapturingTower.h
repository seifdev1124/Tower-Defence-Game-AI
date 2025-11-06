#include "State.h"

class Tower;
class CapturingTower : public State
{
public:

    CapturingTower(Tower* tower){
        m_Tower = tower;
    }

    void OnStateEnter() override;
    void OnStateExit() override;
    void OnStateUpdate(const float pSeconds) override;

private:
    Tower* m_Tower;
    float m_InfluenceValue;
    float m_InfluenceTimer;
};