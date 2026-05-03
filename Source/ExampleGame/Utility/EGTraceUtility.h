#pragma once

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
		const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& ResponseParams = FCollisionResponseParams::DefaultResponseParam,
		bool bDrawDebug = false,
		float LifeTime = -1.0f,
		FColor TraceColor = FColor::Black,
		FColor HitColor = FColor::Red);
	
	bool SweepSphereSingleByChanel(
		const UWorld* World,
		struct FHitResult& OutHit,
		const FVector& Start,
		const FVector& End,
		float Radius,
		ECollisionChannel TraceChannel,
		const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& ResponseParams = FCollisionResponseParams::DefaultResponseParam,
		bool bDrawDebug = false,
		float LifeTime = -1.0f,
		FColor TraceColor = FColor::Black,
		FColor HitColor = FColor::Red);
	
	bool OverlapCapsuleAnyByProfile(
		const UWorld* World,
		const FVector& Position,
		float CapsuleRadius,
		float CapsuleHalfHeight,
		const FQuat& Rot,
		FName ProfileName,
		const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam,
		bool bDrawDebug = false,
		float LifeTime = -1.0f,
		FColor HitColor = FColor::Red);
} // namespace EGTraceUtility