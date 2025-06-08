// Copyright Epic Games, Inc. All Rights Reserved.

#include "GardenSandboxCharacter.h"
#include "GardenSandboxProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BuildingComponent.h"
#include "ResourceComponent.h"
#include "HealthComponent.h"
#include "Engine/CollisionProfile.h"
#include "UObject/ConstructorHelpers.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGardenSandboxCharacter

AGardenSandboxCharacter::AGardenSandboxCharacter()
{
       // Load input mapping contexts
       static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMC(TEXT("/Game/FirstPerson/Input/IMC_Default.IMC_Default"));
       if (DefaultMC.Succeeded())
       {
               DefaultMappingContext = DefaultMC.Object;
       }

       UInputMappingContext* LoadedBuildMapping = nullptr;
       static ConstructorHelpers::FObjectFinder<UInputMappingContext> BuildMC(TEXT("/Game/FirstPerson/Input/IMC_Building.IMC_Building"));
       if (BuildMC.Succeeded())
       {
               LoadedBuildMapping = BuildMC.Object;
       }

       // Load input actions
       static ConstructorHelpers::FObjectFinder<UInputAction> JumpAct(TEXT("/Game/FirstPerson/Input/Actions/IA_Jump.IA_Jump"));
       if (JumpAct.Succeeded())
       {
               JumpAction = JumpAct.Object;
       }

       static ConstructorHelpers::FObjectFinder<UInputAction> MoveAct(TEXT("/Game/FirstPerson/Input/Actions/IA_Move.IA_Move"));
       if (MoveAct.Succeeded())
       {
               MoveAction = MoveAct.Object;
       }

       static ConstructorHelpers::FObjectFinder<UInputAction> LookAct(TEXT("/Game/FirstPerson/Input/Actions/IA_Look.IA_Look"));
       if (LookAct.Succeeded())
       {
               LookAction = LookAct.Object;
       }

       UInputAction* LoadedStartBuilding = nullptr;
       static ConstructorHelpers::FObjectFinder<UInputAction> StartBuildAct(TEXT("/Game/FirstPerson/Input/Actions/IA_EnterBuildingMode.IA_EnterBuildingMode"));
       if (StartBuildAct.Succeeded())
       {
               LoadedStartBuilding = StartBuildAct.Object;
       }

       UInputAction* LoadedPlace = nullptr;
       static ConstructorHelpers::FObjectFinder<UInputAction> PlaceAct(TEXT("/Game/FirstPerson/Input/Actions/IA_Place.IA_Place"));
       if (PlaceAct.Succeeded())
       {
               LoadedPlace = PlaceAct.Object;
       }

       UInputAction* LoadedCancel = nullptr;
       static ConstructorHelpers::FObjectFinder<UInputAction> CancelAct(TEXT("/Game/FirstPerson/Input/Actions/IA_CancelPlacement.IA_CancelPlacement"));
       if (CancelAct.Succeeded())
       {
               LoadedCancel = CancelAct.Object;
       }

       UInputAction* LoadedRotate = nullptr;
       static ConstructorHelpers::FObjectFinder<UInputAction> RotateAct(TEXT("/Game/FirstPerson/Input/Actions/IA_RotateBuilding.IA_RotateBuilding"));
       if (RotateAct.Succeeded())
       {
               LoadedRotate = RotateAct.Object;

       }

       // Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
       Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

       // Create building component
       BuildingComponent = CreateDefaultSubobject<UBuildingComponent>(TEXT("BuildingComponent"));
       if (BuildingComponent)
       {
               BuildingComponent->BuildMappingContext = LoadedBuildMapping;
               BuildingComponent->StartBuildingAction = LoadedStartBuilding;
               BuildingComponent->PlaceAction = LoadedPlace;
               BuildingComponent->CancelAction = LoadedCancel;
               BuildingComponent->RotateAction = LoadedRotate;
       }

    // Create resource component
    ResourceComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("ResourceComponent"));

    // Create health component
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

        // Stellen Sie sicher, dass das 1P-Mesh nur der Owner sieht
	Mesh1P->SetOnlyOwnerSee(true);
	//Mesh1P->SetOwnerNoSee(false);

	GetMesh()->SetOwnerNoSee(true);
	
}

//////////////////////////////////////////////////////////////////////////// Input
void AGardenSandboxCharacter::NotifyControllerChanged()
{
        Super::NotifyControllerChanged();

        // Add Input Mapping Context
        if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
        {
                if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
                {
                        Subsystem->AddMappingContext(DefaultMappingContext, 0);
                }
        }

       if (BuildingComponent)
       {
                BuildingComponent->AttachComponent(this);
       }
}

void AGardenSandboxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
        {
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGardenSandboxCharacter::Move);

		// Looking
                EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGardenSandboxCharacter::Look);

                if (BuildingComponent)
                {
                        BuildingComponent->AddBindings(EnhancedInputComponent);
                }
        }
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AGardenSandboxCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGardenSandboxCharacter::Look(const FInputActionValue& Value)
{
        // input is a Vector2D
        FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
                AddControllerYawInput(LookAxisVector.X);
                AddControllerPitchInput(LookAxisVector.Y);
        }
}

void AGardenSandboxCharacter::BeginPlay()
{
        Super::BeginPlay();

        SetupHitboxes();
}

void AGardenSandboxCharacter::SetupHitboxes()
{
        USkeletalMeshComponent* ThirdPersonMesh = GetMesh();
        if (!ThirdPersonMesh)
        {
                return;
        }

        ThirdPersonMesh->SetCollisionProfileName(TEXT("Pawn"));
        ThirdPersonMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        ThirdPersonMesh->SetGenerateOverlapEvents(true);
        ThirdPersonMesh->SetNotifyRigidBodyCollision(true);
        ThirdPersonMesh->SetAllBodiesNotifyRigidBodyCollision(true);
}
