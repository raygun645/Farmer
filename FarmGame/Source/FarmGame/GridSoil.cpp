// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSoil.h"


AGridSoil::AGridSoil()
{
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	CropSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("CropSprite"));
	GroundSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GroundSprite"));
	GroundSprite->SetupAttachment(RootComponent);
	CropSprite->SetupAttachment(GroundSprite);
	BoxCollider->SetupAttachment(CropSprite);
}

void AGridSoil::Hoe()
{
	UE_LOG(LogTemp, Display, TEXT("Hoe (Soil Side)"));
	
	if (GroundSprite)
	{
		SoilIndex = 1;
		UpdateSoilSprite();
		CropSprite->SetSprite(nullptr); // clear the seed if hoed
		
		IsGroundHoed = true;
		IsGroundWatered = false;
		IsCropPlanted = false;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ISSUE CHANGING SPRITE"));
	}
}

void AGridSoil::Water()
{
	UE_LOG(LogTemp, Display, TEXT("Water (Soil Side)"));

	if (IsGroundHoed == true)
	{
		if (GroundSprite)
		{
			SoilIndex = 2;
			UpdateSoilSprite();
			IsGroundWatered = true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ISSUE CHANGING SPRITE"));
		}
	}
	
}

bool AGridSoil::Plant(/*const TArray<UPaperSpriteComponent*> PlantedSprites, int32 ValueOfCrop*/)
{
	UE_LOG(LogTemp, Display, TEXT("Plant (Soil Side)"));
	if (IsGroundHoed == true && IsCropPlanted == false)
	{
		UE_LOG(LogTemp, Display, TEXT("Seed Planted"));
		CropIndex = 0;
		IsCropPlanted = true;
		UpdateCropSprite();
		return true;
	}
	
	UE_LOG(LogTemp, Display, TEXT("Seed no planted"));
	return false;
	
}

void AGridSoil::Timeskip()
{
	UE_LOG(LogTemp, Display, TEXT("Timeskip"));
	
	if (IsCropPlanted == true)
	{
		if (IsGroundWatered == true)
		{
			CropIndex++;
			SoilIndex = 1;
			IsGroundWatered = false;
			UpdateCropSprite();
		}
	}
	else
	{
		SoilIndex = 0;
		IsGroundHoed = false;
	}

	UpdateSoilSprite();
	IsGroundWatered = false;
}

void AGridSoil::UpdateSoilSprite()
{
	UPaperSprite* SoilUpdate = SoilSpriteStages[SoilIndex];
	if (SoilUpdate)
	{
		GroundSprite->SetSprite(SoilUpdate);
	}
}

void AGridSoil::UpdateCropSprite()
{
	//gets last valid index of array. then clamps the index
	const int32 LastIndex = PlantedCropSpriteStages.Num() > 0 ? PlantedCropSpriteStages.Num() - 1 : 0;
	const int32 ClampedIndex = FMath::Clamp(CropIndex, 0, LastIndex);
	
	if (PlantedCropSpriteStages.IsValidIndex(ClampedIndex))
	{
		UPaperSprite* CropUpdate = PlantedCropSpriteStages[ClampedIndex];
		if (CropUpdate)
		{
			CropSprite->SetSprite(CropUpdate);
		}
	}

	if (ClampedIndex == LastIndex)
	{
		IsPlantReady = true;
	}
}
