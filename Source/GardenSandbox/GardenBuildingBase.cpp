#include "GardenBuildingBase.h"
#include "Components/StaticMeshComponent.h"
#include "HealthComponent.h"

AGardenBuildingBase::AGardenBuildingBase()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}
