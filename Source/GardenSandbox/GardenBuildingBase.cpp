#include "GardenBuildingBase.h"
#include "Components/StaticMeshComponent.h"

AGardenBuildingBase::AGardenBuildingBase()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}
