// Fill out your copyright notice in the Description page of Project Settings.


#include "Screens/WinScreen.h"
#include "STB/STBGameModes.h"


void UWinScreen::Select_Implementation()
{
	if (IsValid(PlayerController))
	{
		PlayerController->ShowUI(ESTBGameMode::MainMenu);
	}
}