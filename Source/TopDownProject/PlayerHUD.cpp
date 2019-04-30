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
	/** Casts Player to player pawn */
	Player = Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	/** Checks if Font is a nullptr */
	if (Font == nullptr)
	{
		/** Sets Font to RobotoDistanceField */
		static ConstructorHelpers::FObjectFinder<UFont>HUDFont(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
		Font = HUDFont.Object;
	}
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	/** Checks if Player is active */
	if (Player)
	{
		/** Sets CurrentWidget to the Health widget */
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), Health);
		/** Checks if CurrentWidget is active */
		if (CurrentWidget)
		{
			/** Adds CurrentWidget to viewport */
			CurrentWidget->AddToViewport();
		}
	}
	else
	{
		/** Casts Player to player pawn */
		Player = Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	}
}

#undef LOCTEXT_NAMESPACE