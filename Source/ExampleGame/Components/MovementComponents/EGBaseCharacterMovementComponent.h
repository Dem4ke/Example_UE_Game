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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.0f, UIMin = 0.f))
	float SprintSpeed = 1200.f;		// Базовая скорость спринта персонажа
	
private:
	bool bIsSprinting = false;		// Состояние спринта персонажа
};
