#include "State.h"
#include "Floor.h"

enum class TargetType
{
    Tower,
    Agent,
    None,
};

class Navigate : public State {

public:

    Navigate(GameEntity* target, const TargetType targetType) {
        m_Target = target;
        m_TargetType = targetType;
    }

    void OnStateEnter() override;
    void OnStateExit() override;
    void OnStateUpdate(const float pSeconds) override;
    void DestinationReached();
    void EndNavigation();

private:
    State* m_CurrentState;
    GameEntity* m_Target;
    TargetType m_TargetType;
    bool m_ShouldCalculate;
    vector<Floor*> m_PathList;
    int m_PathIndex = 0;
};