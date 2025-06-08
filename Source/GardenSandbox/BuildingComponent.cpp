#include "BuildingComponent.h"
#include "GardenSandboxCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "Buildings/GardenStructureGhost.h"
#include "Buildings/GardenStructure.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Buildings/GardenStructureGhost.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInterface.h"
#include "Components/MeshComponent.h"
#include "Buildings/GardenBuildingBase.h"
#include "ResourceComponent.h"
#include "BuildingDataAsset.h"
#include "Net/UnrealNetwork.h"

UBuildingComponent::UBuildingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
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

    }

    return true;
}

void UBuildingComponent::StartPlacement()
{
    UE_LOG(LogTemp, Display, TEXT("StartPlacement"));   
    if (bIsPlacing || !BuildingData || !BuildingData->BuildingClass)
    {
        if (bIsPlacing) UE_LOG(LogTemp, Display, TEXT("Is already placing."));
        if (!BuildingData) UE_LOG(LogTemp, Display, TEXT("No BuildingData."));
        if (!BuildingData->BuildingClass) UE_LOG(LogTemp, Display, TEXT("No BuildingData but BuildingClass."));

        return;
    }

    if (GetOwner()->HasAuthority())
    {
        SpawnGhost();
    }
    else
    {
        ServerStartPlacement();
    }
}

void UBuildingComponent::SpawnGhost()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
    UE_LOG(LogTemp, Display, TEXT("Spawning Ghost"));   

    TSubclassOf<AGardenBuildingBase> GhostToSpawn = BuildingData->GhostClass;
    AGardenBuildingBase* Tmp = World->SpawnActor<AGardenBuildingBase>(GhostToSpawn, FVector::ZeroVector, FRotator::ZeroRotator);
    GhostActor = Cast<AGardenStructureGhost>(Tmp);

    if (GhostActor)
    {
        UE_LOG(LogTemp, Display, TEXT("Ghost Spawned"));
        UE_LOG(LogTemp, Display, TEXT("Ghost name: %s"),*GhostActor->GetName());
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
        UpdateGhostVisual();
        bIsPlacing = true;
        CurrentYaw = 0.f;
        UE_LOG(LogTemp, Display, TEXT("Ghost SpawnProcedure completed."));   

    }

}

void UBuildingComponent::ServerStartPlacement_Implementation()
{
    SpawnGhost();
}

void UBuildingComponent::ServerUpdateGhostTransform_Implementation(const FVector& Location, const FRotator& Rotation)
{
    if (GhostActor)
    {
        GhostActor->SetActorLocation(Location);
        GhostActor->SetActorRotation(Rotation);
        UpdateGhostVisual();
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
        bPlacementValid = true;
        bIsPlacing = false;
    }
}

void UBuildingComponent::AddBindings(UEnhancedInputComponent* EIC)
{
    AddBindings(EIC, true, nullptr);
}

void UBuildingComponent::AddBindings(UEnhancedInputComponent* EIC, UInputAction* InRotateAction)
{
    AddBindings(EIC, true, InRotateAction);
}

void UBuildingComponent::AddBindings(UEnhancedInputComponent* EIC, bool bBindRotate, UInputAction* InRotateAction)
{
    if (!EIC)
    {
        return;
    }

    EIC->BindAction(StartBuildingAction, ETriggerEvent::Triggered, this, &UBuildingComponent::StartPlacement);
    EIC->BindAction(PlaceAction, ETriggerEvent::Triggered, this, &UBuildingComponent::Place);
    EIC->BindAction(CancelAction, ETriggerEvent::Triggered, this, &UBuildingComponent::Cancel);

    if (bBindRotate)
    {
        UInputAction* ActionToBind = InRotateAction ? InRotateAction : RotateAction;
        if (ActionToBind)
        {
            EIC->BindAction(ActionToBind, ETriggerEvent::Triggered, this, &UBuildingComponent::Rotate);
        }
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

    if (bIsPlacing && GhostActor && Character && Character->IsLocallyControlled())
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

        FRotator NewRot = FRotator(0.f, CamRot.Yaw + CurrentYaw, 0.f);
        GhostActor->SetActorRotation(NewRot);

        if (!GetOwner()->HasAuthority())
        {
            ServerUpdateGhostTransform(GhostActor->GetActorLocation(), NewRot);
        }

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
        if (GhostActor)
        {
            GhostActor->SetPlacementValid(bPlacementValid);
        }
    }
}

void UBuildingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UBuildingComponent, GhostActor);
    DOREPLIFETIME(UBuildingComponent, bIsPlacing);
}


