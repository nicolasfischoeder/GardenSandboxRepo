#pragma once

#include "CoreMinimal.h"
#include "GardenBuildingBase.h"
#include "GardenGhostBuildingBase.generated.h"

/** Base class for ghost versions of buildings used during placement */
UCLASS()
class GARDENSANDBOX_API AGardenGhostBuildingBase : public AGardenBuildingBase
{
    GENERATED_BODY()
public:
    AGardenGhostBuildingBase();
};
