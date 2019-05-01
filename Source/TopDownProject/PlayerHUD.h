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
	/** Sets default values for this object's properties */
	APlayerHUD();

	/** Draws the HUD */
	virtual void DrawHUD() override;

private:
	class APlayerChar *Player; /**< Pointer to the player character */

	UPROPERTY(EditAnywhere, Category = "Font")
		UFont *Font; /**< Font used by the HUD */

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<class UUserWidget> PlayerInfo; /**< Health bar to show the player's health */

	UPROPERTY()
		class UUserWidget *CurrentWidget; /**< The current widget to draw on the HUD */
};
