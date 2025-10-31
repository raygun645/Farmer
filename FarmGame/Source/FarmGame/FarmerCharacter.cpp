// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

/* TODO: (In any order)
 * Crop cost and balanced values (DONE)
 * Upgrades to crops/Progression for unlocking new crops (DONE)
 * New sound effects and UI tweaks (In-Progress)
 * Possibly other ways of farming? different tools?
 * Different way to timeskip outside of a button in the UI
 * Autonomous Farming?
 * Save functions for saving unlocks and progress
 * Find a way to have more seeds? maybe a catalogue you can select seeds to have in your toolbar.
 * Maybe a way to buy more space?
 * A Town transition rather than doing everything on the farm.
*/

AFarmerCharacter::AFarmerCharacter()
{
	
}

void AFarmerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//setup logic for player UI
	if (IsLocallyControlled() && PlayerUIWidgetClass)
	{
		PlayerUIWidget = CreateWidget(PlayerController, PlayerUIWidgetClass);
		if (PlayerUIWidget)
		{
			PlayerUIWidget->AddToViewport();
			PlayerController->SetShowMouseCursor(true);
		}
	}

	//set to turnip as default and send a change in case it doesn't show in the UI
	CurrentCropSprites = TurnipCropSprites;
	CropValue = TurnipValue;
	CurrentCropImage = TurnipCropImage;
	CropCost = TurnipSeedCost;

	OnCropChanged.Broadcast(CurrentCropImage, CropCost, CropValue);
}

void AFarmerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

void AFarmerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFarmerCharacter::Move);

		//player actions
		EnhancedInputComponent->BindAction(HoeAction, ETriggerEvent::Triggered, this, &AFarmerCharacter::Hoe);
		EnhancedInputComponent->BindAction(WaterAction, ETriggerEvent::Triggered, this, &AFarmerCharacter::Water);
		EnhancedInputComponent->BindAction(PlantAction, ETriggerEvent::Triggered, this, &AFarmerCharacter::Plant);
		//UI elements
		EnhancedInputComponent->BindAction(ToolTipAction, ETriggerEvent::Triggered, this, &AFarmerCharacter::ToolTip);
		EnhancedInputComponent->BindAction(CropDrawerAction, ETriggerEvent::Triggered, this, &AFarmerCharacter::CropDrawer);
		EnhancedInputComponent->BindAction(SpliceDrawerAction, ETriggerEvent::Triggered, this, &AFarmerCharacter::SpliceDrawer);
		//Swapping seeds
		EnhancedInputComponent->BindAction(SeedSlot1Action, ETriggerEvent::Triggered, this, &AFarmerCharacter::SwapToTurnipSeed);
		EnhancedInputComponent->BindAction(SeedSlot2Action, ETriggerEvent::Triggered, this, &AFarmerCharacter::SwapToRoseSeed);
		EnhancedInputComponent->BindAction(SeedSlot3Action, ETriggerEvent::Triggered, this, &AFarmerCharacter::SwapToStrawberrySeed);
		EnhancedInputComponent->BindAction(SeedSlot4Action, ETriggerEvent::Triggered, this, &AFarmerCharacter::SwapToGrapeSeed);
		EnhancedInputComponent->BindAction(SeedSlot5Action, ETriggerEvent::Triggered, this, &AFarmerCharacter::SwapToPineappleSeed);
		//pause
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AFarmerCharacter::PauseGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

//same rule for both, if not open then open and broadcast the change
void AFarmerCharacter::ToolTip()
{
	if (IsTipTextVisible == false)
	{
		IsTipTextVisible = true;
		OnToolTipTextOpen.Broadcast(true);
	}
	else
	{
		IsTipTextVisible = false;
		OnToolTipTextOpen.Broadcast(false);
	}
}

void AFarmerCharacter::CropDrawer()
{
	if (IsDrawerOpen == false)
	{
		IsDrawerOpen = true;
		OnCropDrawerOpen.Broadcast(true);
	}
	else
	{
		IsDrawerOpen = false;
		OnCropDrawerOpen.Broadcast(false);
	}
}

void AFarmerCharacter::SpliceDrawer()
{
	if (IsSpliceDrawerOpen == false)
	{
		IsSpliceDrawerOpen = true;
		OnSpliceDrawerOpen.Broadcast(true);
	}
	else
	{
		IsSpliceDrawerOpen = false;
		OnSpliceDrawerOpen.Broadcast(false);
	}
}

void AFarmerCharacter::PauseGame()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	UGameplayStatics::SetGamePaused(this,true);

	if (PauseWidgetClass)
	{
		PauseWidget = CreateWidget<UUserWidget>(PlayerController, PauseWidgetClass);
		if (PauseWidget)
		{
			PauseWidget->AddToViewport();

			FInputModeGameAndUI InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
}

void AFarmerCharacter::Hoe()
{
	UGameplayStatics::PlaySound2D(GetWorld(),HoeSound);
	
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, SoilClassFilter);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		AGridSoil* GridSoil = Cast<AGridSoil>(OverlappingActor);
		IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(OverlappingActor);
		
		if (OverlappingActor && InteractionInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Hoed (Player Side)"));
			InteractionInterface->Hoe();
			
			if (GridSoil->IsPlantReady == true)
			{
				UE_LOG(LogTemp, Display, TEXT("Money for plant"));
				UGameplayStatics::PlaySound2D(GetWorld(),CoinsSound);
				int32 Profit = Money += GridSoil->PlantedCropValue;
				OnMoneyChanged.Broadcast(Profit);
			}
			
		}
	}
}

void AFarmerCharacter::Water()
{
	//const int32 RandomIndex = FMath::RandRange(0, WaterSounds.Num() - 1);
	//UGameplayStatics::PlaySound2D(GetWorld(),WaterSounds[RandomIndex]);
	
	UGameplayStatics::PlaySound2D(GetWorld(),WaterSound);
	
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, SoilClassFilter);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(OverlappingActor);
		
		if (OverlappingActor)
		{
			UE_LOG(LogTemp, Display, TEXT("Watered (Player Side)"));
			InteractionInterface->Water();
		}
	}
}

void AFarmerCharacter::Plant()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, SoilClassFilter);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(OverlappingActor);
		
		if (OverlappingActor)
		{
			if (Money > CropCost)
			{
				UE_LOG(LogTemp, Display, TEXT("PlantedSeed (Player Side)"));
				bool PlantResult = InteractionInterface->Plant(CurrentCropSprites, CropValue);
				
				if (PlantResult == true)
				{
					UGameplayStatics::PlaySound2D(GetWorld(),PlantSound);
					
					Money -= CropCost;
					OnMoneyChanged.Broadcast(Money);
				}
				else
				{
					UGameplayStatics::PlaySound2D(GetWorld(),ErrorSound);
				}
			}
		}
	}
}

//here seeds will be assigned to a button press as well as their values
void AFarmerCharacter::SwapToTurnipSeed()
{
	UGameplayStatics::PlaySound2D(GetWorld(),SwitchSound);
	
	//If spliced use those values
	if (IsTurnipSpliced == true)
	{
		CurrentCropSprites = SplicedTurnipCropSprites;
		CropValue = SplicedTurnipValue;
		CurrentCropImage = SplicedTurnipCropImage;
		CropCost = SplicedTurnipSeedCost;

		OnCropChanged.Broadcast(CurrentCropImage, CropCost, CropValue);
	}
	else
	{
		CurrentCropSprites = TurnipCropSprites;
		CropValue = TurnipValue;
		CurrentCropImage = TurnipCropImage;
		CropCost = TurnipSeedCost;

		OnCropChanged.Broadcast(CurrentCropImage, CropCost, CropValue);
	}
	
}

void AFarmerCharacter::SwapToRoseSeed()
{
	UGameplayStatics::PlaySound2D(GetWorld(),SwitchSound);
	
	if (IsRoseSpliced == true)
	{
		CurrentCropSprites = SplicedRoseCropSprites;
		CropValue = SplicedRoseValue;
		CurrentCropImage = SplicedRoseCropImage;
		CropCost = SplicedRoseSeedCost;
	
		OnCropChanged.Broadcast(CurrentCropImage, CropCost, CropValue);
	}
	else
	{
		CurrentCropSprites = RoseCropSprites;
		CropValue = RoseValue;
		CurrentCropImage = RoseCropImage;
		CropCost = RoseSeedCost;
	
		OnCropChanged.Broadcast(CurrentCropImage, CropCost, CropValue);
	}
	
}

void AFarmerCharacter::SwapToStrawberrySeed()
{
	UGameplayStatics::PlaySound2D(GetWorld(),SwitchSound);
	
	if (IsStrawberrySpliced == true)
	{
		CurrentCropSprites = SplicedStrawberryCropSprites;
		CropValue = SplicedStrawberryValue;
		CurrentCropImage = SplicedStrawberryCropImage;
		CropCost = SplicedStrawberrySeedCost;

		OnCropChanged.Broadcast(CurrentCropImage, CropCost, CropValue);
	}
	else
	{
		CurrentCropSprites = StrawberryCropSprites;
		CropValue = StrawberryValue;
		CurrentCropImage = StrawberryCropImage;
		CropCost = StrawberrySeedCost;

		OnCropChanged.Broadcast(CurrentCropImage, CropCost, CropValue);
	}
	
}

void AFarmerCharacter::SwapToGrapeSeed()
{
	UGameplayStatics::PlaySound2D(GetWorld(),SwitchSound);
	
	if (IsGrapeSpliced == true)
	{
		CurrentCropSprites = SplicedGrapeCropSprites;
		CropValue = SplicedGrapeValue;
		CurrentCropImage = SplicedGrapeCropImage;
		CropCost = SplicedGrapeSeedCost;

		OnCropChanged.Broadcast(CurrentCropImage, CropCost, CropValue);
	}
	else
	{
		CurrentCropSprites = GrapeCropSprites;
		CropValue = GrapeValue;
		CurrentCropImage = GrapeCropImage;
		CropCost = GrapeSeedCost;

		OnCropChanged.Broadcast(CurrentCropImage, CropCost, CropValue);
	}
	
}

void AFarmerCharacter::SwapToPineappleSeed()
{
	UGameplayStatics::PlaySound2D(GetWorld(),SwitchSound);
	
	if (IsPineappleSpliced == true)
	{
		CurrentCropSprites = SplicedPineappleCropSprites;
		CropValue = SplicedPineappleValue;
		CurrentCropImage = SplicedPineappleCropImage;
		CropCost = SplicedPineappleSeedCost;

		OnCropChanged.Broadcast(CurrentCropImage, CropCost, CropValue);
	}
	else
	{
		CurrentCropSprites = PineappleCropSprites;
		CropValue = PineappleValue;
		CurrentCropImage = PineappleCropImage;
		CropCost = PineappleSeedCost;

		OnCropChanged.Broadcast(CurrentCropImage, CropCost, CropValue);
	}
	
}

void AFarmerCharacter::Move(const FInputActionValue& Value)
{
	
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//directional rotators
		FRotator RotA = UKismetMathLibrary::MakeRotator(0,0,180.0);
		FRotator RotB = UKismetMathLibrary::MakeRotator(0,0,0);
		FRotator Direction = UKismetMathLibrary::SelectRotator(RotA,RotB, MovementVector.X > 0.0);
		
		// get forward vector
		FVector ForwardDirection(1,0,0);
	
		// get right vector 
		FVector RightDirection(0,1,0);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);

		if (MovementVector.X != 0.0)
		{
			Controller->SetControlRotation(Direction);
		}
	}
}
