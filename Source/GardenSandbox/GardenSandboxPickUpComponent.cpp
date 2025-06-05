// Copyright Epic Games, Inc. All Rights Reserved.

#include "GardenSandboxPickUpComponent.h"
#include "ResourceComponent.h"
#include "GardenSandboxCharacter.h"


UGardenSandboxPickUpComponent::UGardenSandboxPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UGardenSandboxPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UGardenSandboxPickUpComponent::OnSphereBeginOverlap);
}

void UGardenSandboxPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
        // Checking if it is a First Person Character overlapping
        AGardenSandboxCharacter* Character = Cast<AGardenSandboxCharacter>(OtherActor);
        if(Character != nullptr)
        {
                if (Character->ResourceComponent)
                {
                        Character->ResourceComponent->AddResource(ResourceType, ResourceAmount);
                }

                // Notify listeners that this pickup has been collected
                OnPickUp.Broadcast();

                // Unregister from the Overlap Event so it is no longer triggered
                OnComponentBeginOverlap.RemoveAll(this);
        }
}
