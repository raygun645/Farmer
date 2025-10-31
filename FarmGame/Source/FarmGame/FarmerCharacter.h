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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpliceDrawerOpen, bool, IsOpen);

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SpliceDrawerAction;

	void ToolTip();
	void CropDrawer();
	void SpliceDrawer();

	bool IsDrawerOpen = false;
	bool IsTipTextVisible = false;
	bool IsSpliceDrawerOpen = false;

	UPROPERTY(BlueprintAssignable)
	FOnToolTipTextOpen OnToolTipTextOpen;
	UPROPERTY(BlueprintAssignable)
	FOnCropDrawerOpen OnCropDrawerOpen;
	UPROPERTY(BlueprintAssignable)
	FOnSpliceDrawerOpen OnSpliceDrawerOpen;
	
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

    //SFX for actions
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sounds")
	USoundBase* HoeSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sounds")
	USoundBase* WaterSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sounds")
	USoundBase* PlantSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sounds")
	USoundBase* CoinsSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sounds")
	USoundBase* SwitchSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sounds")
	USoundBase* ErrorSound;

	//UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Sounds")
	//TArray<USoundBase*> WaterSounds;

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
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Money = 100;

	UPROPERTY(BlueprintAssignable)
	FOnMoneyChanged OnMoneyChanged;

	//Shared seed settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SharedSeedSettings)
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
	//Base turnip
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip|Unspliced")
	TArray<UPaperSprite*> TurnipCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip|Unspliced")
	UPaperSprite* TurnipCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip|Unspliced")
	int32 TurnipValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip|Unspliced")
	int32 TurnipSeedCost;

	//Turnip upgrade stats
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip|Upgrades")
	bool IsTurnipSpliced;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip|Upgrades")
	int32 TurnipSpliceCost; // = Base value * 10

	//Spliced turnip. Spliced values = Base * 2
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip|Spliced")
	TArray<UPaperSprite*> SplicedTurnipCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip|Spliced")
	UPaperSprite* SplicedTurnipCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip|Spliced")
	int32 SplicedTurnipValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Turnip|Spliced")
	int32 SplicedTurnipSeedCost;
	
	//Base rose
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose|Unspliced")
	TArray<UPaperSprite*> RoseCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose|Unspliced")
	UPaperSprite* RoseCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose|Unspliced")
	int32 RoseValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose|Unspliced")
	int32 RoseSeedCost;

	//Rose upgrade stats
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose|Upgrades")
	bool IsRoseSpliced;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose|Upgrades")
	int32 RoseSpliceCost;

	//Spliced rose
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose|Spliced")
	TArray<UPaperSprite*> SplicedRoseCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose|Spliced")
	UPaperSprite* SplicedRoseCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose|Spliced")
	int32 SplicedRoseValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Rose|Spliced")
	int32 SplicedRoseSeedCost;
	
	//Base strawberry
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry|Unspliced")
	TArray<UPaperSprite*> StrawberryCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry|Unspliced")
	UPaperSprite* StrawberryCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry|Unspliced")
	int32 StrawberryValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry|Unspliced")
	int32 StrawberrySeedCost;

	//Strawberry upgrade stats
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry|Upgrades")
	bool IsStrawberrySpliced;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry|Upgrades")
	int32 StrawberrySpliceCost;

	//Spliced strawberry
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry|Spliced")
	TArray<UPaperSprite*> SplicedStrawberryCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry|Spliced")
	UPaperSprite* SplicedStrawberryCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry|Spliced")
	int32 SplicedStrawberryValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Strawberry|Spliced")
	int32 SplicedStrawberrySeedCost;
	
	//Base grape
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape|Unspliced")
	TArray<UPaperSprite*> GrapeCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape|Unspliced")
	UPaperSprite* GrapeCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape|Unspliced")
	int32 GrapeValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape|Unspliced")
	int32 GrapeSeedCost;

	//Grape upgrade stats
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape|Upgrades")
	bool IsGrapeSpliced;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape|Upgrades")
	int32 GrapeSpliceCost;

	//Spliced grape
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape|Spliced")
	TArray<UPaperSprite*> SplicedGrapeCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape|Spliced")
	UPaperSprite* SplicedGrapeCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape|Spliced")
	int32 SplicedGrapeValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Grape|Spliced")
	int32 SplicedGrapeSeedCost;
	
	//Base pineapple
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple|Unspliced")
	TArray<UPaperSprite*> PineappleCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple|Unspliced")
	UPaperSprite* PineappleCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple|Unspliced")
	int32 PineappleValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple|Unspliced")
	int32 PineappleSeedCost;

	//Pineapple upgrade stats
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple|Upgrades")
	bool IsPineappleSpliced;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple|Upgrades")
	int32 PineappleSpliceCost;

	//Spliced pineapple
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple|Spliced")
	TArray<UPaperSprite*> SplicedPineappleCropSprites;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple|Spliced")
	UPaperSprite* SplicedPineappleCropImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple|Spliced")
	int32 SplicedPineappleValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Seeds|Pineapple|Spliced")
	int32 SplicedPineappleSeedCost;
	
};
