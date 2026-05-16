// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

class UPrimitiveComponent;

// Абстрактный класс для работы с интерактивными объектами
UCLASS(Abstract, NotBlueprintable)
class EXAMPLEGAME_API AInteractiveActor : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnInteractionVolumeOverlapBegin(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnInteractionVolumeOverlapEnd(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

protected:
	// Автоматический регистратор этого класса у персонажа рядом
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interatrion")
	UPrimitiveComponent* InteractionVolume = nullptr;
};
