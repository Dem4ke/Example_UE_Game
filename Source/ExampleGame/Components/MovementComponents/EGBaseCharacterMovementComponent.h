// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UtilityComponents/LedgeDetectorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EGBaseCharacterMovementComponent.generated.h"

struct FMantlingMovementParameters
{
	FVector InitialLocation = FVector::ZeroVector;			// Начальное положение 
	FRotator InitialRotation = FRotator::ZeroRotator;		// Начальный поворот
	
	FVector TargetLocation = FVector::ZeroVector;			// Конечное положение 
	FRotator TargetRotation = FRotator::ZeroRotator;		// Конечный поворот
	
	FVector InitialAnimationLocation = FVector::ZeroVector;	// Изначальное положение персонажа с учетом полного проигрывания анимации 
	
	float Duration = 1.0f;									// Длительность подтягивания
	float StartTime = 0.0f;									// Время начала проигрывания анимации
	
	UCurveVector* MantlingCurve = nullptr;					// Кривая для настройки анимации
};

UENUM(BlueprintType)
// Обозначение кастомного способа передвижения
enum class ECustomMovementMode : uint8 
{  
	CMOVE_None = 0 UMETA(DisplayName = "None"),			// Передвижения не существует
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),		// Лазание
	CMOVE_Ladder UMETA(DisplayName = "Ladder"),			// Передвижение по лестнице
	CMOVE_Max UMETA(Hiden),								// Максимальное значение способа передвижения
};

class ALadder;

UCLASS()
class EXAMPLEGAME_API UEGBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:	
	void StartSprint();
	void StopSprint();
	
	void StartMantle(const FMantlingMovementParameters& MantlingParameters);
	void EndMantle();
	bool IsMantling() const;
	
	void AttachToLadder(const ALadder* Ladder);
	void DetachFromLadder();
	bool IsOnLadder() const;
	const ALadder* GetCurrentLadder() const;
	
	bool IsSprinting() const;
	float GetMaxSpeed() const override;
	
protected:
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

private:
	// Методы физики
	void PhysMantle(float DeltaTime, int32 Iterations);
	void PhysLadder(float DeltaTime, int32 Iterations);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.0f, UIMin = 0.f))
	float SprintSpeed = 1200.f;					// Базовая скорость спринта персонажа
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Swimming", meta = (ClampMin = "0", UIMin = "0"))
	float SwimmingCapsuleRadius = 60.f;			// Радиус капсулы игрока при плавании
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Swimming", meta = (ClampMin = "0", UIMin = "0"))
	float SwimmingCapsuleHalfHeight = 60.f;		// Половина высоты капсулы игрока при плавании
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Ladder", meta = (ClampMin = "0", UIMin = "0"))
	float LadderClimbingMaxSpeed = 200.f;						// Максимальная скорость перемещения по лестнице
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Ladder", meta = (ClampMin = "0", UIMin = "0"))
	float LadderClimbingBreakingDeceleration = 2048.f;			// Скорость затухания движения
	
private:
	bool bIsSprinting = false;		// Состояние спринта персонажа
	
	FMantlingMovementParameters CurrentMantlingParameters;	// Параметры анимации подтягивания	
	FTimerHandle MantlingTimer;								// Таймер для работы с подтягиванием
	
	const ALadder* CurrentLadder = nullptr;	// Параметры текущей используемой лестницы
};
