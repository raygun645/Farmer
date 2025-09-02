// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "FarmerCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class FARMGAME_API AFarmerCharacter : public APaperCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

public:
	AFarmerCharacter();

	virtual void BeginPlay() override;

protected:
	void Move(const FInputActionValue& Value);

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

//private:
	//Player UI
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> PlayerUIWidgetClass;
	UPROPERTY()
	UUserWidget* PlayerUIWidget;
	
	//pause menu and input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	void PauseGame();

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> PauseWidgetClass;
	UPROPERTY()
	UUserWidget* PauseWidget;
	
	
};
