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
	
}

void AGridSoil::Water()
{
	
}

bool AGridSoil::Plant(const TArray<UPaperSpriteComponent*> PlantedSprites, int32 ValueOfCrop)
{
	
	return false;
}
