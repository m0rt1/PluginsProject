// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateOwnerAnimNotifyState.h"
#include "RotatingComponent.h"
#include "Components/SkeletalMeshComponent.h"

URotateOwnerAnimNotifyState::URotateOwnerAnimNotifyState()
	: DegreesPerSecond(540.f)
{

}

void URotateOwnerAnimNotifyState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();

		if (Owner)
		{
			URotatingComponent* RotatingComponent = Cast<URotatingComponent>(Owner->GetComponentByClass(URotatingComponent::StaticClass()));

			if (RotatingComponent)
			{
				RotatingComponent->StartRotating(10.f, DegreesPerSecond);
			}
		}
	}
}

void URotateOwnerAnimNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();

		if (Owner)
		{
			URotatingComponent* RotatingComponent = Cast<URotatingComponent>(Owner->GetComponentByClass(URotatingComponent::StaticClass()));

			if (RotatingComponent)
			{
				RotatingComponent->StopRotating();
			}
		}
	}
}
