// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EGBaseCharacterAnimInstance.generated.h"

class AEGBaseCharacter;

UCLASS()
// Класс для работы с анимациями персонажа
class EXAMPLEGAME_API UEGBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	float Speed = 0.0f;				// Скорость персонажа
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsFalling = false;		// Признак нахождения персонажа в воздухе
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsCrouching = false;		// Признак нахождения персонажа в присяде
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation")
	bool bIsSprinting = false;		// Признак нахождения персонажа в спринте
	
private:
	TWeakObjectPtr<AEGBaseCharacter> CachedBaseCharacter;		// Указатель на текущего перснонажа
};
