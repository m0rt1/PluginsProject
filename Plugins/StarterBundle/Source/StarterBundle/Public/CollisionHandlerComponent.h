// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionHandlerComponent.generated.h"


/**
 * Enum which helps to determine on which part of body or weapon collision should be activated.
 * Example: When collision is activated, switch colliding component and its sockets based on ECollisionPart value,
 * if it is PrimaryItem, check collision on sword, if it is LeftArm check collision on left arm etc.
 */
UENUM(BlueprintType)
enum class ECollisionPart : uint8
{
	NONE,
	PrimaryItem,
	SecondaryItem,
	LeftArm,
	RightArm,
	LeftLeg,
	RightLeg,
	Custom1,
	Custom2,
	Custom3
};

/**
 * Custom debug mode enum to avoid including Kismet System Library header.
 * In cpp file while doing trace check it will be converted to EDrawDebugTrace::Type
 */
UENUM(BlueprintType)
enum class ECollisionHandlerDebugMode : uint8
{
	None,
	ForOneFrame,
	ForDuration,
	Persistant
};

/* Delegate called when there was a collision */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, const FHitResult&, HitResult);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitNative, FHitResult);

/* Delegate called when collision was activated */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollisionActivated, ECollisionPart, CollisionPart);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCollisionActivatedNative, ECollisionPart);

/* Delegate called when collision was deactivated */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollisionDeactivated);
DECLARE_MULTICAST_DELEGATE(FOnCollisionDeactivatedNative);

/**
 * Component which allows to setup in a simple way accurate collision checks.
 * It works on the principle of checking whether there is an object between the socket position in this and the previous frame.
 * Example: Sockets attached to hand of character mesh, Sockets attached to static mesh sword
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STARTERBUNDLE_API UCollisionHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/* Constructor */
	UCollisionHandlerComponent();

	/* Whether to use trace complex option during sphere trace or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandler")
	uint32 bTraceComplex : 1;

	/* Radius of sphere trace checking collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandler")
	float TraceRadius;

	/* How often trace check is done while collision is activated */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandler")
	float TraceCheckInterval;

	/* Classes that will be ignored while checking collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandler")
	TArray<TSubclassOf<AActor>> IgnoredClasses;

	/* Profile names that components will be ignored with, may be useful to ignore collision capsule (Pawn) and collide only with its character mesh (CharacterMesh) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandler")
	TArray<FName> IgnoredCollisionProfileNames;

	/* Types of objects to collide with - Pawn, WordStatic etc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandler")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToCollideWith;

	/* Determines debug mode: None/ForDuration/ForOneFrame etc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionHandler")
	ECollisionHandlerDebugMode DebugMode;

	/* Delegate called when there was a collision */
	UPROPERTY(BlueprintAssignable, Category = "CollisionHandler")
	FOnHit OnHit;
	/* Native version above, called before BP delegate */
	FOnHitNative OnHitNative;

	/* Delegate called when collision was activated */
	UPROPERTY(BlueprintAssignable, Category = "CollisionHandler")
	FOnCollisionActivated OnCollisionActivated;
	/* Native version above, called before BP delegate */
	FOnCollisionActivatedNative OnCollisionActivatedtNative;

	/* Delegate called when collision was deactivated */
	UPROPERTY(BlueprintAssignable, Category = "CollisionHandler")
	FOnCollisionDeactivated OnCollisionDeactivated;
	/* Native version above, called before BP delegate */
	FOnCollisionDeactivatedNative OnCollisionDeactivatedNative;

	/* Updates colliding component and sockets array that should be associated with it */
	UFUNCTION(BlueprintCallable, Category = "CollisionHandler")
	void UpdateCollidingComponentAndSockets(UPrimitiveComponent* Component, const TArray<FName>& Sockets);

	/* Activates collision, usually called by anim notify (ActivateCollisionNotifyState) on anim montage */
	UFUNCTION(BlueprintCallable, Category = "CollisionHandler")
	void ActivateCollision(ECollisionPart CollisionPart);

	/* Deactivates collision, usually called by anim notify (ActivateCollisionNotifyState) on anim montage */
	UFUNCTION(BlueprintCallable, Category = "CollisionHandler")
	void DeactivateCollision();

	UFUNCTION(BlueprintCallable, Category = "CollisionHandler")
	TArray<FName> GetCollisionSockets() const;

	UFUNCTION(BlueprintCallable, Category = "CollisionHandler")
	UPrimitiveComponent* GetCollidingComponent() const;

	UFUNCTION(BlueprintCallable, Category = "CollisionHandler")
	bool IsCollisionActivated() const;

	UFUNCTION(BlueprintCallable, Category = "CollisionHandler")
	ECollisionPart GetActivatedCollisionPart() const;

protected:
	/* Called when the game starts */
	virtual void BeginPlay() override;

	/* Tracks which collision part was recently activated */
	UPROPERTY(BlueprintReadOnly, Category = "CollisionHandler")
	ECollisionPart ActivatedCollisionPart;

	/* Component from which socket locations will be taken, may be Static Mesh sword, Character Mesh etc. */
	UPROPERTY(BlueprintReadOnly, Category = "CollisionHandler")
	UPrimitiveComponent* CollidingComponent;

	/* Array of sockets which locations will be taken from CollidinMesh to check for collision */
	UPROPERTY(BlueprintReadOnly, Category = "CollsionHandler")
	TArray<FName> CollisionSockets;

	/* Determines whether collision is activated */
	UPROPERTY(BlueprintReadOnly, Category = "CollisionHandler")
	uint32 bIsCollisionActivated : 1;

	/* Calls the collision handler callbacks */
	void NotifyOnHit(const FHitResult& HitResult);
	void NotifyOnCollisionActivated(ECollisionPart CollisionPart);
	void NotifyOnCollisionDeactivated();

	/* Updates values in map LastFrameSocketLocations */
	void UpdateSocketLocations();

	/* Does a sphere trace between socket locations in last and current frame and check whether 
	there is any colliding object between these locations */
	void PerformTraceCheck();
private:	
	/* Array storing temporary actors that were hit during single activation */
	UPROPERTY()
	TArray<AActor*> HitActors;

	/* Map storing location of socket in last frame */
	UPROPERTY()
	TMap<FName, FVector> LastFrameSocketLocations;

	/* Determines whether trace check can be performed, used to make sure it wont happen on first timer tick to firstly store socket locations */
	uint32 bCanPerformTrace : 1;

	/* Checks whether given class should be ignored or not */
	bool IsIgnoredClass(TSubclassOf<AActor> ActorClass);
	
	/* Checks whether given profile name is ignored or not*/
	bool IsIgnoredProfileName(FName ProfileName);

	/* function called on timer to perform trace check */
	UFUNCTION()
	void TraceCheckLoop();

	/* Handle for trace check loop timer */
	FTimerHandle TraceCheckTimerHandle;
};
