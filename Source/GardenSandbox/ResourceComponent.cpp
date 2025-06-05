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
    if (FResourceAmount* Existing = Resources.FindByPredicate([ResourceName](const FResourceAmount& Res)
    {
        return Res.Type == ResourceName;
    }))
    {
        Existing->Amount += Amount;
    }
    else
    {
        FResourceAmount NewRes;
        NewRes.Type = ResourceName;
        NewRes.Amount = Amount;
        Resources.Add(NewRes);
    }
}


bool UResourceComponent::ConsumeResources(const TArray<FResourceAmount>& Costs)
{
    for (const FResourceAmount& Cost : Costs)
    {
        FResourceAmount* Current = Resources.FindByPredicate([&Cost](const FResourceAmount& Res)
        {
            return Res.Type == Cost.Type;
        });
        if (!Current || Current->Amount < Cost.Amount)
        {
            return false;
        }
    }

    for (const FResourceAmount& Cost : Costs)
    {
        FResourceAmount* Current = Resources.FindByPredicate([&Cost](const FResourceAmount& Res)
        {
            return Res.Type == Cost.Type;
        });
        if (Current)
        {
            Current->Amount -= Cost.Amount;
        }
        else
        {
            // Should not happen due to above checks, but handle gracefully
            FResourceAmount NewRes;
            NewRes.Type = Cost.Type;
            NewRes.Amount = -Cost.Amount;
            Resources.Add(NewRes);
        }
    }

    return true;
}

void UResourceComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UResourceComponent, Resources);
}

