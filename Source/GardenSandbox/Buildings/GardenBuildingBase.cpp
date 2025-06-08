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

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);


}

void AGardenBuildingBase::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (MeshComponent && MeshComponent->GetStaticMesh())
    {
        const float HalfHeight = MeshComponent->GetStaticMesh()->GetBoundingBox().GetExtent().Z;
        MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, HalfHeight));

    }
}
