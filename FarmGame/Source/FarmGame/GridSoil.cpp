// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSoil.h"


AGridSoil::AGridSoil()
{
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	CropSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("CropSprite"));
	CropSprite->SetupAttachment(RootComponent);
	BoxCollider->SetupAttachment(CropSprite);
}

void AGridSoil::Hoe()
{
	UE_LOG(LogTemp, Display, TEXT("Hoe (Soil Side)"));
}

void AGridSoil::Water()
{
	UE_LOG(LogTemp, Display, TEXT("Water (Soil Side)"));
}

bool AGridSoil::Plant(/*const TArray<UPaperSpriteComponent*> PlantedSprites, int32 ValueOfCrop*/)
{
	UE_LOG(LogTemp, Display, TEXT("Plant (Soil Side)"));
	return false;
}

void AGridSoil::Timeskip()
{
	UE_LOG(LogTemp, Display, TEXT("Timeskip"));
}
