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

	//In-Game actor components
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (allowPrivateAccess = true))
	UBoxComponent* BoxCollider;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (allowPrivateAccess = true))
	UPaperSpriteComponent* CropSprite;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (allowPrivateAccess = true))
	UPaperSpriteComponent* GroundSprite;
	
public:

	AGridSoil();
	
	//interface events
	UFUNCTION(Category = "Interaction")
	virtual void Hoe() override;
	UFUNCTION(Category = "Interaction")
	virtual void Water() override;
	UFUNCTION(Category = "Interaction")
	virtual bool Plant(const TArray<UPaperSprite*> PlantedSprites, int32 ValueOfCrop) override;
	UFUNCTION(BlueprintCallable,Category = "Interaction")
	virtual void Timeskip() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UPaperSprite*> PlantedCropSpriteStages;
	int32 PlantedCropValue;

	//Ground States
	bool IsGroundHoed = false;
	bool IsGroundWatered = false;
	bool IsCropPlanted = false;
	bool IsPlantReady = false;

protected:

	//In-Game soil states
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UPaperSprite*> SoilSpriteStages;
	int32 SoilIndex; // 0 = grass/1 = tilled/2 = watered 
	
	UFUNCTION(Category = "Crop/Soil")
	void UpdateSoilSprite();

	//Updating the crop and index
	UFUNCTION(Category = "Crop/Soil")
	void UpdateCropSprite();
	int32 CropIndex;
};
