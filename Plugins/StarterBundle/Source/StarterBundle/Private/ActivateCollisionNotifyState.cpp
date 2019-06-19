// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivateCollisionNotifyState.h"
#include "Components/SkeletalMeshComponent.h"

UActivateCollisionNotifyState::UActivateCollisionNotifyState()
	: CollisionPart(ECollisionPart::PrimaryItem)
{
	
}

void UActivateCollisionNotifyState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();

		if (Owner)
		{
			UCollisionHandlerComponent* CollisionHandlerComponent = Cast<UCollisionHandlerComponent>(Owner->GetComponentByClass(UCollisionHandlerComponent::StaticClass()));

			if (CollisionHandlerComponent)
			{
				CollisionHandlerComponent->ActivateCollision(CollisionPart);
			}
		}
	}
}

void UActivateCollisionNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();

		if (Owner)
		{
			UCollisionHandlerComponent* CollisionHandlerComponent = Cast<UCollisionHandlerComponent>(Owner->GetComponentByClass(UCollisionHandlerComponent::StaticClass()));
			
			if (CollisionHandlerComponent)
			{
				CollisionHandlerComponent->DeactivateCollision();
			}
		}
	}
}
