#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Buildings/GardenStructure.h"
#include "Buildings/GardenStructureGhost.h"
#include "BuildingComponent.generated.h"

class AGardenSandboxCharacter;
class UInputAction;
class UInputMappingContext;
class UMaterialInterface;
class UBuildingDataAsset;
class AGardenBuildingBase;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GARDENSANDBOX_API UBuildingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBuildingComponent();

    /** Size of the placement grid. Set to 0 for no snapping */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building")
    float GridSize = 100.f;

    /** Additional yaw rotation applied to the ghost */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building")
    float RotationStep = 90.f;

    /** Data asset describing the building to place */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building")
    UBuildingDataAsset* BuildingData;

    /** Material used when placement is valid */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building|Visual")
    UMaterialInterface* ValidPlacementMaterial;

    /** Material used when placement is invalid */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building|Visual")
    UMaterialInterface* InvalidPlacementMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
    UInputMappingContext* BuildMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
    UInputAction* StartBuildingAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
    UInputAction* PlaceAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
    UInputAction* CancelAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
    UInputAction* RotateAction;

    bool AttachComponent(AGardenSandboxCharacter* TargetCharacter);

    /**
    * Bind all input actions used by this component. Optionally supply a
    * custom rotate action or disable the rotate binding entirely.
    */
    void AddBindings(UEnhancedInputComponent* EIC);
    void AddBindings(UEnhancedInputComponent* EIC, UInputAction* InRotateAction);
    void AddBindings(UEnhancedInputComponent* EIC, bool bBindRotate, UInputAction* InRotateAction = nullptr);

    void StartPlacement();
    void Place();
    void Cancel();
    void Rotate();

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    
    AGardenStructureGhost* GhostActor;
    AGardenSandboxCharacter* Character;
    bool bIsPlacing;
    float CurrentYaw;
    bool bPlacementValid;

    void UpdateGhostVisual();
};

