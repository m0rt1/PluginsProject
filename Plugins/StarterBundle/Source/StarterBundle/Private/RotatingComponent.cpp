// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingComponent.h"
#include "RotatingComponentInterface.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
URotatingComponent::URotatingComponent()
	: DegreesPerSecond(540.f), bIsRotating(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void URotatingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URotatingComponent::NotifyOnRotatingStart()
{
	OnRotatingStartNative.Broadcast();
	OnRotatingStart.Broadcast();
}

void URotatingComponent::NotifyOnRotatingEnd()
{
	OnRotatingEndNative.Broadcast();
	OnRotatingEnd.Broadcast();
}


void URotatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsRotating)
	{
		TimeElapsed += DeltaTime;
		// check if elapsed time doesn't exceed RotatingTime
		if (TimeElapsed <= RotatingTime)
		{
			// make sure owner implements interface IRotatingComponent and then retrieve desired rotation 
			AActor* Owner = GetOwner();
			if (Owner)
			{
				// Blueprints Interface check version
				if (Owner->GetClass()->ImplementsInterface(URotatingComponentInterface::StaticClass()))
				{
					FRotator DesiredRotation = IRotatingComponentInterface::Execute_GetDesiredRotation(Owner);
					FRotator CurrentRotation = Owner->GetActorRotation();
					FRotator NewRotation = UKismetMathLibrary::RInterpTo_Constant(CurrentRotation, DesiredRotation, DeltaTime, DegreesPerSecond);
					Owner->SetActorRotation(NewRotation);
				}

				 // C++ Interface check version
				/*IRotatingComponentInterface* RCI = Cast<IRotatingComponentInterface>(Owner);
				if (RCI)
				{
					FRotator DesiredRotation = RCI->Execute_GetDesiredRotation(Owner);
					FRotator CurrentRotation = Owner->GetActorRotation();
					FRotator NewRotation = UKismetMathLibrary::RInterpTo_Constant(CurrentRotation, DesiredRotation, DeltaTime, DegreesPerSecond);
					Owner->SetActorRotation(NewRotation);
				}*/
			}
		}
		else
		{
			StopRotating();
		}
	}
}

bool URotatingComponent::IsRotating() const
{
	return bIsRotating;
}

void URotatingComponent::StartRotating(float time, float degressPerSecond)
{
	RotatingTime = time;
	DegreesPerSecond = degressPerSecond;
	TimeElapsed = 0.f;
	bIsRotating = true;
	NotifyOnRotatingStart();
}

void URotatingComponent::StartRotatingWithLimit(float maxPossibleRotation, float degressPerSecond)
{
	// calculate time and call StartRotating with it
	float time = maxPossibleRotation / degressPerSecond;
	StartRotating(time, degressPerSecond);
}

void URotatingComponent::StopRotating()
{
	bIsRotating = false;
	NotifyOnRotatingEnd();
}
