#pragma once

#include "CoreMinimal.h"
#include "Buildings/GardenGhostBuildingBase.h"
#include "GardenStructureGhost.generated.h"

/** Ghost class for AGardenStructure used during placement */
UCLASS()
class GARDENSANDBOX_API AGardenStructureGhost : public AGardenGhostBuildingBase
{
    GENERATED_BODY()
public:
    AGardenStructureGhost();
};
