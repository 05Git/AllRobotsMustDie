#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharController.generated.h"

/**
 * PlayerCharController
 * Controller for PlayerChar
 */
UCLASS()
class TOPDOWNPROJECT_API APlayerCharController : public APlayerController
{
	GENERATED_BODY()

public:
	/** Sets default values for this character's properties */
	APlayerCharController();

};
