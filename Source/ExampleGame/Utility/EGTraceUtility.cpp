#include "EGTraceUtility.h"

#include "DrawDebugHelpers.h"

namespace EGTraceUtility
{
	bool SweepCapsuleSingleByChanel(
		const UWorld* World, 
		struct FHitResult& OutHit, 
		const FVector& Start,
		const FVector& End, 
		float CapsuleRadius, 
		float CapsuleHalfHeight, 
		const FQuat& Rot,
		ECollisionChannel TraceChannel, 
		const FCollisionQueryParams& Params,
		const FCollisionResponseParams& ResponseParams, 
		bool bDrawDebug, 
		float LifeTime, 
		FColor TraceColor,
		FColor HitColor)
	{
		bool bResult = false;
		
		// Стуктура для проверки коллизии
		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
		
		// Расчет коллизии
		bResult = World->SweepSingleByChannel(
			OutHit, 
			Start,
			End,
			Rot,
			TraceChannel,
			CollisionShape,
			Params,
			ResponseParams);
		
#if ENABLE_DRAW_DEBUG
		// Если необходима отладочная отрисовка
		if (bDrawDebug)
		{
			// Отладочная отрисовка капсул начала и конца поиска, также линии, их соединяющей
			DrawDebugCapsule(World, 
				Start, 
				CapsuleHalfHeight, 
				CapsuleRadius, 
				FQuat::Identity,
				TraceColor,
				false,
				LifeTime);
	
			DrawDebugCapsule(World, 
				End, 
				CapsuleHalfHeight, 
				CapsuleRadius, 
				FQuat::Identity,
				TraceColor,
				false,
				LifeTime);
	
			DrawDebugLine(World, 
				Start, 
				End, 
				TraceColor, 
				false, 
				LifeTime);
			
			// Если коллизия была найдена
			if (bResult)
			{
				// Отрисовка капсулы коллизии и точки соприкосновения 
				// Отладочная отрисовка капсулы
				DrawDebugCapsule(World, 
					OutHit.Location, 
					CapsuleHalfHeight, 
					CapsuleRadius, 
					FQuat::Identity,
					HitColor,
					false,
					LifeTime);
	
				DrawDebugPoint(World,
					OutHit.ImpactPoint,
					20.f,
					HitColor,
					false,
					LifeTime);
			}
		}
#endif 
		
		return bResult;
	}

	bool SweepSphereSingleByChanel(
		const UWorld* World, 
		struct FHitResult& OutHit, 
		const FVector& Start,
		const FVector& End, 
		float Radius, 
		ECollisionChannel TraceChannel, 
		const FCollisionQueryParams& Params,
		const FCollisionResponseParams& ResponseParams, 
		bool bDrawDebug, 
		float LifeTime, 
		FColor TraceColor,
		FColor HitColor)
	{
		bool bResult = false;
		
		// Стуктура для проверки коллизии
		FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
		
		// Расчет коллизии
		bResult = World->SweepSingleByChannel(
			OutHit, 
			Start,
			End,
			FQuat::Identity,
			TraceChannel,
			CollisionShape,
			Params,
			ResponseParams);
		
#if ENABLE_DRAW_DEBUG
		// Если необходима отладочная отрисовка
		if (bDrawDebug)
		{
			// Создание и отрисовка дебажной отрисовки
			FVector DebugDrawCapsuleLocation = (Start + End) * 0.5f;
			FVector TraceVector = End - Start;
			float DebugDrawCapsuleHalfHeight = TraceVector.Size() * 0.5f;
			
			// Создание матрицы вращения относительно оси базисного вектора по Z
			FQuat DebugCapsuleRotation = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();
			
			DrawDebugCapsule(World,
				DebugDrawCapsuleLocation,
				DebugDrawCapsuleHalfHeight,
				Radius,
				DebugCapsuleRotation,
				TraceColor,
				false,
				LifeTime);
	
			// Если была найдена точка коллизии
			if (bResult)
			{
				// Отрисовка сферы и точки пересечения
				DrawDebugSphere(World,
					OutHit.Location,
					Radius,
					32,
					HitColor,
					false,
					LifeTime);
	
				DrawDebugPoint(World,
					OutHit.ImpactPoint,
					20.f,
					HitColor,
					false,
					LifeTime);
			}
		}
#endif
		
		return bResult;
	}

	bool OverlapCapsuleAnyByProfile(
		const UWorld* World, 
		const FVector& Position, 
		float CapsuleRadius,
		float CapsuleHalfHeight, 
		const FQuat& Rot, 
		FName ProfileName, 
		const FCollisionQueryParams& Params,
		bool bDrawDebug, 
		float LifeTime, 
		FColor HitColor)
	{
		bool bResult = false;
		
		// Стуктура для проверки коллизии
		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
		
		bResult = World->OverlapAnyTestByProfile(
			Position,
			Rot,
			ProfileName,
			CollisionShape,
			Params);
		
#if ENABLE_DRAW_DEBUG
		if (bDrawDebug && bResult)
		{
			DrawDebugCapsule(World,
				Position,
				CapsuleHalfHeight,
				CapsuleRadius,
				Rot,
				HitColor,
				false,
				LifeTime);
		}
#endif
		
		return bResult;
	}
} // namespace EGTraceUtility
