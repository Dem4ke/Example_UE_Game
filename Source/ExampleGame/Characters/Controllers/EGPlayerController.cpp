// Fill out your copyright notice in the Description page of Project Settings.

#include "EGPlayerController.h"
#include "../EGBaseCharacter.h"

// Метод задания пешки для обработки контроллером
void AEGPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	
	// Пустой указатель валиден
	CachedBaseCharacter = Cast<AEGBaseCharacter>(InPawn);
}

// Задание возможных инпутов
void AEGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// Задание инпутов
	InputComponent->BindAxis("MoveForward", this, &AEGPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AEGPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AEGPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AEGPlayerController::LookUp);
	InputComponent->BindAxis("SwimForward", this, &AEGPlayerController::SwimForward);
	InputComponent->BindAxis("SwimUp", this, &AEGPlayerController::SwimUp);
	InputComponent->BindAxis("SwimRight", this, &AEGPlayerController::SwimRight);
	InputComponent->BindAxis("TurnAtRate", this, &AEGPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUpAtRate", this, &AEGPlayerController::LookUpAtRate);
	
	InputComponent->BindAction("Mantle", EInputEvent::IE_Pressed, this, &AEGPlayerController::Mantle);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AEGPlayerController::Jump);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AEGPlayerController::ChangeCrouchState);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AEGPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AEGPlayerController::StopSprint);
}

// Обработка движения вперед
void AEGPlayerController::MoveForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(Value);
	}
}

// Обработка движения вбок
void AEGPlayerController::MoveRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(Value);
	}
}

// Обработка поворота по горизонтали
void AEGPlayerController::Turn(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Turn(Value);
	}
}

// Обработка поворота по вертикали
void AEGPlayerController::LookUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUp(Value);
	}
}

void AEGPlayerController::SwimForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimForward(Value);
	}
}

void AEGPlayerController::SwimUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimUp(Value);
	}
}

void AEGPlayerController::SwimRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimRight(Value);
	}
}

void AEGPlayerController::TurnAtRate(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->TurnAtRate(Value);
	}
}

void AEGPlayerController::LookUpAtRate(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUpAtRate(Value);
	}
}

void AEGPlayerController::Mantle()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Mantle();
	}
}

// Обработка прыжка
void AEGPlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Jump();
	}
}

// Обработка смены положения игрока
void AEGPlayerController::ChangeCrouchState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeCrouchState();
	}
}

// Обработка начала спринта
void AEGPlayerController::StartSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartSprint();
	}
}

// Обработка окончания спринта
void AEGPlayerController::StopSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopSprint();
	}
}
