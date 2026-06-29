// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EGBaseCharacter.generated.h"

class AInteractiveActor;
class UEGBaseCharacterMovementComponent;
class ULedgeDetectorComponent;
class UAnimMontage;
class UCurveVector;
class ALadder;

USTRUCT(BlueprintType)
// Характеристики лазания
struct FMantlingSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* MantlingMontage = nullptr;	// Монтаж с анимацией лазания
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveVector* MantlingCurve = nullptr;		// Набор кривых для настройки монтажа
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionXY = 65.0f;		// Смещение от целевой позиции лазания в направлении нормали выступа (радиус капсулы персонажа)
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionZ = 200.0f;		// Смещение от целевой позиции лазания по высоте в соответствии с анимацией (расчетная высота анимации)
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeight = 200.0f;					// Максимальная высота, на которую можно забраться с этой анимацией
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeight = 100.0f;					// Минимальная высота, на которую можно забраться с этой анимацией
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeightStartTime = 0.0f;			// Время начала проигрывания анимации при максимальной высоте
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeightStartTime = 0.5f;			// Время начала проигрывания анимации при минимальной высоте
};

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
	virtual void Mantle();
	virtual void InteractWithLadder();
	virtual void ClimbLadderUp(float Value);
	
	virtual bool CanJumpInternal_Implementation() const override;
	
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
	
	// Не переопределяемые методы
	void RegisterInteractiveActor(AInteractiveActor* InteractiveActor);
	void UnregisterInteractiveActor(AInteractiveActor* InteractiveActor);
	
	const ALadder* GetAvailableLadder() const;
	
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
	const FMantlingSettings& GetMantlingSettings(float LedgeHeight) const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Controls")
	float BaseTurnRate = 45.f;		// Базовая скорость поворота персонажа (камеры) по горизонтали
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Controls")
	float BaseLookUpRate = 45.f;	// Базовая скорость поворота персонажа (камеры) по вертикали
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float SprintSpeed = 1000.f;		// Базовая скорость спринта персонажа
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	ULedgeDetectorComponent* LedgeDetectorComponent = nullptr;			// Компонент для поиска уступов
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling")
	FMantlingSettings HighMantleSettings;	// Характеристики проигрывания анимации высокого лазания
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling")
	FMantlingSettings LowMantleSettings;	// Характеристики проигрывания анимации низкого лазания
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float LowMantleMaxHeight = 125.0f;		// Максимальная высота для проигрывания анимации низкого лазания
	
	UEGBaseCharacterMovementComponent* MovementComponent = nullptr;		// Компонент перемещения для персонажа
	
private:
	bool bIsSprintRequested = false;				// Состояние вызванности спринта
	TArray<AInteractiveActor*> InteractiveActors;	// Набор интерактивных объектов
};
