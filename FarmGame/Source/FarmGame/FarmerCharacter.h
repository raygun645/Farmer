// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridSoil.h"
#include "PaperCharacter.h"
#include "FarmerCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, UpdatedMoney);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCropChanged, UPaperSprite*, UpdatedCropSprite, int32, CropCost, int32, CropValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToolTipTextOpen, bool, IsVisible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCropDrawerOpen, bool, IsOpen);

UCLASS()
class FARMGAME_API AFarmerCharacter : public APaperCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerMappingContext;

	//input actions
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
	//Player UI and inputs
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> PlayerUIWidgetClass;
	UPROPERTY()
	UUserWidget* PlayerUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToolTipAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CropDrawerAction;

	void ToolTip();
	void CropDrawer();

	bool IsDrawerOpen = false;
	bool IsTipTextVisible = false;

	UPROPERTY(BlueprintAssignable)
	FOnToolTipTextOpen OnToolTipTextOpen;
	UPROPERTY(BlueprintAssignable)
	FOnCropDrawerOpen OnCropDrawerOpen;
	
	
	
	//pause menu and input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	void PauseGame();

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> PauseWidgetClass;
	UPROPERTY()
	UUserWidget* PauseWidget;

	//Player actions and input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HoeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* WaterAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PlantAction;

	void Hoe();
	void Water();
	void Plant();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sounds")
	USoundBase* HoeSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sounds")
	USoundBase* WaterSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sounds")
	USoundBase* PlantSound;

	//seed swapping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SeedSlot1Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SeedSlot2Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SeedSlot3Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SeedSlot4Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SeedSlot5Action;

	void SwapToTurnipSeed();
	void SwapToRoseSeed();
	void SwapToStrawberrySeed();
	void SwapToGrapeSeed();
	void SwapToPineappleSeed();

	//soil filter for overlaps
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Overlap)
	TSubclassOf<AGridSoil> SoilClassFilter;

	//Money
	int32 Money = 100;

	UPROPERTY(BlueprintAssignable)
	FOnMoneyChanged OnMoneyChanged;

	//Shared seed settings
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SharedSeedSettings)
	TArray<UPaperSprite*> CurrentCropSprites;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SharedSeedSettings)
	UPaperSprite* CurrentCropImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SharedSeedSettings)
	int32 CropValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SharedSeedSettings)
	int32 CropCost;

	UPROPERTY(BlueprintAssignable)
	FOnCropChanged OnCropChanged;

	//Specific seeds
	//Turnip
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip")
	TArray<UPaperSprite*> TurnipCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip")
	UPaperSprite* TurnipCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip")
	int32 TurnipValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip")
	int32 TurnipSeedCost;

	//Rose
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose")
	TArray<UPaperSprite*> RoseCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose")
	UPaperSprite* RoseCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose")
	int32 RoseValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose")
	int32 RoseSeedCost;

	//Strawberry
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry")
	TArray<UPaperSprite*> StrawberryCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry")
	UPaperSprite* StrawberryCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry")
	int32 StrawberryValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry")
	int32 StrawberrySeedCost;

	//Grape
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape")
	TArray<UPaperSprite*> GrapeCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape")
	UPaperSprite* GrapeCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape")
	int32 GrapeValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape")
	int32 GrapeSeedCost;

	//Pineapple
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple")
	TArray<UPaperSprite*> PineappleCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple")
	UPaperSprite* PineappleCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple")
	int32 PineappleValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple")
	int32 PineappleSeedCost;
	
	
};
