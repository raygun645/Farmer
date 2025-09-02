// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridSoil.h"
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
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
	void Hoe();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
	bool Plant(const TArray<UPaperSpriteComponent*>& PlantedSprites, int32 ValueOfCrop);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
	void Water();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
	void TimeSkip();
};
