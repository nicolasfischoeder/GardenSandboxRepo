#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GardenBuildingBase.generated.h"

UCLASS()
class GARDENSANDBOX_API AGardenBuildingBase : public AActor
{
    GENERATED_BODY()

public:
    AGardenBuildingBase();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* MeshComponent;

};

