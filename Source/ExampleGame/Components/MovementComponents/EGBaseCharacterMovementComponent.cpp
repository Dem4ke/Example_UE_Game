// Fill out your copyright notice in the Description page of Project Settings.


#include "EGBaseCharacterMovementComponent.h"

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

// Получение состояния спринта
bool UEGBaseCharacterMovementComponent::IsSprinting() const
{
	return bIsSprinting;
}
