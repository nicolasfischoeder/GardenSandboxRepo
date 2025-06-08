#include "GardenGhostBuildingBase.h"
#include "Components/StaticMeshComponent.h"

AGardenGhostBuildingBase::AGardenGhostBuildingBase()
{
    bReplicates = false;
    SetReplicateMovement(false);

    if (MeshComponent)
    {
        MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}
