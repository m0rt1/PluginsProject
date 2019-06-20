// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RotateOwnerAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class STARTERBUNDLE_API URotateOwnerAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	// constructor
	URotateOwnerAnimNotifyState();

	// overriden functions
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	/* Limit of degrees to rotate per second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotating")
	float DegreesPerSecond;
};
