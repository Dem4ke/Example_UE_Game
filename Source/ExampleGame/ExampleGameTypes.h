#pragma once

// Определяем удобное название для кастомной Line trace collision chanel
#define ECC_Climbing ECC_GameTraceChannel1
#define ECC_InteractionVolume ECC_GameTraceChannel2

const FName CollisionProfilePawn = FName("Pawn");
const FName CollisionProfilePawnInteractionVolume = FName("PawnInteractionVolume");
const FName DebugCategoryLedgeDetection = FName("LedgeDetection");
