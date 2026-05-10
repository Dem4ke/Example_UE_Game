// Fill out your copyright notice in the Description page of Project Settings.

#include "EGBaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/MovementComponents/EGBaseCharacterMovementComponent.h"
#include "../Components/UtilityComponents/LedgeDetectorComponent.h"
#include "Curves/CurveVector.h"

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
	// Проверка не подтягивается ли уже персонаж
	if (MovementComponent->IsMantling())
	{
		return;
	}
	
	FLedgeDescription LedgeDescription;
	
	// Если выступ был найден
	if (LedgeDetectorComponent->DetectLedge(LedgeDescription))
	{
		FMantlingMovementParameters MantlingParams;
		MantlingParams.InitialLocation = GetActorLocation();
		MantlingParams.InitialRotation = GetActorRotation();
		MantlingParams.TargetLocation = LedgeDescription.Location;
		MantlingParams.TargetRotation = LedgeDescription.Rotation;
		
		float MantlingHeight = (MantlingParams.TargetLocation - MantlingParams.InitialLocation).Z;
		const FMantlingSettings& MantleSettings = GetMantlingSettings(MantlingHeight);
		
		float MinRange = 0.f;
		float MaxRange = 0.f;
		MantleSettings.MantlingCurve->GetTimeRange(MinRange, MaxRange);
		
		MantlingParams.Duration = MaxRange - MinRange;
		MantlingParams.MantlingCurve = MantleSettings.MantlingCurve;
		
		FVector2D SourceRange(MantleSettings.MinHeight, MantleSettings.MaxHeight);
		FVector2D TargetRange(MantleSettings.MinHeightStartTime, MantleSettings.MaxHeightStartTime);
		MantlingParams.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, MantlingHeight);
		
		// Подгонка координат начала и конца анимации
		MantlingParams.InitialAnimationLocation = MantlingParams.TargetLocation - 
			MantleSettings.AnimationCorrectionZ * FVector::UpVector + 
			MantleSettings.AnimationCorrectionXY * LedgeDescription.Normal;
		
		// Активация процесса подтягивания
		MovementComponent->StartMantle(MantlingParams);
		
		// Активация анимации
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(
			MantleSettings.MantlingMontage, 
			1.0f, 
			EMontagePlayReturnType::Duration, 
			MantlingParams.StartTime);
	}
}

bool AEGBaseCharacter::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() &&
		!MovementComponent->IsMantling();
}

// Указатель на возможность применение спринта
bool AEGBaseCharacter::CanSprint()
{
	// Если персонаж в воздухе, то бежать нельзя
	if (MovementComponent->IsFalling())
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

const FMantlingSettings& AEGBaseCharacter::GetMantlingSettings(float LedgeHeight) const
{
	if (LedgeHeight < LowMantleMaxHeight)
	{
		return LowMantleSettings;	
	}
		
	return HighMantleSettings;	
}
