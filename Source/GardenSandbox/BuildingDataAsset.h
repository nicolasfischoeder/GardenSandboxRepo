#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ResourceComponent.h"
#include "BuildingDataAsset.generated.h"

class AGardenBuildingBase;

/** Data Asset describing a placeable building */
UCLASS(BlueprintType)
class GARDENSANDBOX_API UBuildingDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()
public:
    /** Actor class spawned when the building is placed */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Building")
    TSubclassOf<AGardenBuildingBase> BuildingClass;

    /** Optional ghost class used while placing. If null BuildingClass is used */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Building")
    TSubclassOf<AActor> GhostClass;

    /** Type of resource required to build */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resources")
    EResourceType RequiredResource = EResourceType::Wood;

    /** Amount of the resource required */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resources")
    int32 ResourceCost = 1;
};
