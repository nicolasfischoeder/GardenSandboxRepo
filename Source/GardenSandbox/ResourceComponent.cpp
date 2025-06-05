#include "ResourceComponent.h"
#include "Net/UnrealNetwork.h"

UResourceComponent::UResourceComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UResourceComponent::AddResource(EResourceType ResourceName, int32 Amount)
{
    if (Amount <= 0)
    {
        return;
    }
    int32& Current = Resources.FindOrAdd(ResourceName);
    Current += Amount;
}

bool UResourceComponent::ConsumeResource(EResourceType ResourceName, int32 Amount)
{
    if (Amount <= 0)
    {
        return true;
    }
    int32* Current = Resources.Find(ResourceName);
    if (!Current || *Current < Amount)
    {
        return false;
    }
    *Current -= Amount;
    return true;
}

int32 UResourceComponent::GetResourceAmount(EResourceType ResourceName) const
{
    const int32* Current = Resources.Find(ResourceName);
    return Current ? *Current : 0;
}

bool UResourceComponent::ConsumeResources(const TArray<FResourceAmount>& Costs)
{
    for (const FResourceAmount& Cost : Costs)
    {
        int32* Current = Resources.Find(Cost.Type);
        if (!Current || *Current < Cost.Amount)
        {
            return false;
        }
    }

    for (const FResourceAmount& Cost : Costs)
    {
        int32& Current = Resources.FindOrAdd(Cost.Type);
        Current -= Cost.Amount;
    }

    return true;
}

void UResourceComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UResourceComponent, Resources);
}
