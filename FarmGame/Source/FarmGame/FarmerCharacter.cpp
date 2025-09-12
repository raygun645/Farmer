// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


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

	//set to turnip as default
	CurrentCropSprites = TurnipCropSprites;
	CropValue = TurnipValue;
	CurrentCropImage = TurnipCropImage;
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
				int32 Profit = Money += GridSoil->PlantedCropValue;
				OnMoneyChanged.Broadcast(Profit);
			}
			
		}
	}
}

void AFarmerCharacter::Water()
{
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
			if (Money > 0)
			{
				UE_LOG(LogTemp, Display, TEXT("PlantedSeed (Player Side)"));
				bool PlantResult = InteractionInterface->Plant(CurrentCropSprites, CropValue);
				
				if (PlantResult == true)
				{
					UGameplayStatics::PlaySound2D(GetWorld(),PlantSound);
					
					Money -= 20;
					OnMoneyChanged.Broadcast(Money);
				}
			}
		}
	}
}

//here seeds will be assigned to a button press
void AFarmerCharacter::SwapToTurnipSeed()
{
	CurrentCropSprites = TurnipCropSprites;
	CropValue = TurnipValue;
	CurrentCropImage = TurnipCropImage;

	OnSpriteImageChanged.Broadcast(CurrentCropImage);
}

void AFarmerCharacter::SwapToRoseSeed()
{
	CurrentCropSprites = RoseCropSprites;
	CropValue = RoseValue;
	CurrentCropImage = RoseCropImage;
	
	OnSpriteImageChanged.Broadcast(CurrentCropImage);
}

void AFarmerCharacter::SwapToStrawberrySeed()
{
	CurrentCropSprites = StrawberryCropSprites;
	CropValue = StrawberryValue;
	CurrentCropImage = StrawberryCropImage;

	OnSpriteImageChanged.Broadcast(CurrentCropImage);
}

void AFarmerCharacter::SwapToGrapeSeed()
{
	CurrentCropSprites = GrapeCropSprites;
	CropValue = GrapeValue;
	CurrentCropImage = GrapeCropImage;

	OnSpriteImageChanged.Broadcast(CurrentCropImage);
}

void AFarmerCharacter::SwapToPineappleSeed()
{
	CurrentCropSprites = PineappleCropSprites;
	CropValue = PineappleValue;
	CurrentCropImage = PineappleCropImage;

	OnSpriteImageChanged.Broadcast(CurrentCropImage);
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
