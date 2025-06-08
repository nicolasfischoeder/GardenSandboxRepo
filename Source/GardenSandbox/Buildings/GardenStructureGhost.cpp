#include "GardenStructureGhost.h"
#include "Components/MeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AGardenStructureGhost::AGardenStructureGhost()
    : ValidMaterial(nullptr)
    , InvalidMaterial(nullptr)
{
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> ValidMatObj(TEXT("/Game/Materials/Ghost/BuildingGhostMaterial_Green.BuildingGhostMaterial_Green"));
    if (ValidMatObj.Succeeded())
    {
        ValidMaterial = ValidMatObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> InvalidMatObj(TEXT("/Game/Materials/Ghost/BuildingGhostMaterial_Red.BuildingGhostMaterial_Red"));
    if (InvalidMatObj.Succeeded())
    {
        InvalidMaterial = InvalidMatObj.Object;
    }
}

void AGardenStructureGhost::SetPlacementValid(bool bIsValid)
{
    UMaterialInterface* Mat = bIsValid ? ValidMaterial : InvalidMaterial;
    if (!Mat)
    {
        return;
    }

    TArray<UMeshComponent*> MeshComponents;
    GetComponents(MeshComponents);
    for (UMeshComponent* Mesh : MeshComponents)
    {
        if (!Mesh)
        {
            continue;
        }

        int32 Count = Mesh->GetNumMaterials();
        for (int32 i = 0; i < Count; ++i)
        {
            Mesh->SetMaterial(i, Mat);
        }
    }
}
