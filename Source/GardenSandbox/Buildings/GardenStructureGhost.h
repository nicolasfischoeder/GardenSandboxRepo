#pragma once

#include "CoreMinimal.h"
#include "GardenBuildingBase.h"
#include "GardenStructureGhost.generated.h"

/** Ghost class for AGardenStructure used during placement */
UCLASS()
class GARDENSANDBOX_API AGardenStructureGhost : public AGardenBuildingBase
{
    GENERATED_BODY()
public:
    AGardenStructureGhost();

    /**
     * Change ghost material based on placement validity
     */
    UFUNCTION(BlueprintCallable, Category="Building")
    void SetPlacementValid(bool bIsValid);

protected:
    /** Material used for valid placement */
    UPROPERTY()
    UMaterialInterface* ValidMaterial;

    /** Material used for invalid placement */
    UPROPERTY()
    UMaterialInterface* InvalidMaterial;

};
