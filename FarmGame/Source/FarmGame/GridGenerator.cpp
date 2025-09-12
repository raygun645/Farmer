// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGenerator.h"

// Called when the game starts or when spawned
void AGridGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateGridWithBorder();
	
}

void AGridGenerator::GenerateGridWithBorder()
{
	//Grid Generation
	for (int32 RowIndex = GridRows * -1; RowIndex <= GridRows; RowIndex++)
	{
		for (int32 ColumnIndex = GridCols * -1; ColumnIndex <= GridCols; ColumnIndex++)
		{
			float LocationX = ColumnIndex * TileSize;
			float LocationY = RowIndex * TileSize;

			const FVector Location(LocationX, LocationY, 0);
			const FRotator Rotation(0,0,90.f);

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = this;
			SpawnParameters.Instigator = GetInstigator();
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (GridRows == RowIndex || GridRows * -1 == RowIndex  || GridCols == ColumnIndex || GridCols * -1 == ColumnIndex)
			{
				GetWorld()->SpawnActor<AActor>(BorderActorClass, Location, Rotation, SpawnParameters);
			}
			else
			{
				GetWorld()->SpawnActor<AActor>(SoilActorClass, Location, Rotation, SpawnParameters);
			}
		}
	}
}




