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
	Player = Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Font == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<UFont>HUDFont(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
		Font = HUDFont.Object;
	}
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	if (Player)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), Health);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
	else
	{
		Player = Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	}
}

#undef LOCTEXT_NAMESPACE