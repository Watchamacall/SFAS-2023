// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <STB/UIManagerComponent.h>
#include "ProMeshSquareActor.h"
#include "Wall.h"
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



#pragma region Override Functions
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void UpdateCameraManager(float DeltaSeconds) override;
	virtual void UpdateRotation(float DeltaTime) override;
	virtual void Tick(float DeltaSeconds) override;
#pragma endregion


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
	 * Creates the UI
	*/
	UFUNCTION(BlueprintCallable)
		void CreateUI();
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
	 * Returns the Single Verticies Min and Max values it can move
	*/
	UFUNCTION(BlueprintCallable)
		FXYMinMax GetSingleVertMinMax();
	/*
	 * Returms the Static Mesh the Vertex Colliders will use
	*/
	UFUNCTION(BlueprintCallable)
		UStaticMesh* GetColliderMesh();
	/*
	* Returns true if the PlayerLocation is within bounds of the CurrentBallLocation. See Gameplay->TryMove for more information.
	*/
	UFUNCTION(BlueprintCallable)
		bool TryMove();


	
protected:
#pragma region Override Functions
	virtual void BeginPlay() override;	
	virtual void SetupInputComponent() override;
#pragma endregion
	/*
	 * Array of all the classes UI 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI|General")
		TMap<TSubclassOf<UScreen>, ESTBGameMode> UI_Classes;
	
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

	/*
	 * The Minimum and Maximum XY values for the Cursor
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Cursor")
		FVector2D CursorXYMin = FVector2D::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Cursor")
		FVector2D CursorXYMax = FVector2D::ZeroVector;
	/*
	 * The Single Vertex's X & Y Movement Minimum **Based on where it is originally located** (Left & Down)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement Options|Single Vertex")
		FVector2D SingleVertexXYMovementMin = FVector2D(-200.f, 0.f);

	/*
	 * The Single Vertex's X & Y Movement Maximum **Based on where it is originally located** (Right & Up)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement Options|Single Vertex")
		FVector2D SingleVertexXYMovementMax = FVector2D(200.f, 200.f);

	/*
	 * The Whole Object's X & Y Movement Minimum (Left & Down)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement Options|Whole Object")
		FVector2D WholeObjectXYMovementMin = FVector2D(-200.f, 0.f);

	/*
	 * The Whole Object's X & Y Movement Maximum (Right & Up)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement Options|Whole Object")
		FVector2D WholeObjectXYMovementMax = FVector2D(200.f, 200.f);

	/*
	 * The Object which will be used to show the player
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Generated Mesh")
		AProMeshSquareActor* ActorToShow;

	/*
	 * Where the Generated Mesh will call it's origin
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generated Mesh")
		FVector ActorToShowOrigin = FVector::ZeroVector;
	/*
	 * What Static Mesh the Vertex Colliders will use
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generated Mesh")
		UStaticMesh* ColliderMesh;
	/*
	 * The speed the whole object moves at 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement Options|Whole Object")
		float WholeObjectMoveSpeed = 5.f;
	/*
	 * The speed a single vertex moves at
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement Options|Single Vertex")
		float VertexMoveSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
		AWall* WallComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wall")
		FVector WallInitialVector = FVector::ZeroVector;


	
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
#pragma region Left Axis Functions
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
#pragma endregion
	/*
	 * Updates a Vertex from the ActorToShow variable and sends it across to the object
	*/
	void UpdateVertex(char VectorName, int8 VertexIndex, float Value);
#pragma region Button Functions
	/*
	 * The Top Button press (Y / Triangle)
	*/
	UFUNCTION()
	void TopButtonPress();

	/*
	 * The Left Button press (X / Square)
	*/
	UFUNCTION()
	void LeftButtonPress();

	/*
	 * The Right Button press (B / Circle)
	*/
	UFUNCTION()
	void RightButtonPress();

	/*
	 * The Bottom Button press (A / Cross)
	*/
	UFUNCTION()
	void BottomButtonPress();

	/*
	 * The Top Button Release (Y / Triangle)
	*/
	UFUNCTION()
	void TopButtonRelease();

	/*
	 * The Left Button Release (X / Square)
	*/
	UFUNCTION()
	void LeftButtonRelease();

	/*
	 * The Right Button Release (B / Circle)
	*/
	UFUNCTION()
	void RightButtonRelease();

	/*
	 * The Bottom Button Release (A / Cross)
	*/
	UFUNCTION()
	void BottomButtonRelease();
#pragma endregion
	/*
	 * The class which holds all the logic for the game 
	*/
	UPROPERTY(VisibleAnywhere)
	UGameplay * Gameplay;
	
	/*
	 * Array holding the locations in memory where the Widgets are
	*/
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TArray<UScreen*> Widgets;

	/*
	 * The current location in the game world where the player is
	*/
	UPROPERTY(VisibleAnywhere)
	FVector2D CurrentPlayerLocation;	
	
#pragma region Orbit Variables
	UPROPERTY(EditAnywhere, Category="Camera")
		FVector LastOrbitPawnLocation;
	UPROPERTY(EditAnywhere, Category = "Camera")
		FRotator LastOrbitPawnViewRotation;
	FVector OrbitPivot = FVector(0.0f, 0.0f, 100.0f);
	float OrbitRadius = 200.0f;
#pragma endregion

	float CurrentTimeWallInMotion = 0.f;


#pragma region ButtonBooleans
	bool TopButton;
	bool LeftButton;
	bool RightButton;
	bool BottomButton;
#pragma endregion
	/*
	 * The current state the game is in
	*/
	ESTBGameMode CurrentState;

#pragma region Input Names
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
#pragma endregion
};
