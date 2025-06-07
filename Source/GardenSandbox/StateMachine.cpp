#include "StateMachine.h"

UStateMachine::UStateMachine()
{
}

void UStateMachine::SetState(TScriptInterface<IState> NewState)
{
    if (CurrentState == NewState)
    {
        return;
    }

    TScriptInterface<IState> OldState = CurrentState;
    if (CurrentState)
    {
        CurrentState->Exit();
    }

    CurrentState = NewState;

    if (CurrentState)
    {
        CurrentState->Enter();
    }

    OnStateChanged.Broadcast(OldState, CurrentState);
}
