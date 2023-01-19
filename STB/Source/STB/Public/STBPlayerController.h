// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <STB/UIManagerComponent.h>
#include "STBPlayerController.generated.h"


class UGameplay;
enum class ESTBGameMode : uint8;
class UScreen;

/**
 * 
 */
UCLASS()
class STB_API ASTBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	

	ASTBPlayerController();

	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void UpdateCameraManager(float DeltaSeconds) override;
	virtual void UpdateRotation(float DeltaTime) override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void CreateUI();

	UFUNCTION(BlueprintCallable)
	void BeginNewGame();

	UFUNCTION(BlueprintCallable)
	void ContinueGame();	

	/* 
	* Shows the UI @param State UI element, hiding the rest of them
	* @param State - The state which you want to show
	*/
	UFUNCTION(BlueprintCallable)
	void ShowUI(ESTBGameMode State);

	/* 
	* Returns the Gameplay variable
	*/
	UFUNCTION(BlueprintCallable)
	const UGameplay * GetGameplay() const;

	/* 
	* Returns the Player's current Vector2D location
	*/
	UFUNCTION(BlueprintCallable)
	const FVector2D& GetCurrentPlayerLocation() const;
	
	/* 
	* Returns the Vector2D location of the ball
	*/
	UFUNCTION(BlueprintCallable)
	FVector2D GetCurrentBallLocation() const;

	/*
	* Returns true if the PlayerLocation is within bounds of the CurrentBallLocation. See Gameplay->TryMove for more information.
	*/
	UFUNCTION(BlueprintCallable)
	bool TryMove();
	
protected:

	virtual void BeginPlay() override;	
	virtual void SetupInputComponent() override;
	
	/*
	 * Array of all the classes UI 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
		TMap<TSubclassOf<UScreen>, ESTBGameMode> UI_Classes;
	/* 
	* The class which is the intro UMG is
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> IntroClass;

	/*
	* The class which is the menu UMG is
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> MenuClass;

	/*
	* The class which is the settings UMG is
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> SettingsClass;

	/*
	* The class which is the playing UMG is
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> PlayingClass;

	/*
	* The class which is the game over UMG is
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> GameOverClass;

	/*
	* The class which is the outro UMG is
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UScreen> OutroClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MinOrbitRadius = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MaxOrbitRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MinPitch = -30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float MaxPitch = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float AxisScale = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float RadiusScale = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float PlayerLocationXRange = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float PlayerLocationYRange = 300.0f;
	
private:

	/*
	* Sets up the Screen, based on the @paramref State, @paramref Class and given it a @paramref Name
	* @param State - The Unique State for easy UI switching
	* @param Class - The Class of the UI
	* @param Name - The name of the UI
	*/
	void SetupScreen(ESTBGameMode State, TSubclassOf<UScreen> Class, FName Name);
	/*
	* Sets up the Screen, based on the TMap UI_Classes, calls SetupScreen's brother
	*/
	void SetupScreen();

	/*
	* Moves the Player on the X axis based on the @param Value
	* @param Value - The value of movement (Ranges from -1 to 1)
	*/
	UFUNCTION()
	void LeftRight(float Value);

	/*
	* Moves the Player on the Y axis based on the @param Value
	* @param Value - The value of movement (Ranges from -1 to 1)
	*/
	UFUNCTION()
	void UpDown(float Value);
	
	UFUNCTION()
	void TopButtonPress();

	UFUNCTION()
	void LeftButtonPress();

	UFUNCTION()
	void RightButtonPress();

	UFUNCTION()
	void BottomButtonPress();

	UPROPERTY(VisibleAnywhere)
	UGameplay * Gameplay;
	
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TArray<UScreen*> Widgets;

	UPROPERTY(VisibleAnywhere)
	FVector2D CurrentPlayerLocation;	
	
	FVector LastOrbitPawnLocation;
	FRotator LastOrbitPawnViewRotation;
	FVector OrbitPivot = FVector(0.0f, 0.0f, 100.0f);
	float OrbitRadius = 200.0f;
	/*
	 * The current state the game is in
	*/
	ESTBGameMode CurrentState;

	static const FString TopButtonActionName;
	static const FString LeftButtonActionName;
	static const FString RightButtonActionName;
	static const FString BottomButtonActionName;
	static const FString LeftStickXAxisName;
	static const FString LeftStickYAxisName;
	static const FString RightStickXAxisName;
	static const FString RightStickYAxisName;
	static const FString LeftTriggerAxisName;
	static const FString RightTriggerAxisName;
};
