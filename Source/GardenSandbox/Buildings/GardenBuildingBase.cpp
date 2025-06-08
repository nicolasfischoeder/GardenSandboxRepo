#include "GardenBuildingBase.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../HealthComponent.h"

AGardenBuildingBase::AGardenBuildingBase()
{
    PrimaryActorTick.bCanEverTick = false;

    bReplicates = true;
    SetReplicateMovement(true);

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;


}

void AGardenBuildingBase::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

}
