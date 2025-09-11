// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
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

		//pause
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AFarmerCharacter::PauseGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
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
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, SoilClassFilter);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(OverlappingActor);
		
		if (OverlappingActor && InteractionInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Hoed (Player Side)"));
			InteractionInterface->Hoe();
		}
	}
}

void AFarmerCharacter::Water()
{
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
			UE_LOG(LogTemp, Display, TEXT("PlantedSeed (Player Side)"));
			InteractionInterface->Plant();
		}
	}
}

void AFarmerCharacter::Move(const FInputActionValue& Value)
{
	//taken from UE base characters
	
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

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
