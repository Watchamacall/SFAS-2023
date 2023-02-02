// Search for a Star 2023

#include "Screens/PlayingScreen.h"

#include "Gameplay.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UPlayingScreen::NativeConstruct()
{
	Super::NativeConstruct();

	for(int Count = 0; Count < Texts.Num(); ++Count)
	{
		if(Texts[Count]->GetName().Contains("Level"))
		{
			LevelTextFormat = Texts[Count]->Text;
			LevelTextIndex = Count;
		}
		else if(Texts[Count]->GetName().Contains("Lives"))
		{
			LivesTextFormat = Texts[Count]->Text;
			LivesTextIndex = Count;
		}
		else if(Texts[Count]->GetName().Contains("Prompt"))
		{
			PromptTextIndex = Count;
		}
		else if (Texts[Count]->GetName().Contains("Time"))
		{
			TimeRemainingFormat = Texts[Count]->Text;
			TimeRemainingIndex = Count;
		}
	}

	for(int Count = 0; Count < Images.Num(); ++Count)
	{
		if(Images[Count]->GetName().Contains("Target"))
		{
			TargetImageIndex = Count;
		}
		else if(Images[Count]->GetName().Contains("Guess"))
		{
			GuessImageIndex = Count;
		} 
	}	
	
	SetLevel(1);
	SetLives(0);
	SetTimeRemaining(0.f);
	
	PlayingState = EPlayingState::Guessing;
}

void UPlayingScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(IsValid(PlayerController))
	{
		if(PlayingState == EPlayingState::Showing) //When pressing Select where you wanna place the Guess Marker
		{
			if(TargetImageIndex >= 0 && TargetImageIndex < Images.Num())
			{
				Images[TargetImageIndex]->SetOpacity(FMath::Clamp(Images[TargetImageIndex]->ColorAndOpacity.A + InDeltaTime, 0.0f, 1.0f)); //Slowly bring in the TargetImage

				if(GuessImageIndex >= 0 && GuessImageIndex < Images.Num())
				{
					Images[GuessImageIndex]->SetOpacity(1.0f - Images[TargetImageIndex]->ColorAndOpacity.A);
				}
				
				if(Images[TargetImageIndex]->ColorAndOpacity.A >= 1.0f)
				{
					PlayingState = EPlayingState::Shown;
				}
			}
		}

		if(GuessImageIndex >= 0 && GuessImageIndex < Images.Num())
		{
			const FVector2D GuessLocation = PlayerController->GetCurrentPlayerLocation();
			if(UCanvasPanelSlot* GuessSlot = Cast<UCanvasPanelSlot>(Images[GuessImageIndex]->Slot))
			{
				GuessSlot->SetPosition(GuessLocation);
			}
		}

		if (PlayingState == EPlayingState::Guessing)
		{
			if (TimeRemainingIndex >= 0 && TimeRemainingIndex < Texts.Num())
			{
				SetTimeRemaining(TimeTillWall -= InDeltaTime);
				if (TimeTillWall <= 0) //If the wall is close to the player's mesh
				{
					const bool bWon = PlayerController->TryMove();
					if (!bWon)
					{
						SetLives(PlayerController->GetGameplay()->GetLives());
					}
					DoReveal(bWon); //Reveal the case of who won
				}
			}
		}

	}
}

void UPlayingScreen::Select_Implementation()
{
	if (IsValid(PlayerController))
	{
		if (PlayingState == EPlayingState::Guessing)
		{
			FVector2D GuessPosition = Cast<UCanvasPanelSlot>(Images[GuessImageIndex]->Slot)->GetPosition();
			FVector WorldLocation;
			FVector WorldDirection;

			//Since the GuessPosition origin is the center of the screen whilst the Deproject origin is top left, add the Viewport * .5f to center it and get accurate direction
			int32 ViewportSizeX, ViewportSizeY;
			PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

			//TODO: Fix the Direction the ray goes, currently moves too far in all directions
			if (PlayerController->DeprojectScreenPositionToWorld(GuessPosition.X + (ViewportSizeX * .5f), GuessPosition.Y + (ViewportSizeY * .5f), WorldLocation, WorldDirection))
			{
				FHitResult Hit;
				DrawDebugLine(GetWorld(), WorldLocation, WorldLocation + (WorldDirection * LinecastDistance), FColor::Green, false, 10.f);

				if (GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, WorldLocation + (WorldDirection * LinecastDistance), ECollisionChannel::ECC_WorldDynamic))
				{
					//TODO: Add a variable which changes here, maybe just an int that relates to the LeftRight and UpDown of the PlayerController and allows for the movement of the Vertex (In the ProGenMeshBase Class) until Release where that variable is changed to -1 or something, saves having to rework this entire thing

					FName ComponentName = Hit.GetComponent()->GetFName();
					UE_LOG(LogTemp, Display, TEXT("Collider is %s"), *ComponentName.ToString());
					if (UStaticVertexCollider* Collider = Cast<UStaticVertexCollider>(Hit.GetComponent()))
					{
						PlayerController->SetSelectedVertex(Collider->VertexNo);
					}

				}
			}
		}
		else if (PlayingState == EPlayingState::Shown) //After the whole part of the game
		{
			Reset();
		}
	}
	//We are not guessing anymore so this isn't needed
	
	//if(PlayingState == EPlayingState::Guessing)
	//{
	//	if(IsValid(PlayerController))
	//	{
	//		const bool bCorrect = PlayerController->TryMove();
	//		if(!bCorrect)
	//		{
	//			if(const UGameplay* Gameplay = PlayerController->GetGameplay())
	//			{
	//				const int Lives = Gameplay->GetLives();
	//				SetLives(Lives);
	//			}					
	//		}

	//		DoReveal(bCorrect);
	//	}
	//}
	//else if(PlayingState == EPlayingState::Shown)
	//{
	//	Reset();
	//}
}

void UPlayingScreen::Show(bool bShow)
{
	Super::Show(bShow);

	if(bShow)
	{
		Reset();
	}
}

void UPlayingScreen::SetLevel(int Level)
{
	if(LevelTextIndex >= 0 && LevelTextIndex < Texts.Num())
	{
		Texts[LevelTextIndex]->SetText(FText::Format(LevelTextFormat, Level));
	}
}

void UPlayingScreen::SetLives(int Lives)
{
	if(LivesTextIndex >= 0 && LivesTextIndex < Texts.Num())
	{
		Texts[LivesTextIndex]->SetText(FText::Format(LivesTextFormat, Lives));
	}
}
void UPlayingScreen::SetTimeRemaining(float TimeRemaining)
{
	if (TimeRemainingIndex >= 0 && TimeRemainingIndex < Texts.Num())
	{
		Texts[TimeRemainingIndex]->SetText(FText::Format(TimeRemainingFormat, FMath::Clamp((int)TimeRemaining, 0, TimeRemaining)));
	}
}

void UPlayingScreen::ShowLevel(bool bShow)
{
	if(LevelTextIndex >= 0 && LevelTextIndex < Texts.Num())
	{
		Texts[LevelTextIndex]->SetOpacity(bShow ? 1.0f : 0.0f);
	}
}

void UPlayingScreen::ShowLives(bool bShow)
{
	if(LivesTextIndex >= 0 && LivesTextIndex < Texts.Num())
	{
		Texts[LivesTextIndex]->SetOpacity(bShow ? 1.0f : 0.0f);
	}
}

void UPlayingScreen::ShowPrompt(bool bShow)
{
	if(PromptTextIndex >= 0 && PromptTextIndex < Texts.Num())
	{
		Texts[PromptTextIndex]->SetOpacity(bShow ? 1.0f : 0.0f);
	}
}

void UPlayingScreen::SetBallLocation()
{
	if(TargetImageIndex >= 0 && TargetImageIndex < Images.Num())
	{
		const FVector2D BallLocation = PlayerController->GetCurrentBallLocation();
		if(UCanvasPanelSlot* TargetSlot = Cast<UCanvasPanelSlot>(Images[TargetImageIndex]->Slot))
		{
			TargetSlot->SetPosition(BallLocation);
		}

		Images[TargetImageIndex]->SetOpacity(0.0f);
	}
}

void UPlayingScreen::DoReveal(const bool bLastGuessCorrect)
{
	//TODO: Update this function to show how correct the Player was
	if(TargetImageIndex >= 0 && TargetImageIndex < Images.Num())
	{
		Images[TargetImageIndex]->SetBrushTintColor(bLastGuessCorrect ? WinColor : LoseColor);
	}
	
	ShowLevel(false);
	ShowLives(false);
	ShowPrompt(true);
	PlayingState = EPlayingState::Showing;
}

void UPlayingScreen::Reset()
{
	if(TargetImageIndex >= 0 && TargetImageIndex < Images.Num())
	{
		Images[TargetImageIndex]->SetOpacity(0.0f);
	}

	if(GuessImageIndex >= 0 && GuessImageIndex < Images.Num())
	{
		Images[GuessImageIndex]->SetOpacity(1.0f);
	}	

	if(IsValid(PlayerController))
	{
		PlayerController->ContinueGame();
		
		if(const UGameplay* Gameplay = PlayerController->GetGameplay())
		{
			const int Level = Gameplay->GetLevel() + 1;
			SetLevel(Level);
			TimeTillWall = Gameplay->GetTimeToImpact();
		}
	}

	ShowLevel(true);
	ShowLives(true);
	ShowPrompt(false);
	SetBallLocation();
	PlayingState = EPlayingState::Guessing;
}

