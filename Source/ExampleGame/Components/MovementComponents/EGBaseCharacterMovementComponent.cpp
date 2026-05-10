// Fill out your copyright notice in the Description page of Project Settings.


#include "EGBaseCharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
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

// Начало перелезаия препятствия
void UEGBaseCharacterMovementComponent::StartMantle(const FMantlingMovementParameters& MantlingParameters)
{
	CurrentMantlingParameters = MantlingParameters;
	SetMovementMode(EMovementMode::MOVE_Custom, static_cast<uint8>(ECustomMovementMode::CMOVE_Mantling));
}

// Окончание действия лазания
void UEGBaseCharacterMovementComponent::EndMantle()
{
	SetMovementMode(EMovementMode::MOVE_Walking);
}

bool UEGBaseCharacterMovementComponent::IsMantling() const
{
	return UpdatedComponent && 
		MovementMode == MOVE_Custom && 
		CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Mantling);
}

float UEGBaseCharacterMovementComponent::GetMaxSpeed() const
{
	if (bIsSprinting)
	{
		return SprintSpeed;
	}
	
	return Super::GetMaxSpeed();
}

// Обработчик физики в кастомном моде перемещения
void UEGBaseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
		case static_cast<uint8>(ECustomMovementMode::CMOVE_Mantling):
		{
			float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer) + 
				CurrentMantlingParameters.StartTime;
				
			FVector MantlingCurveValue = CurrentMantlingParameters.MantlingCurve->GetVectorValue(ElapsedTime);
			float PositionAlpha = MantlingCurveValue.X;
			float XYCorrectionAlpha = MantlingCurveValue.Y;
			float ZCorrectionAlpha = MantlingCurveValue.Z;
				
			FVector CorrectedInitialLocation = FMath::Lerp(CurrentMantlingParameters.InitialLocation, 
				CurrentMantlingParameters.InitialAnimationLocation, XYCorrectionAlpha);
				
			CorrectedInitialLocation.Z = FMath::Lerp(CurrentMantlingParameters.InitialLocation.Z, 
				CurrentMantlingParameters.InitialAnimationLocation.Z, ZCorrectionAlpha);
				
			FVector NewLocation = FMath::Lerp(CorrectedInitialLocation, 
				CurrentMantlingParameters.TargetLocation, PositionAlpha);	
			FRotator NewRotation = FMath::Lerp(CurrentMantlingParameters.InitialRotation, 
				CurrentMantlingParameters.TargetRotation, PositionAlpha);
				
			FVector DeltaLocation = NewLocation - GetActorLocation();
			FHitResult Hit;	
				
			SafeMoveUpdatedComponent(DeltaLocation, NewRotation, false, Hit);	
				
			break;
		}
		default:
		{
			break;
		}
	}
	
	Super::PhysCustom(DeltaTime, Iterations);
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
	
	// Если текущий режим перемещения - кастомный
	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
			case static_cast<uint8>(ECustomMovementMode::CMOVE_Mantling):
			{
				GetWorld()->GetTimerManager().SetTimer(MantlingTimer, 
					this, 
					&UEGBaseCharacterMovementComponent::EndMantle, 
					CurrentMantlingParameters.Duration, 
					false);
			}
			default:
			{
				break;
			}
		}
	}
}

// Получение состояния спринта
bool UEGBaseCharacterMovementComponent::IsSprinting() const
{
	return bIsSprinting;
}
