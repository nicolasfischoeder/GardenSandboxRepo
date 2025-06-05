#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/DamageType.h"
#include "HealthComponent.generated.h"

USTRUCT(BlueprintType)
struct FDamageMultiplier
{
    GENERATED_BODY()

    /** DamageType affected by this multiplier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
    TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();

    /** Multiplier applied to incoming damage of the given type */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
    float Multiplier = 1.f;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GARDENSANDBOX_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UHealthComponent();

    /** Maximum health value */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
    float MaxHealth = 100.f;

    /** Current health replicated to clients */
    UPROPERTY(ReplicatedUsing=OnRep_Health, VisibleAnywhere, BlueprintReadOnly, Category="Health")
    float Health = 100.f;

    /** Multipliers for specific damage types */
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Damage")
    TArray<FDamageMultiplier> DamageMultipliers;

    /** Check whether the actor is still alive */
    UFUNCTION(BlueprintCallable, Category="Health")
    bool IsAlive() const { return Health > 0.f; }

    /** Apply healing */
    UFUNCTION(BlueprintCallable, Category="Health")
    void Heal(float Amount);

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnRep_Health();

    UFUNCTION()
    void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

