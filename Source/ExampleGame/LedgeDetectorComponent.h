// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LedgeDetectorComponent.generated.h"

class ACharacter;

// Структура, описывающая выступ
USTRUCT(BlueprintType)
struct FLedgeDescription
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	FVector Location;	// Локация выступа (точка, на которую должен забраться персонаж)
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	FRotator Rotation;	// Ориентация выступа в пространстве (как повернут выступ, чтобы повернуть персонажа)
};

// Класс для осуществления пространственной проверки выступов, на которые можно забраться 
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXAMPLEGAME_API ULedgeDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Метод для поиска доступной точки для залазания (параметры точки записываются в передаваемую структуру)
	bool DetectLedge(OUT FLedgeDescription& LedgeDescription);		
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (UIMin = 0.f, ClampMin = 0.f))
	float MinimumLedgeHeight = 40.f;		// Минимальная высота выступа, на который можно забраться
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (UIMin = 0.f, ClampMin = 0.f))
	float MaximumLedgeHeight = 200.f;		// Максимальная высота выступа, на который можно забраться
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (UIMin = 0.f, ClampMin = 0.f))
	float ForwardCheckDistance = 100.f;		// Длина каста для поиска подходящего выступа
	
private:
	TWeakObjectPtr<ACharacter> CachedCharacterOwner;	// Закешированный владелец данного компонента (персонаж)
};
