// Fill out your copyright notice in the Description page of Project Settings.

#include "EGBaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/MovementComponents/EGBaseCharacterMovementComponent.h"
#include "../Components/UtilityComponents/LedgeDetectorComponent.h"

// Конструктор с переинициализацией MovementComponent
AEGBaseCharacter::AEGBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UEGBaseCharacterMovementComponent>
		(ACharacter::CharacterMovementComponentName))
{
	MovementComponent = StaticCast<UEGBaseCharacterMovementComponent*>(GetCharacterMovement());
	
	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetectorComponent"));
}

// Обработчик игрового кадра
void AEGBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Изменения состояния спринта
	TryChangeSprintState();
}

// Получение компонента перемещения персонажа
UEGBaseCharacterMovementComponent* AEGBaseCharacter::GetBaseCharacterMovementComponent() const
{
	return MovementComponent;
}

// Обработчик изменения состояния приседа
void AEGBaseCharacter::ChangeCrouchState()
{
	// Если персонаж в воздухе, то присесть нельзя
	if (MovementComponent->IsFalling())
	{
		return;
	}
	
	// Если пешка уже в присяде, то она встает
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	// Если нет, то присяд
	else
	{
		Crouch();
	}
}

// Обработчик начала спринта
void AEGBaseCharacter::StartSprint()
{
	bIsSprintRequested = true;
	
	// Если персонаж в состоянии присяда
	if (bIsCrouched)
	{
		// Вызывается команда для выхода из состояния присяда
		UnCrouch();
	}
}

// Обработчик конца спринта
void AEGBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

// Метод подтягивания персонажа
void AEGBaseCharacter::Mantle()
{
	FLedgeDescription LedgeDescription;
	
	// Если выступ был найден
	if (LedgeDetectorComponent->DetectLedge(LedgeDescription))
	{
		// Активация процесса подтягивания
		
	}
}

// Указатель на возможность применение спринта
bool AEGBaseCharacter::CanSprint()
{
	// Если персонаж в воздухе или присяде, то бежать нельзя
	if (MovementComponent->IsFalling() ||
		MovementComponent->IsCrouching())
	{
		return false;
	}
	
	return true;
}

// Изменение состояния спринта
void AEGBaseCharacter::TryChangeSprintState()
{
	if (bIsSprintRequested && 
		!MovementComponent->IsSprinting() && 
		CanSprint()) 
	{
		MovementComponent->StartSprint();
		OnSprintStart();	// Уведомление о начале спринта
	}	
	else if (!bIsSprintRequested && 
		MovementComponent->IsSprinting())
	{
		MovementComponent->StopSprint();
		OnSprintEnd();		// Уведомление об окончании спринта
	}
}
