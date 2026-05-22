// Fill out your copyright notice in the Description page of Project Settings.

#include "Ladder.h"
#include "../../../ExampleGameTypes.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

// Конструктор
ALadder::ALadder()
{
	// Создание компонентов и прикрепление их к корневому элементу
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LadderRoot"));
	
	LeftRailMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftRail"));	
	LeftRailMeshComponent->SetupAttachment(RootComponent);
	
	RightRailMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightRail"));
	RightRailMeshComponent->SetupAttachment(RootComponent);
	
	StepsMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Steps"));
	StepsMeshComponent->SetupAttachment(RootComponent);
	
	// Создание интерактивного объема (выставление канала коллизии)
	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	InteractionVolume->SetGenerateOverlapEvents(true);
}

// Вызывается при создании объекта с сохраненными значениями из блупринтов
void ALadder::OnConstruction(const FTransform& Transform)
{
	// Выставление местоположений у направляющих
	LeftRailMeshComponent->SetRelativeLocation(FVector(0.0f, -Width * 0.5f, Height * 0.5f));
	RightRailMeshComponent->SetRelativeLocation(FVector(0.0f, Width * 0.5f, Height * 0.5f));
	
	// Получение сеток для компонентов
	UStaticMesh* LeftRailMesh = LeftRailMeshComponent->GetStaticMesh();
	UStaticMesh* RightRailMesh = RightRailMeshComponent->GetStaticMesh();
	UStaticMesh* StepsMesh = StepsMeshComponent->GetStaticMesh();
	
	// Масштабирование сеток
	if (IsValid(LeftRailMesh))
	{
		float MeshHeight = LeftRailMesh->GetBoundingBox().GetSize().Z;
		
		if (!FMath::IsNearlyZero(MeshHeight))
		{
			LeftRailMeshComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, Height / MeshHeight));
		}
	}
	
	if (IsValid(RightRailMesh))
	{
		float MeshHeight = RightRailMesh->GetBoundingBox().GetSize().Z;
		
		if (!FMath::IsNearlyZero(MeshHeight))
		{
			RightRailMeshComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, Height / MeshHeight));
		}
	}
	
	if (IsValid(StepsMesh))
	{
		float MeshWidth = StepsMesh->GetBoundingBox().GetSize().Y;
		
		if (!FMath::IsNearlyZero(MeshWidth))
		{
			StepsMeshComponent->SetRelativeScale3D(FVector(1.0f, Width / MeshWidth, 1.0f));
		}
	}
	
	// Заполнение ступеней
	StepsMeshComponent->ClearInstances();
	uint32 StepsCount = FMath::FloorToInt((Height - BottomStepOffset) / StepsInterval);
	
	for (uint32 i = 0; i < StepsCount; ++i)
	{
		FTransform StepTransform(FVector(1.0f, 1.0f, BottomStepOffset + i * StepsInterval));
		StepsMeshComponent->AddInstance(StepTransform);
	}
	
	float BoxDepthExtent = GetLadderInteractionBox()->GetUnscaledBoxExtent().X;
	GetLadderInteractionBox()->SetBoxExtent(FVector(BoxDepthExtent, Width * 0.5f, Height * 0.5f));
	GetLadderInteractionBox()->SetRelativeLocation(FVector(BoxDepthExtent, 0.0f, Height * 0.5f));
}

// Геттер для интерактивного объема 
UBoxComponent* ALadder::GetLadderInteractionBox() const
{
	return StaticCast<UBoxComponent*>(InteractionVolume);
}
