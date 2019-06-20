// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotatingComponent.generated.h"

/* Delegate called when rotating has started */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotatingStart);
DECLARE_MULTICAST_DELEGATE(FOnRotatingStartNative);

/* Delegate called when rotating has ended */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotatingEnd);
DECLARE_MULTICAST_DELEGATE(FOnRotatingEndNative);

/**
 * Component which allows to rotate character towards desired rotation which is defined in owning actor throught interface
 * Example of use: Rotate character towards input direction while it's playing attack anim montage with enabled root motion
 * Note! Owner of the component must implement RotatingComponentInterface
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STARTERBUNDLE_API URotatingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotatingComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "RotatingComponent")
	bool IsRotating() const;

	/* Delegate called when rotating has started */
	UPROPERTY(BlueprintAssignable, Category = "RotatingComponent")
	FOnRotatingStart OnRotatingStart;
	/* Native version above, called before BP delegate */
	FOnRotatingStartNative OnRotatingStartNative;

	/* Delegate called when rotating has ended */
	UPROPERTY(BlueprintAssignable, Category = "RotatingComponent")
	FOnRotatingEnd OnRotatingEnd;
	/* Native version above, called before BP delegate */
	FOnRotatingEndNative OnRotatingEndNative;

	/* Starts rotating owner for given time with given rotating speed */
	UFUNCTION(BlueprintCallable, Category = "RotatingComponent")
	void StartRotating(float time, float degressPerSecond);

	/**
	 * Starts rotating owner where time is calculated based on maxPossibleRotation/degressPerSecond, with given rotating speed
	 * Example: maxPossibleRotation=180, degressPerSecond=360 -> rotation will be activated for 0.5 sec
	 */ 
	UFUNCTION(BlueprintCallable, Category = "RotatingComponent")
	void StartRotatingWithLimit(float maxPossibleRotation, float degressPerSecond);

	UFUNCTION(BlueprintCallable, Category = "RotatingComponent")
	void StopRotating();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/* Calls the rotating callbacks */
	void NotifyOnRotatingStart();
	void NotifyOnRotatingEnd();

	/* Limit of degrees to rotate per second */
	float DegreesPerSecond;

	/* How long rotating should be activated */
	float RotatingTime;

private:

	/* Whether rotating is activated or not */
	uint32 bIsRotating : 1;

	/* Time elapsed since rotating was activated */
	float TimeElapsed;
};
