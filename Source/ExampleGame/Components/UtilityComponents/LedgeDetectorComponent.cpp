// Fill out your copyright notice in the Description page of Project Settings.

#include "LedgeDetectorComponent.h"

#include "../../ExampleGameTypes.h"
#include "../../Utility/EGTraceUtility.h"
#include "../../Subsystems/DebugSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts
void ULedgeDetectorComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<ACharacter>(), TEXT("ULedgeDetectorComponent::BeginPlay() can use only a ACharacter"));
	CachedCharacterOwner = StaticCast<ACharacter*>(GetOwner());
}

// Метод для поиска доступной точки для залазания (параметры точки записываются в передаваемую структуру)
bool ULedgeDetectorComponent::DetectLedge(OUT FLedgeDescription& LedgeDescription)
{
	// Включение и выключение отладочной отрисовки возможно только при работе игры в дебаг режиме
#if ENABLE_DRAW_DEBUG
	// Получение информации о необходимости дебажной отрисовки
	UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	bool bIsDebugDrawEnabled = DebugSubsystem->IsCategoryEnabled(DebugCategoryLedgeDetection);
#else
	bool bIsDebugDrawEnabled = false;
#endif
	
	// Все проверки осуществляются с помощью Shape Traces (в виде капсулы игрока)
	// Получение капсулы игрока
	UCapsuleComponent* Capsule = CachedCharacterOwner->GetCapsuleComponent();
	
	// Параметры запроса на поиск коллизии
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());
	
	// Зазор для избежания нежелательных коллизий между персонажем и поверхностью пола
	float BottomZOffset = 2.0f;
	
	// Вычисление нижней точки капсулы
	FVector CharacterBottom = CachedCharacterOwner->GetActorLocation() - 
		(Capsule->GetScaledCapsuleHalfHeight() - BottomZOffset) * FVector::UpVector;
	
	// Forward check (проверка на вертикальные объекты)
	float ForwardCheckCapsuleRadius = Capsule->GetScaledCapsuleRadius();
	float ForwardCheckCapsuleHalfHeight = (MaximumLedgeHeight - MinimumLedgeHeight) * 0.5f;
	
	// Точка начала каста
	FVector ForwardStartLocation = CharacterBottom + 
		(MinimumLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	
	// Точка конца каста
	FVector ForwardEndLocation = ForwardStartLocation + 
		CachedCharacterOwner->GetActorForwardVector() * ForwardCheckDistance;
	
	// Результат коллизии
	FHitResult ForwardHitResult;
	
	// Проверка капсулы на коллизию с ближайшим объектом с помощью Shape trace
	if (!EGTraceUtility::SweepCapsuleSingleByChanel(
		GetWorld(),
		ForwardHitResult,
		ForwardStartLocation,
		ForwardEndLocation,
		ForwardCheckCapsuleRadius,
		ForwardCheckCapsuleHalfHeight,
		FQuat::Identity,
		ECC_Climbing,
		QueryParams,
		FCollisionResponseParams::DefaultResponseParam,
		bIsDebugDrawEnabled,
		2.0f))
	{
		return false;
	}

	// Download check (проверка на высоту найденного вертикального объекта)
	// Результат коллизии
	FHitResult DownwardHitResult;
	float DownwardSphereTraceRadius = Capsule->GetScaledCapsuleRadius();
	
	// Точка начала каста
	float DepthOffset = 10.f;
	FVector DownwardStartLocation = ForwardHitResult.ImpactPoint - DepthOffset * ForwardHitResult.ImpactNormal;
	DownwardStartLocation.Z = CharacterBottom.Z + MaximumLedgeHeight + DownwardSphereTraceRadius;
	
	// Точка конца каста
	FVector DownwardEndLocation(DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBottom.Z);
	
	if (!EGTraceUtility::SweepSphereSingleByChanel(
		GetWorld(),
		DownwardHitResult,
		DownwardStartLocation,
		DownwardEndLocation,
		DownwardSphereTraceRadius,
		ECC_Climbing,
		QueryParams,
		FCollisionResponseParams::DefaultResponseParam,
		bIsDebugDrawEnabled,
		2.0f))
	{
		return false;
	}
	
	// Overlap check (проверка на дополнительные препятствия в месте подъема игрока)	
	float OverlapCapsuleFloorOffset = 2.0f;
	FVector OverlapLocation = DownwardHitResult.ImpactPoint + 
		(Capsule->GetScaledCapsuleHalfHeight() + OverlapCapsuleFloorOffset) * FVector::UpVector;
	
	// Тест коллизии по каналам
	if (EGTraceUtility::OverlapCapsuleAnyByProfile(
		GetWorld(),
		OverlapLocation,
		Capsule->GetScaledCapsuleRadius(),
		Capsule->GetScaledCapsuleHalfHeight(),
		FQuat::Identity,
		CollisionProfilePawn,
		QueryParams,
		bIsDebugDrawEnabled,
		2.0f))
	{
		return false;
	}
	
	// Задание информации о выступе
	LedgeDescription.Location = OverlapLocation;
	LedgeDescription.Normal = ForwardHitResult.ImpactNormal;
	// Нормаль берем перевернутую (* - перегруженный оператор для компонентного произведения (просто умножает х * х | y * y | z * z))
	LedgeDescription.Rotation = (ForwardHitResult.ImpactNormal * FVector(-1.f, -1.f, 0.f)).ToOrientationRotator();
	
	return true;
}




