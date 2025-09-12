// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridGenerator.generated.h"

UCLASS()
class FARMGAME_API AGridGenerator : public AActor
{
	GENERATED_BODY()
	
public:	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	TSubclassOf<AActor> SoilActorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Settings")
	TSubclassOf<AActor> BorderActorClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid Settings")
	int32 GridRows;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid Settings")
	int32 GridCols;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid Settings")
	int32 TileSize = 40; //direct tile size from sprite
private:
	void GenerateGridWithBorder();
	
};
