#include "PlayerHUD.h"
#include "PlayerChar.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#define LOCTEXT_NAMESPACE "HUD"

APlayerHUD::APlayerHUD()
{
	Player = Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)); // Casts Player to player pawn
	// Checks if Font is a nullptr
	if (Font == nullptr)
	{
		// Sets Font to RobotoDistanceField
		static ConstructorHelpers::FObjectFinder<UFont>HUDFont(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
		Font = HUDFont.Object;
	}
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	// Checks if Player is active
	if (Player)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), Health); // Sets CurrentWidget to the Health widget
		// Checks if CurrentWidget is active
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport(); // Adds CurrentWidget to viewport
		}
	}
	else
	{
		Player = Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)); // Casts Player to player pawn
	}
}

#undef LOCTEXT_NAMESPACE