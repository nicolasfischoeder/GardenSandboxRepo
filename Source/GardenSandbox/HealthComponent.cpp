#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;

    if (AActor* Owner = GetOwner())
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
    }
}

void UHealthComponent::Heal(float Amount)
{
    if (Amount <= 0.f || Health <= 0.f)
    {
        return;
    }

    Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.f || Health <= 0.f)
    {
        return;
    }

    float ActualDamage = Damage;
    if (DamageType)
    {
        for (const FDamageMultiplier& Mult : DamageMultipliers)
        {
            if (DamageType->IsA(Mult.DamageType))
            {
                ActualDamage *= Mult.Multiplier;
                break;
            }
        }
    }

    Health = FMath::Clamp(Health - ActualDamage, 0.f, MaxHealth);
}

void UHealthComponent::OnRep_Health()
{
    // Could trigger UI updates here
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UHealthComponent, Health);
    DOREPLIFETIME(UHealthComponent, DamageMultipliers);
}

