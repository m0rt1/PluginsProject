// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionHandlerComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UCollisionHandlerComponent::UCollisionHandlerComponent()
	: TraceRadius(0.1f),
	TraceCheckInterval(0.025f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// adds Pawn value to objects to collide with
	ObjectTypesToCollideWith.Add(EObjectTypeQuery::ObjectTypeQuery3);
}

// Called when the game starts
void UCollisionHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCollisionHandlerComponent::NotifyOnHit(const FHitResult& HitResult)
{
	// Notify native before blueprint
	OnHitNative.Broadcast(HitResult);
	OnHit.Broadcast(HitResult);
}

void UCollisionHandlerComponent::NotifyOnCollisionActivated(ECollisionPart CollisionPart)
{
	// Notify native before blueprint
	OnCollisionActivatedtNative.Broadcast(CollisionPart);
	OnCollisionActivated.Broadcast(CollisionPart);
}

void UCollisionHandlerComponent::NotifyOnCollisionDeactivated()
{
	// Notify native before blueprint
	OnCollisionDeactivatedNative.Broadcast();
	OnCollisionDeactivated.Broadcast();
}

void UCollisionHandlerComponent::UpdateSocketLocations()
{
	if (CollidingComponent)
	{
		// update location of sockets and store them in map
		for (const FName& socketName : CollisionSockets)
		{
			FVector socketLocation = CollidingComponent->GetSocketLocation(socketName);
			LastFrameSocketLocations.Add(socketName, socketLocation);
		}
	}
}

void UCollisionHandlerComponent::PerformTraceCheck()
{
	if (CollidingComponent)
	{
		// add owner to hit actors to make sure it will be ignored
		HitActors.Add(GetOwner());

		for (const FName& socketName : CollisionSockets)
		{
			FVector StartTrace = *LastFrameSocketLocations.Find(socketName);
			FVector EndTrace = CollidingComponent->GetSocketLocation(socketName);
			EDrawDebugTrace::Type DebugTraceType = (EDrawDebugTrace::Type)DebugMode;
			TArray<FHitResult> HitResults;

			bool WasHit =  UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartTrace, EndTrace, TraceRadius, ObjectTypesToCollideWith,
				bTraceComplex, HitActors, DebugTraceType, HitResults, true);

			if (WasHit)
			{
				for (const FHitResult& HitResult : HitResults)
				{
					if (HitActors.Contains(HitResult.GetActor()) == false &&
						IsIgnoredClass(HitResult.GetActor()->GetClass()) == false &&
						IsIgnoredProfileName(HitResult.Component->GetCollisionProfileName()) == false)
					{
						HitActors.Add(HitResult.GetActor());
						NotifyOnHit(HitResult);
					}
				}
			}
		}
	}
}

bool UCollisionHandlerComponent::IsIgnoredClass(TSubclassOf<AActor> ActorClass)
{
	// if actor class is child or same class of any of ignored classes, return true, otherwise false
	for (const auto& IgnoredClass : IgnoredClasses)
	{
		if (ActorClass->IsChildOf(IgnoredClass))
			return true;
	}
	return false;
}

bool UCollisionHandlerComponent::IsIgnoredProfileName(FName ProfileName)
{
	return IgnoredCollisionProfileNames.Contains(ProfileName);
}

void UCollisionHandlerComponent::TraceCheckLoop()
{
	// on first tick just update socket locations so on next tick it will be able to compare socket locations
	if (bCanPerformTrace)
	{
		PerformTraceCheck();
	}

	UpdateSocketLocations();
	bCanPerformTrace = true;

}

void UCollisionHandlerComponent::UpdateCollidingComponentAndSockets(UPrimitiveComponent* Component, const TArray<FName>& Sockets)
{
	CollidingComponent = Component;
	CollisionSockets = Sockets;
}

void UCollisionHandlerComponent::ActivateCollision(ECollisionPart CollisionPart)
{
	bIsCollisionActivated = true;
	HitActors.Empty();

	// set timer which will check for collisions
	GetWorld()->GetTimerManager().SetTimer(TraceCheckTimerHandle, this, &UCollisionHandlerComponent::TraceCheckLoop, TraceCheckInterval, true);
	
	// call OnCollisionActivated delegates
	NotifyOnCollisionActivated(CollisionPart);
}

void UCollisionHandlerComponent::DeactivateCollision()
{
	bIsCollisionActivated = false;
	bCanPerformTrace = false;
	
	// clear timer checking for collision
	GetWorld()->GetTimerManager().ClearTimer(TraceCheckTimerHandle);

	// call OnCollisionDeactivated delegates
	NotifyOnCollisionDeactivated();
}

