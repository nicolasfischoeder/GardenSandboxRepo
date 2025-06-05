// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GardenSandboxCharacter.h"
#include "ResourceComponent.h"
#include "GardenSandboxPickUpComponent.generated.h"

// Declaration of the delegate that will be called when someone picks this up
// The character picking this up is the parameter sent with the notification
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AGardenSandboxCharacter*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GARDENSANDBOX_API UGardenSandboxPickUpComponent : public USphereComponent
{
        GENERATED_BODY()

public:
	
	/** Delegate to whom anyone can subscribe to receive this event */
        UPROPERTY(BlueprintAssignable, Category = "Interaction")
        FOnPickUp OnPickUp;

        /** Type of resource to give when picked up */
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Resources")
        EResourceType ResourceType = EResourceType::Wood;

        /** Amount of the resource to give */
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Resources")
        int32 ResourceAmount = 1;

        UGardenSandboxPickUpComponent();
protected:

	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Code for when something overlaps this component */
	UFUNCTION()
void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
