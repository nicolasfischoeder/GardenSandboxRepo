#include "ResourceComponent.h"

UResourceComponent::UResourceComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
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

