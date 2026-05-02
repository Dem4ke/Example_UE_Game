// Fill out your copyright notice in the Description page of Project Settings.


#include "EGBaseCharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

// Начало спринта
void UEGBaseCharacterMovementComponent::StartSprint()
{
	// Задание состояния спринта
	bIsSprinting = true;
	
	// Принудительная постановка скорости движения как максимальной
	bForceMaxAccel = 1;
}

void UEGBaseCharacterMovementComponent::StopSprint()
{
	// Задание состояния спринта
	bIsSprinting = false;
	
	// Снятие принудительной постановки скорости движения как максимальной
	bForceMaxAccel = 0;
}

float UEGBaseCharacterMovementComponent::GetMaxSpeed() const
{
	if (bIsSprinting)
	{
		return SprintSpeed;
	}
	
	return Super::GetMaxSpeed();
}

// Метод обработки изменения состояния движения персонажа
void UEGBaseCharacterMovementComponent::OnMovementModeChanged(
	EMovementMode PreviousMovementMode,
	uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	
	// Если текущий режим перемещения - плавание
	if (MovementMode == MOVE_Swimming)
	{
		// Изменение размера капсулы игрока под вариант плавания
		// CharacterOwner - владелец компонента
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(SwimmingCapsuleRadius, SwimmingCapsuleHalfHeight);
	}
	else if (PreviousMovementMode == MOVE_Swimming)
	{
		// Получение стандартных значений капсулы, которые устанавливаются при настройке персонажа
		// Объект класса ACharacter с базовым набором параметров
		ACharacter* DefCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		
		// Изменение размера капсулы игрока	на стандартное
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(
			DefCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), 
			DefCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(),
			true);
	}
}

// Получение состояния спринта
bool UEGBaseCharacterMovementComponent::IsSprinting() const
{
	return bIsSprinting;
}
