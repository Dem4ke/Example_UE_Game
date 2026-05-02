// Fill out your copyright notice in the Description page of Project Settings.

#include "LedgeDetectorComponent.h"

#include "ExampleGameTypes.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

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
	
	FCollisionShape ForwardCapsuleShape = 
		FCollisionShape::MakeCapsule(ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight);	
	
	// Точка начала каста
	FVector ForwardStartLocation = CharacterBottom + 
		(MinimumLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	
	// Точка конца каста
	FVector ForwardEndLocation = ForwardStartLocation + 
		CachedCharacterOwner->GetActorForwardVector() * ForwardCheckDistance;
	
	// Результат коллизии
	FHitResult ForwardHitResult;
	
	// Создание shape cast а по нашей форме и проверка на коллизии
	if (!GetWorld()->SweepSingleByChannel(ForwardHitResult, 
		ForwardStartLocation, 
		ForwardEndLocation, 
		FQuat::Identity, 
		ECC_Climbing,
		ForwardCapsuleShape,
		QueryParams))
	{
		return false;
	}
	
	// Download check (проверка на высоту найденного вертикального объекта)
	// Результат коллизии
	FHitResult DownwardHitResult;
	
	// Создание сферы для каста (здесь не нужна капсула, так как кастим строго вниз и проверка только по радиусу)
	float DownwardSphereTraceRadius = Capsule->GetScaledCapsuleRadius();
	FCollisionShape DownwardCapsuleShape = FCollisionShape::MakeSphere(DownwardSphereTraceRadius);
	
	// Точка начала каста
	float DepthOffset = 10.f;
	FVector DownwardStartLocation = ForwardHitResult.ImpactPoint - DepthOffset * ForwardHitResult.ImpactNormal;
	DownwardStartLocation.Z = CharacterBottom.Z + MaximumLedgeHeight + DownwardSphereTraceRadius;
	
	// Точка конца каста
	FVector DownwardEndLocation(DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBottom.Z);
	
	// Создание shape cast а по нашей форме и проверка на коллизии
	if (!GetWorld()->SweepSingleByChannel(DownwardHitResult, 
		DownwardStartLocation, 
		DownwardEndLocation, 
		FQuat::Identity, 
		ECC_Climbing,
		DownwardCapsuleShape,
		QueryParams))
	{
		return false;
	}
	
	// Overlap check (проверка на дополнительные препятствия в месте подъема игрока)
	FCollisionShape OverlapCapsuleShape = 
		FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), 
									 Capsule->GetScaledCapsuleHalfHeight());
	
	FVector OverlapLocation = DownwardHitResult.ImpactPoint - Capsule->GetScaledCapsuleHalfHeight() * FVector::UpVector;
	
	// Тест коллизии по каналам
	if (GetWorld()->OverlapAnyTestByProfile(OverlapLocation, 
		FQuat::Identity, 
		FName("Pawn"), 
		OverlapCapsuleShape, 
		QueryParams))
	{
		return false;
	}
	
	// Задание информации о выступе
	LedgeDescription.Location = DownwardHitResult.ImpactPoint;
	// Нормаль берем перевернутую (* - перегруженный оператор для компонентного произведения (просто умножает х * х | y * y | z * z))
	LedgeDescription.Rotation = (ForwardHitResult.ImpactNormal * FVector(-1.f, -1.f, 0.f)).ToOrientationRotator();
	
	return true;
}




