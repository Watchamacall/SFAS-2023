// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Screen.h"
#include "WinScreen.generated.h"

/**
 * 
 */
UCLASS()
class STB_API UWinScreen : public UScreen
{
	GENERATED_BODY()

public:

	virtual void Select_Implementation() override;

};
