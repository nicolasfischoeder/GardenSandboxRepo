#include "Buildings/GardenGhostBuildingBase.h"
#include "Components/StaticMeshComponent.h"

AGardenGhostBuildingBase::AGardenGhostBuildingBase()
{
    if (MeshComponent)
    {
        MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}
