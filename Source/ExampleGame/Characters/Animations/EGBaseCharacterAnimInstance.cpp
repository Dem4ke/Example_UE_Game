// Fill out your copyright notice in the Description page of Project Settings.


#include "EGBaseCharacterAnimInstance.h"
#include "../PlayerCharacter.h"
#include "../../Components/MovementComponents/EGBaseCharacterMovementComponent.h"

void UEGBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	// Обязательная проверка на то, что данный класс работает исключительно с AEGBaseCharacter
	checkf(TryGetPawnOwner()->IsA<AEGBaseCharacter>(), 
		TEXT("UEGBaseCharacterAnimInstance::NativeBeginPlay() can be used only with AEGBaseCharacter"));
	
	// Кеширование персонажа
	CachedBaseCharacter = StaticCast<AEGBaseCharacter*>(TryGetPawnOwner());
}

void UEGBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	// Проверка кеша на валидность
	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}
	
	// Установка значений
	UEGBaseCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetBaseCharacterMovementComponent();
	Speed = CharacterMovement->Velocity.Size();
	bIsFalling = CharacterMovement->IsFalling();
	bIsCrouching = CharacterMovement->IsCrouching();
	bIsSprinting = CharacterMovement->IsSprinting();
}
