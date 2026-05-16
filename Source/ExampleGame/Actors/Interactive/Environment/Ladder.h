// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../InteractiveActor.h"
#include "Ladder.generated.h"

class UStaticMeshComponent;

// Класс лестницы как интерактивного объекта
UCLASS(Blueprintable)
class EXAMPLEGAME_API ALadder : public AInteractiveActor
{
	GENERATED_BODY()
	
public:
	ALadder();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder paramers")
	float Height = 100.0f;				// Высота лестницы
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder paramers")
	float Width = 50.0f;				// Ширина лестницы
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder paramers")
	float StepsInterval = 25.0f;		// Расстояние между ступенями
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder paramers")
	float BottomStepOffset = 25.0f;		// Расстояние от начала лестницы до первой ступени
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RightRailMeshComponent = nullptr;			// Правый поручень лестницы
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LeftRailMeshComponent = nullptr;			// Левый поручень лестницы
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInstancedStaticMeshComponent* StepsMeshComponent = nullptr;	// Сетка для ступеней
};
