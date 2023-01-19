// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class STB_API UUIBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool pauseWorld = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ACharacter* player;
};
