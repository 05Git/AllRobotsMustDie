#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * PlayerHUD
 * Heads up display used to display informtation to the player
 */
UCLASS()
class TOPDOWNPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	APlayerHUD();

	virtual void DrawHUD() override;

private:
	class APlayerChar *Player;

	UPROPERTY(EditAnywhere, Category = "Font")
		UFont *Font;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<class UUserWidget> Health;

	UPROPERTY()
		class UUserWidget *CurrentWidget;
};
