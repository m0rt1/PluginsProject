// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RotatingComponentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URotatingComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STARTERBUNDLE_API IRotatingComponentInterface
{
	GENERATED_BODY()

public:
	/* Returns rotation that owner of the RotatingComponant should be rotated at while component is activated */
	UFUNCTION(BlueprintNativeEvent, Category = "RotatingComponent")
		FRotator GetDesiredRotation();
};
