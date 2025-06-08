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

};
