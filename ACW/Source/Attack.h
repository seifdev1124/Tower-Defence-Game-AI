#include "State.h"

class Attack : public State
{
public:

    Attack(Agent* attackedAgent) {
        m_AttackedAgent = attackedAgent;
    }

    void OnStateEnter() override;
    void OnStateExit() override;
    void OnStateUpdate(const float pSeconds) override;

private:
    Agent* m_AttackedAgent;
};