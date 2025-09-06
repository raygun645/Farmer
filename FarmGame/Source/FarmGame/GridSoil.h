// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "PaperSpriteActor.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "GridSoil.generated.h"

/**
 * 
 */
UCLASS()
class FARMGAME_API AGridSoil : public APaperSpriteActor, public IInteractionInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (allowPrivateAccess = true))
	UBoxComponent* BoxCollider;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (allowPrivateAccess = true))
	UPaperSpriteComponent* CropSprite;
	
public:

	AGridSoil();
	
	//interface events
	UFUNCTION(Category = "Interaction")
	virtual void Hoe() override;
	UFUNCTION(Category = "Interaction")
	virtual void Water() override;
	UFUNCTION(Category = "Interaction")
	virtual bool Plant(const TArray<UPaperSpriteComponent*> PlantedSprites, int32 ValueOfCrop) override;
	UFUNCTION(Category = "Interaction")
	virtual void Timeskip() override;
};
