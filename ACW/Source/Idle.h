#include "State.h"

class Idle : public State
{
public:

    Idle(const vec3 startPos) {
        m_StartPos = startPos;
    }

    void OnStateEnter() override;
    void OnStateExit() override;
    void OnStateUpdate(const float pSeconds) override;

private:
    vec3 m_StartPos;
};