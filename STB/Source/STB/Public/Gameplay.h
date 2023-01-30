// Search for a Star 2023

#pragma once

#include "CoreMinimal.h"
#include "ProgressionData.h"
#include "ProMeshSquareActor.h"
#include "DependancyRemover.h"
#include "Gameplay.generated.h"

UCLASS()
class STB_API UGameplay : public UObject
{
	GENERATED_BODY()
	
public:

	UGameplay();

	UFUNCTION(BlueprintCallable)
	void SetLevels(AProgressionData * Data);

	/*
	* Resets the currentLevel and the currentLives
	*/
	UFUNCTION(BlueprintCallable)
	void StartNewGame();

	/*
	* Clears up the level and starts the next level
	*/
	UFUNCTION(BlueprintCallable)
	void NextLevel();	

	/*
	* Removes a life from the player
	*/
	UFUNCTION(BlueprintCallable)
	void ResetLevel();
	
	/*
	* Returns the Current Bounds of the Ball
	*/
	UFUNCTION(BlueprintCallable)
	const FBoxSphereBounds& GetCurrentBallBounds() const;	

	/*
	* Returns the Ball's current location
	*/
	UFUNCTION(BlueprintCallable)
	const FVector& GetBallLocation() const;
	
	UFUNCTION(BlueprintCallable)
		int GetTimeToImpact() const;
	/*
	* Returns the current level the player is on
	*/
	UFUNCTION(BlueprintCallable)
	int GetLevel() const;

	/* 
	* Returns the current lives of the player
	*/
	UFUNCTION(BlueprintCallable)
	int GetLives() const;

	/* 
	* Returns if the player won or not
	*/
	UFUNCTION(BlueprintCallable)
	bool GetWin() const;

	UFUNCTION(BlueprintCallable)
		AActor* GetOwner();
	UFUNCTION(BlueprintCallable)
		void SetOwner(AActor* NewOwner);
	/* 
	* Returns true if @param PlayerGuess is within range to @param BallLocation2D
	* @param PlayerGuess - The vector in which the Player's location is
	* @param BallLocation2D - The vector where the ball is located
	*/
	UFUNCTION(BlueprintCallable)
	bool TryMove(const FVector2D& PlayerGuess, const FVector2D& BallLocation2D);
	/* 
	* Randomises the ball's location based on the bounds
	*/
	UFUNCTION(BlueprintCallable)
	void ChooseRandomBallLocation();

	UPROPERTY(VisibleAnywhere)
		AProMeshSquareActor* ActorToShow;
	
protected:
	/*
	 * The amount of lives the player starts out with
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StartingLives = 3;
	
private:

	/*
	* The levels
	*/
	UPROPERTY(VisibleAnywhere)
		AProgressionData * Levels;

	/*
	 * Bounds for the ball to move around
	*/
	UPROPERTY(VisibleAnywhere)
		FBoxSphereBounds CurrentBallBounds; 

	/*
	 * The location of the ball the player needs to go near
	*/
	UPROPERTY(VisibleAnywhere)
		FVector BallLocation;

	/*
	 * How long the wall moves for before a check is made
	*/
	UPROPERTY(VisibleAnywhere)
		float CurrentTimeUntilImpact;

	/*
	 * The distance the ball has to be around the BallLocation
	*/
	UPROPERTY(VisibleAnywhere)
		float CurrentRequiredDistance;
	/*
	 * The current level the game is at
	*/
	UPROPERTY(VisibleAnywhere)
		int CurrentLevel;

	/*
	 * The current lives the player has 
	*/
	UPROPERTY(VisibleAnywhere)
		int CurrentLives;
	
	/*
	 * If the player has won the level
	*/
	UPROPERTY(VisibleAnywhere)
		bool bWin;	

	AActor* Owner;
};
