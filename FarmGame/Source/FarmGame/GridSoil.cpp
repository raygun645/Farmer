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
		IsGroundHoed = true;
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
	if (IsGroundHoed == true && IsGroundWatered == true)
	{
		UE_LOG(LogTemp, Display, TEXT("Seed Planted"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Seed no planted"));
	}
	return false;
}

void AGridSoil::Timeskip()
{
	UE_LOG(LogTemp, Display, TEXT("Timeskip"));
}

void AGridSoil::UpdateSoilSprite()
{
	UPaperSprite* SoilUpdate = SoilSpriteStages[SoilIndex];
	if (SoilUpdate)
	{
		GroundSprite->SetSprite(SoilUpdate);
	}
}
