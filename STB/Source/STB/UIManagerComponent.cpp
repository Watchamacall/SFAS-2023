#include "UIManagerComponent.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerComponent.h"

// Sets default values for this component's properties
UUIManagerComponent::UUIManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UUIManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UISetup();
}


// Called every frame
void UUIManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UUIManagerComponent::SetForegroundWidget(UUIBaseWidget* newWidget)
{
	/* 
	 * What I wanna do in this function is:
	 * Find out if the current element is 
	*/
	if (newWidget == nullptr)
	{
		return;
	}
	prevWidget = curWidget;
	TArray<UUIBaseWidget*> parentClasses = GetParentClasses(newWidget);
	int floatOrder = 0; 
	for (size_t i = 0; i < UIInstances.Num(); i++)
	{
		if (newWidget == UIInstances[i])
		{
			UIInstances[i]->AddToViewport(floatOrder);
			floatOrder--;
		}
		else if (parentClasses.Contains(UIInstances[i]))
		{
			if (!UIInstances[i]->IsInViewport())
			{
				UIInstances[i]->AddToViewport(floatOrder);
				floatOrder--;
			}
		}
		else if (UIInstances[i]->IsInViewport())
		{
			UIInstances[i]->RemoveFromViewport();
			UIInstances[i]->RemoveFromParent();
		}

	}

	auto player = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UGameplayStatics::SetGamePaused(GetWorld(), newWidget->pauseWorld);

	if (newWidget->pauseWorld)
	{
		player->SetInputMode(FInputModeGameAndUI());
		player->bShowMouseCursor = true;
	}
	else
	{
		player->SetInputMode(FInputModeGameOnly());
		player->bShowMouseCursor = false;
	}

	curWidget = newWidget;
}


void UUIManagerComponent::UISetup()
{
	if (UIInstances.Num() > 0) //Safety check to ensure that we're not wasting resources with creating the widgets again
	{
		return;
	}

	for (size_t i = 0; i < UIElements.Num(); i++)
	{
		if (UIElements[i])
		{
			UIInstances.Add(CreateWidget<UUIBaseWidget>(GetWorld(), UIElements[i], UIElements[i]->GetFName()));
			UIInstances[i]->player = (ACharacter*)GetOwner();
		}
	}

	SetChildParentMap();
}
UUIBaseWidget* UUIManagerComponent::GetUIInstanceFromClass(TSubclassOf<UUIBaseWidget> className)
{
	for (size_t i = 0; i < UIInstances.Num(); i++)
	{
		if (className == UIInstances[i]->GetClass())
			return UIInstances[i];
	}
	return nullptr;
}

void UUIManagerComponent::SetChildParentMap()
{
	for (size_t i = 0; i < UIElements.Num(); i++)
	{
		GetParentClass(UIElements[i]) == nullptr ? childParentMap.Add(
			UIInstances[UIElements.IndexOfByKey(UIElements[i])],
			nullptr) : childParentMap.Add(
				UIInstances[UIElements.IndexOfByKey(UIElements[i])],
				UIInstances[UIElements.IndexOfByKey(GetParentClass(UIElements[i]))]);
		
	}
}

TSubclassOf<UUIBaseWidget> UUIManagerComponent::GetParentClass(TSubclassOf<UUIBaseWidget> childWidget)
{
	for (size_t i = 0; i < UIElements.Num(); i++)
	{
		if (UIElements[i])
		{
			if (IsASubclassOf(UIElements[i]->GetClass(), childWidget->GetClass()))
			{
				return UIElements[i];
			}
		}
	}
	return nullptr;
}

TArray<UUIBaseWidget*> UUIManagerComponent::GetParentClasses(UUIBaseWidget* potentialChild)
{
	if (childParentMap.Num() < 1)
	{
		return TArray<UUIBaseWidget*>();
	}
	TArray<UUIBaseWidget*> returnArray = TArray<UUIBaseWidget*>();

	if (childParentMap.Contains(potentialChild))
	{
		UUIBaseWidget* parentWidget = *childParentMap.Find(potentialChild);

		if (parentWidget != nullptr)
		{
			returnArray.Add(parentWidget);

			auto parent = GetParentClasses(parentWidget);
			for (size_t i = 0; i < parent.Num(); i++)
			{
				if (!returnArray.Contains(parent[i]))
				{
					returnArray.Add(parent[i]);
				}
			}
		}
	}

	return returnArray;
}

bool UUIManagerComponent::IsASubclassOf(UClass* parentClass, UClass* childClass)
{
	return parentClass->GetClass() == childClass->GetSuperClass() ? true : false;
}

