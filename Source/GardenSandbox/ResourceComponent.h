#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
    Wood        UMETA(DisplayName="Holz"),
    Stone       UMETA(DisplayName="Stein"),
    PlantFiber  UMETA(DisplayName="Pflanzenfasern"),
    Iron        UMETA(DisplayName="Eisen"),
    Copper      UMETA(DisplayName="Kupfer"),
    Vegetable   UMETA(DisplayName="Gemüse"),
    Berry       UMETA(DisplayName="Beeren"),
    Herb        UMETA(DisplayName="Kräuter")
};

#include "ResourceComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GARDENSANDBOX_API UResourceComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UResourceComponent();

    /** Adds resources of the given type */
    UFUNCTION(BlueprintCallable, Category="Resources")
    void AddResource(EResourceType Resource, int32 Amount);

    /** Tries to consume resources. Returns true on success */
    UFUNCTION(BlueprintCallable, Category="Resources")
    bool ConsumeResource(EResourceType Resource, int32 Amount);

    /** Get current amount of resource */
    UFUNCTION(BlueprintCallable, Category="Resources")
    int32 GetResourceAmount(EResourceType Resource) const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Resources")
    TMap<EResourceType, int32> Resources;
};

