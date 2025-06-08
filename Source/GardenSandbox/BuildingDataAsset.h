#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ResourceComponent.h"
#include "BuildingDataAsset.generated.h"

class AGardenStructure;
class AGardenStructureGhost;

/** Data Asset describing a placeable building */
UCLASS(BlueprintType)
class GARDENSANDBOX_API UBuildingDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()
public:
    /** Actor class spawned when the building is placed */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Building")
    TSubclassOf<AGardenStructure> BuildingClass;

    /** Optional ghost class used while placing. If null BuildingClass is used */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Building")
    TSubclassOf<AGardenStructureGhost> GhostClass;

    /** Resources required to build */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resources")
    TArray<FResourceAmount> RequiredResources;

};
