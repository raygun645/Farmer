// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "UObject/Interface.h"

#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FARMGAME_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void Hoe() = 0;

	virtual bool Plant(/*const TArray<UPaperSpriteComponent*> PlantedSprites, int32 ValueOfCrop*/) = 0;

	virtual void Water() = 0;
	
	virtual void Timeskip() = 0;

	
};
