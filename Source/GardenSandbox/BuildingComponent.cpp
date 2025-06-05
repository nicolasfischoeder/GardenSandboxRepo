#include "BuildingComponent.h"
#include "GardenSandboxCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInterface.h"
#include "Components/MeshComponent.h"
#include "GardenBuildingBase.h"
#include "ResourceComponent.h"
#include "BuildingDataAsset.h"

UBuildingComponent::UBuildingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    GhostActor = nullptr;
    Character = nullptr;
    bIsPlacing = false;
    CurrentYaw = 0.f;
    bPlacementValid = true;
}

bool UBuildingComponent::AttachComponent(AGardenSandboxCharacter* TargetCharacter)
{
    Character = TargetCharacter;

    if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UBuildingComponent>())
    {
        return false;
    }

    if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(BuildMappingContext, 1);
        }

        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
        {
            EnhancedInputComponent->BindAction(StartBuildingAction, ETriggerEvent::Triggered, this, &UBuildingComponent::StartPlacement);
            EnhancedInputComponent->BindAction(PlaceAction, ETriggerEvent::Triggered, this, &UBuildingComponent::Place);
            EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Triggered, this, &UBuildingComponent::Cancel);
            if (RotateAction)
            {
                EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &UBuildingComponent::Rotate);
            }
        }
    }

    return true;
}

void UBuildingComponent::StartPlacement()
{
    if (bIsPlacing || !BuildingData || !BuildingData->BuildingClass)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    TSubclassOf<AActor> GhostToSpawn = BuildingData->GhostClass ? BuildingData->GhostClass : BuildingData->BuildingClass;
    GhostActor = World->SpawnActor<AActor>(GhostToSpawn, FVector::ZeroVector, FRotator::ZeroRotator);
    if (GhostActor)
    {
        GhostActor->SetActorEnableCollision(false);
        TArray<UActorComponent*> Comps;
        GhostActor->GetComponents(UPrimitiveComponent::StaticClass(), Comps);
        for (UActorComponent* Comp : Comps)
        {
            if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Comp))
            {
                Prim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }
        }
        GhostActor->GetComponents<UMeshComponent>(GhostMeshComponents);
        UpdateGhostVisual();
        bIsPlacing = true;
        CurrentYaw = 0.f;
    }
}

void UBuildingComponent::Place()
{
    if (!bIsPlacing || !GhostActor || !BuildingData || !BuildingData->BuildingClass)
    {
        return;
    }

    if (Character && Character->ResourceComponent)
    {
        if (!Character->ResourceComponent->ConsumeResources(BuildingData->RequiredResources))
        {
            return;
        }
    }

    FVector Loc = GhostActor->GetActorLocation();
    FRotator Rot = GhostActor->GetActorRotation();
    GhostActor->Destroy();
    GhostActor = nullptr;
    GhostMeshComponents.Empty();
    bPlacementValid = true;

    if (Character && Character->ResourceComponent)
    {
        // Older versions consumed a single resource type here.
        // This is now handled by ConsumeResources above.
    }

    if (UWorld* World = GetWorld())
    {
        World->SpawnActor<AActor>(BuildingData->BuildingClass, Loc, Rot);
    }

    bIsPlacing = false;
}

void UBuildingComponent::Cancel()
{
    if (bIsPlacing)
    {
        if (GhostActor)
        {
            GhostActor->Destroy();
            GhostActor = nullptr;
        }
        GhostMeshComponents.Empty();
        bPlacementValid = true;
        bIsPlacing = false;
    }
}

void UBuildingComponent::Rotate()
{
    if (bIsPlacing)
    {
        CurrentYaw += RotationStep;
        if (CurrentYaw > 360.f)
        {
            CurrentYaw -= 360.f;
        }
    }
}

void UBuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsPlacing && GhostActor && Character)
    {
        FVector CamLoc;
        FRotator CamRot;
        Character->GetActorEyesViewPoint(CamLoc, CamRot);

        FVector TraceStart = CamLoc;
        FVector TraceEnd = CamLoc + CamRot.Vector() * 1000.f;

        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Character);

        if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
        {
            FVector Loc = Hit.Location;
            if (GridSize > 0.f)
            {
                Loc.X = FMath::GridSnap(Loc.X, GridSize);
                Loc.Y = FMath::GridSnap(Loc.Y, GridSize);
                Loc.Z = FMath::GridSnap(Loc.Z, GridSize);
            }
            GhostActor->SetActorLocation(Loc);
        }
        else
        {
            FVector Loc = TraceEnd;
            if (GridSize > 0.f)
            {
                Loc.X = FMath::GridSnap(Loc.X, GridSize);
                Loc.Y = FMath::GridSnap(Loc.Y, GridSize);
                Loc.Z = FMath::GridSnap(Loc.Z, GridSize);
            }
            GhostActor->SetActorLocation(Loc);
        }

        GhostActor->SetActorRotation(FRotator(0.f, CamRot.Yaw + CurrentYaw, 0.f));

        UpdateGhostVisual();
    }
}

void UBuildingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (bIsPlacing)
    {
        Cancel();
    }

    if (Character)
    {
        if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
            {
                Subsystem->RemoveMappingContext(BuildMappingContext);
            }
        }
    }

    Super::EndPlay(EndPlayReason);
}

void UBuildingComponent::UpdateGhostVisual()
{
    if (!GhostActor)
    {
        return;
    }

    FVector Origin;
    FVector BoxExtent;
    GhostActor->GetActorBounds(true, Origin, BoxExtent);
    FCollisionShape Box = FCollisionShape::MakeBox(BoxExtent);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GhostActor);
    if (Character)
    {
        Params.AddIgnoredActor(Character);
    }

    FCollisionObjectQueryParams ObjParams;
    ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    ObjParams.AddObjectTypesToQuery(ECC_Pawn);

    bool bNewValid = !GetWorld()->OverlapAnyTestByObjectType(Origin, FQuat::Identity, ObjParams, Box, Params);
    if (bPlacementValid != bNewValid)
    {
        bPlacementValid = bNewValid;
        for (UMeshComponent* Mesh : GhostMeshComponents)
        {
            if (!Mesh)
            {
                continue;
            }
            UMaterialInterface* Mat = bPlacementValid ? ValidPlacementMaterial : InvalidPlacementMaterial;
            if (Mat)
            {
                int32 Count = Mesh->GetNumMaterials();
                for (int32 i = 0; i < Count; ++i)
                {
                    Mesh->SetMaterial(i, Mat);
                }
            }
        }
    }
}

