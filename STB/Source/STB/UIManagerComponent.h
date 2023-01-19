// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "UIBaseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UIManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STB_API UUIManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUIManagerComponent();

	/*
	 * The classes of the UI Elements
	*/
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UUIBaseWidget>> UIElements;
	/*
	 * The pointers to the instances in memory 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<UUIBaseWidget*> UIInstances;
	/*
	 * TMap of the object and what it's parent is
	*/
	TMap< UUIBaseWidget*, UUIBaseWidget* > childParentMap;

	/*
	 * The current widget being shown
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UUIBaseWidget* curWidget;
	/*
	 * The previous widget that was shown
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UUIBaseWidget* prevWidget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/*
	 * Sets the newWidget to be shown
	*/
	UFUNCTION(BlueprintCallable)
	void SetForegroundWidget(UUIBaseWidget* newWidget);
	/*
	 * Initial setup of the UI
	*/
	UFUNCTION(BlueprintCallable)
	void UISetup();
	/*
	 * Returns the pointer location of the inputted className
	*/
	UFUNCTION(BlueprintCallable)
		UUIBaseWidget* GetUIInstanceFromClass(TSubclassOf<UUIBaseWidget> className);

	void SetChildParentMap();
	TSubclassOf<UUIBaseWidget> GetParentClass(TSubclassOf<UUIBaseWidget> childWidget);
	TArray<UUIBaseWidget*> GetParentClasses(UUIBaseWidget* potentialChild);
	virtual bool IsASubclassOf(UClass* parentClass, UClass* childClass);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
		
};
