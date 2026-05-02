// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EGBaseCharacter.generated.h"

class UEGBaseCharacterMovementComponent;

// Abstract - запрещает создавать объект данного класса на сцене
// NotBlueprintable - запрещает создавать блупринты для данного класса
UCLASS(Abstract, NotBlueprintable)
// Базовый класс представляет интерфейс для обработки движений
class EXAMPLEGAME_API AEGBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEGBaseCharacter(const FObjectInitializer& ObjectInitializer);
	
	// ACharacter methods
	virtual void Tick(float DeltaTime) override;
	
	UEGBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() const;
	
	// Методы с базовым поведением
	virtual void ChangeCrouchState();
	virtual void StartSprint();
	virtual void StopSprint();
	
	// Чисто виртуальные методы
	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void LookUp(float Value) {};
	virtual void SwimForward(float Value) {};
	virtual void SwimUp(float Value) {};
	virtual void SwimRight(float Value) {};
	
	// Функционал для геймпадов
	virtual void TurnAtRate(float Value) {};
	virtual void LookUpAtRate(float Value) {};
	
protected:
	virtual bool CanSprint();
	
	// Сигналы и обработчики для спринта 
	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintStart();
	virtual void OnSprintStart_Implementation() {};
	
	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintEnd();
	virtual void OnSprintEnd_Implementation() {};
	
private:
	void TryChangeSprintState();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Controls")
	float BaseTurnRate = 45.f;		// Базовая скорость поворота персонажа (камеры) по горизонтали
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Controls")
	float BaseLookUpRate = 45.f;	// Базовая скорость поворота персонажа (камеры) по вертикали
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float SprintSpeed = 1000.f;		// Базовая скорость спринта персонажа
	
	UEGBaseCharacterMovementComponent* MovementComponent = nullptr;		// Компонент перемещения для персонажа
	
private:
	bool bIsSprintRequested = false;		// Состояние вызванности спринта
};
