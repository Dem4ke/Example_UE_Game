// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugSubsystem.h"

bool UDebugSubsystem::IsCategoryEnabled(const FName& CategoryName) const
{
	const bool* bIsEnabled = DebugCategoriesMap.Find(CategoryName);
	
	if (bIsEnabled)
	{
		return *bIsEnabled;
	}
	
	return false;
}

void UDebugSubsystem::EnableDebugCategory(const FName& CategoryName, bool bIsEnabled)
{
	DebugCategoriesMap.FindOrAdd(CategoryName);
	DebugCategoriesMap[CategoryName] = bIsEnabled;
}
