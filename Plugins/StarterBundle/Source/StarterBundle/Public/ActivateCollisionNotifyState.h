// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CollisionHandlerComponent.h"
#include "ActivateCollisionNotifyState.generated.h"


/**
 * 
 */
UCLASS()
class STARTERBUNDLE_API UActivateCollisionNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	// constructor
	UActivateCollisionNotifyState();
	
	// overriden functions
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	ECollisionPart CollisionPart;

};
