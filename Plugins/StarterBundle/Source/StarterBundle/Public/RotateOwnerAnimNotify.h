// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RotateOwnerAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class STARTERBUNDLE_API URotateOwnerAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	/* constructor */
	URotateOwnerAnimNotify();

	/* overridden method */
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	/* Limit of degrees to rotate per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotating")
	float DegreesPerSecond;

	/**
	 * Calculates rotating time based on that value
	 * Example: MaxPossibleRotation=180, DegressPerSecond=360 -> rotation will be activated for 0.5 sec
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotating")
	float MaxPossibleRotation;
};
