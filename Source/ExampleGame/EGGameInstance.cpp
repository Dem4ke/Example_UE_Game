// Fill out your copyright notice in the Description page of Project Settings.


#include "EGGameInstance.h"

// Метод для возможности работы макроса Exec в различных классах (для работы с консолью UE Editor)
bool UEGGameInstance::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	bool bResult = Super::ProcessConsoleExec(Cmd, Ar, Executor);
	
	// Если консольная команда ничего не выдала
	if (!bResult)
	{
		// Нужно попробовать вызвать ее у всех подсистем
		for (auto& Subsystem : GetSubsystemArrayCopy<UGameInstanceSubsystem>())
		{
			// Объединение результата со всеми подсистемами
			bResult |= Subsystem->ProcessConsoleExec(Cmd, Ar, Executor);
		}
	}
	
	return bResult;
}
