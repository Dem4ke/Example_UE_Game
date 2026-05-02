// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EGBaseCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class EXAMPLEGAME_API UEGBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:	
	void StartSprint();
	void StopSprint();
	
	bool IsSprinting() const;
	float GetMaxSpeed() const override;
	
protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.0f, UIMin = 0.f))
	float SprintSpeed = 1200.f;					// Базовая скорость спринта персонажа
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Swimming", meta = (ClampMin = "0", UIMin = "0"))
	float SwimmingCapsuleRadius = 60.f;			// Радиус капсулы игрока при плавании
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: Swimming", meta = (ClampMin = "0", UIMin = "0"))
	float SwimmingCapsuleHalfHeight = 60.f;		// Половина высоты капсулы игрока при плавании
	
private:
	bool bIsSprinting = false;		// Состояние спринта персонажа
};
