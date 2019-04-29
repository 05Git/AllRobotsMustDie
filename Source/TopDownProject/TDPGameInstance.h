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
	UTDPGameInstance();

	UFUNCTION(BlueprintCallable, Category = "Volume")
		float GetVolume();

	UFUNCTION(BlueprintCallable, Category = "Volume")
		void SetVolume(float Volume);

private:
	float Volume;
};
