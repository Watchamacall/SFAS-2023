// Search for a Star 2023


#include "STBGameModeBase.h"
#include "STBGameModes.h"
#include "STBPawn.h"
#include "STBPlayerController.h"
#include "Kismet/GameplayStatics.h"

ASTBGameModeBase::ASTBGameModeBase()
{
	DefaultPawnClass = ASTBPawn::StaticClass();
	PlayerControllerClass = ASTBPlayerController::StaticClass();
}

void ASTBGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SetupUI();
}

void ASTBGameModeBase::SetupUI() const
{
	if(ASTBPlayerController* Controller = Cast<ASTBPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		Controller->CreateUI();
	}
}
