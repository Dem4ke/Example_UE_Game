// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActor.h"
#include "Components/CapsuleComponent.h"
#include "../../Characters/EGBaseCharacter.h"

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(InteractionVolume))
	{
		InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveActor::OnInteractionVolumeOverlapBegin);
		InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AInteractiveActor::OnInteractionVolumeOverlapEnd);
	}
}

// Обработка начала пересечения интерактивного объема
void AInteractiveActor::OnInteractionVolumeOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, const 
	FHitResult& SweepResult)
{
	// Единственный тип Actor, который должен обрабатываться - это персонаж
	AEGBaseCharacter* BaseCharacter = Cast<AEGBaseCharacter>(OtherActor);
	
	// Если персонаж не валиден - выход
	if (!IsValid(BaseCharacter))
	{
		return;	
	}
	
	// Вызов пересечения исключительно от капсулы игрока
	if (Cast<UCapsuleComponent>(OtherComp) != BaseCharacter->GetCapsuleComponent())
	{
		return;
	}
	
	// Регистрация актора персонажа
	BaseCharacter->RegisterInteractiveActor(this);
}

// Обработка конца пересечения интерактивного объема
void AInteractiveActor::OnInteractionVolumeOverlapEnd(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	// Единственный тип Actor, который должен обрабатываться - это персонаж
	AEGBaseCharacter* BaseCharacter = Cast<AEGBaseCharacter>(OtherActor);
	
	// Если персонаж не валиден - выход
	if (!IsValid(BaseCharacter))
	{
		return;	
	}
	
	// Вызов пересечения исключительно от капсулы игрока
	if (Cast<UCapsuleComponent>(OtherComp) != BaseCharacter->GetCapsuleComponent())
	{
		return;
	}
	
	// Отмена регистрации актора персонажа
	BaseCharacter->UnregisterInteractiveActor(this);
}
