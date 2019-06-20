// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateOwnerAnimNotify.h"
#include "RotatingComponent.h"
#include "Components/SkeletalMeshComponent.h"

URotateOwnerAnimNotify::URotateOwnerAnimNotify()
	: DegreesPerSecond(540.f), MaxPossibleRotation(180.f)
{

}

void URotateOwnerAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();

		if (Owner)
		{
			URotatingComponent* RotatingComponent = Cast<URotatingComponent>(Owner->GetComponentByClass(URotatingComponent::StaticClass()));

			if (RotatingComponent)
			{
				RotatingComponent->StartRotatingWithLimit(MaxPossibleRotation, DegreesPerSecond);
			}
		}
	}
}
