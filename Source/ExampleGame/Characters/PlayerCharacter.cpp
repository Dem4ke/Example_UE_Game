// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"

// Конструктор
APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Создание элементов и закрепление их в иерархии объектов класса Player
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	
	// Задание свободного полета у камеры
	SpringArmComponent->bUsePawnControlRotation = true;
	
	// Базовым компонентом является капсула с коллизией
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->bUsePawnControlRotation = false;
	
	// Для камеры родитель - палка
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	// Задание поворота персонажа за движением мыши
	GetCharacterMovement()->bOrientRotationToMovement = 1;
	// Скорость поворота персонажа
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

// Обработчик сигнала начала работы с объектом класса
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Задание кэшированного значения длины "палки" для SpringArmComponent 
	DefaultSpringArmLength = SpringArmComponent->TargetArmLength;
	
	// Создание таймлайна
	// Связывание сигнала с обработчиком по имени метода обработчика
	FOnTimelineFloat ProgressUpdate;
	ProgressUpdate.BindUFunction(this, FName("SpringArmLengthUpdate"));
	
	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this, FName("SpringArmLengthUpdateFinished"));
	
	SpringArmLengthChangeTimeline.AddInterpFloat(SpringArmLengthChangeCurveFloat, ProgressUpdate);
	SpringArmLengthChangeTimeline.SetTimelineFinishedFunc(FinishedEvent);
}

// Обработчик обновления каждого кадра
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Обновление таймлайна при его работе
	if (SpringArmLengthChangeTimeline.IsPlaying())
	{
		SpringArmLengthChangeTimeline.TickTimeline(DeltaTime);
	}
}

// Обработка движения вперед
void APlayerCharacter::MoveForward(float Value)
{	
	// Если персонаж не на замле и не в состоянии падения, то ничего не делать
	if (!GetCharacterMovement()->IsMovingOnGround() && 
		!GetCharacterMovement()->IsFalling())
	{
		return;
	}
	
	// Проверка на близость к нулю
	if (!FMath::IsNearlyZero(Value, 1e-6f))
	{
		// Получение информации о векторе движения по X и поворот его на вектор 1,0,0
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector(YawRotator.RotateVector(FVector::ForwardVector));
		AddMovementInput(ForwardVector, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	// Если персонаж не на замле и не в состоянии падения, то ничего не делать
	if (!GetCharacterMovement()->IsMovingOnGround() && 
		!GetCharacterMovement()->IsFalling())
	{
		return;
	}
	
	// Проверка на близость к нулю
	if (!FMath::IsNearlyZero(Value, 1e-6f))
	{
		// Получение информации о векторе движения по Y и поворот его на вектор 0,1,0
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector RightVector(YawRotator.RotateVector(FVector::RightVector));
		AddMovementInput(RightVector, Value);
	}
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APlayerCharacter::SwimForward(float Value)
{
	if (!GetCharacterMovement()->IsSwimming())
	{
		return;
	}
	
	// Проверка на близость к 0
	if (!FMath::IsNearlyZero(Value, 1e-6f))
	{
		// Получение информации о векторе движения по X и поворот его на вектор 1,0,0
		FRotator PitchYawRotator(GetControlRotation().Pitch, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector(PitchYawRotator.RotateVector(FVector::ForwardVector));
		AddMovementInput(ForwardVector, Value);
	}
}

void APlayerCharacter::SwimUp(float Value)
{
	if (!GetCharacterMovement()->IsSwimming())
	{
		return;
	}
	
	// Проверка на близость к 0
	if (!FMath::IsNearlyZero(Value, 1e-6f))
	{
		// Установка движения сторого по вектору 0,0,1
		AddMovementInput(FVector::UpVector, Value);
	}
}

void APlayerCharacter::SwimRight(float Value)
{
	if (!GetCharacterMovement()->IsSwimming())
	{
		return;
	}
	
	// Проверка на близость к 0
	if (!FMath::IsNearlyZero(Value, 1e-6f))
	{
		// Получение информации о векторе движения по Y и поворот его на вектор 0,1,0
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector RightVector(YawRotator.RotateVector(FVector::RightVector));
		AddMovementInput(RightVector, Value);
	}
}

void APlayerCharacter::TurnAtRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Метод срабатывает при выполненном приседании
// Параметры: 1 - половина высоты капсулы, 2 - масштабированное значение половины высоты капсулы (если применен масштаб)
void APlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	// Обработка перестройки сетки персонажа
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	
	// Добавляем к сдвигу камеры половину высоты капсулы
	SpringArmComponent->TargetOffset += FVector(0.0f, 0.0f, HalfHeightAdjust);
}

// Метод срабатывает при выполненном вставания
void APlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	// Обработка перестройки сетки персонажа
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	
	// Вычитаем из сдвига камеры половину высоты капсулы
	SpringArmComponent->TargetOffset -= FVector(0.0f, 0.0f, HalfHeightAdjust);
}

// Метод проверки может ли персонаж совершить прыжок
bool APlayerCharacter::CanJumpInternal_Implementation() const
{
	return bIsCrouched || Super::CanJumpInternal_Implementation();
}

void APlayerCharacter::OnJumped_Implementation()
{
	// Если игрок в присяде, то он должен встать
	if (bIsCrouched)
	{
		UnCrouch();
	}
}

// Обработчик сигнала начала спринта
void APlayerCharacter::OnSprintStart_Implementation()
{
	// Прямое проигрывание анимации
	SpringArmLengthChangeTimeline.Play();
}

// Обработчик сигнала окончания спринта
void APlayerCharacter::OnSprintEnd_Implementation()
{
	// Обратное проигрывание анимации
	SpringArmLengthChangeTimeline.Reverse();
}

// Обработчик работы таймлайна изменения длины "палки" у SpringArmComponent
void APlayerCharacter::SpringArmLengthUpdate(float Alfa)
{
	// Задание длины "палки" у spring arm через линейную интерполяцию
	SpringArmComponent->TargetArmLength = FMath::Lerp(DefaultSpringArmLength, SpringArmLength, Alfa);
}

// Окончание изменения длины "палки" у SpringArmComponent
void APlayerCharacter::SpringArmLengthUpdateFinished()
{
	// В данной реализации пустой метод
}

