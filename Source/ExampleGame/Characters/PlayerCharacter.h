// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EGBaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS(Blueprintable)
// Класс главного персонажа-героя
class EXAMPLEGAME_API APlayerCharacter : public AEGBaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	// AEGBaseCharacter methods
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;
	virtual void Turn(float Value) override;
	virtual void LookUp(float Value) override;
	virtual void SwimForward(float Value) override;
	virtual void SwimUp(float Value) override;
	virtual void SwimRight(float Value) override;
	
	// Функционал для геймпадов
	virtual void TurnAtRate(float Value) override;
	virtual void LookUpAtRate(float Value) override;
	
	// ACharacter methods
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	
	// Обработчики сигналов для каких-либо действий
	virtual bool CanJumpInternal_Implementation() const override;
	virtual void OnJumped_Implementation() override;
	
	virtual void OnSprintStart_Implementation() override;
	virtual void OnSprintEnd_Implementation() override;
	
private:
	UFUNCTION()
	void SpringArmLengthUpdate(float Alfa);
	
	UFUNCTION()
	void SpringArmLengthUpdateFinished();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera")
	UCameraComponent* CameraComponent = nullptr;				// Компонент камеры
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Camera")
	USpringArmComponent* SpringArmComponent = nullptr;			// Компонент "палки" для камеры
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Camera")
	UCurveFloat* SpringArmLengthChangeCurveFloat = nullptr;		// Кривая, используемая для изменений таймлайна
	
private:
	float DefaultSpringArmLength = 0.0f;		// Стандартная (кешированная) длина "палки" для SpringArmComponent
	float SpringArmLength = 450.0f;				// Длина "палки" для SpringArmComponent при спринте
	
	FTimeline SpringArmLengthChangeTimeline;	// Таймлайн для плавных изменений длины "палки" у SpringArmComponent
};
