#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "State.generated.h"

UINTERFACE(BlueprintType)
class GARDENSANDBOX_API UState : public UInterface
{
    GENERATED_BODY()
};

class GARDENSANDBOX_API IState
{
    GENERATED_BODY()

public:
    virtual void Enter() = 0;
    virtual void Execute() = 0;
    virtual void Exit() = 0;
};
