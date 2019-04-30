#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TDPGameInstance.generated.h"

/**
 * TDPGameInstance
 * Stores and sets values for the game
 */
UCLASS()
class TOPDOWNPROJECT_API UTDPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	/** Sets default values for this object's properties */
	UTDPGameInstance();

	/** Returns Volume */
	UFUNCTION(BlueprintCallable, Category = "Volume")
		float GetVolume();

	/** Sets Volume */
	UFUNCTION(BlueprintCallable, Category = "Volume")
		void SetVolume(float Volume);

private:
	float Volume; /**< Volume determines the volume multiplier used when playing sound files */
};
