#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "State.h"
#include "StateMachine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateChanged, TScriptInterface<IState>, OldState, TScriptInterface<IState>, NewState);

UCLASS(Blueprintable, BlueprintType)
class GARDENSANDBOX_API UStateMachine : public UObject
{
    GENERATED_BODY()
public:
    UStateMachine();

    UPROPERTY(BlueprintAssignable, Category="State")
    FOnStateChanged OnStateChanged;

    UPROPERTY()
    TScriptInterface<IState> CurrentState;

    UFUNCTION(BlueprintCallable, Category="State")
    void SetState(TScriptInterface<IState> NewState);
};
