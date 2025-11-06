#include "State.h"

class Patrol : public State
{
public:

    Patrol() {
        
    }

    void OnStateEnter() override;
    void OnStateExit() override;
    void OnStateUpdate(const float pSeconds) override;

private:
    Agent* m_Target;
};