#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FollowActor.generated.h"

/**
* FollowActor
* An actor that follows the player around and hovers over the nearest enemy
*/
UCLASS()
class TOPDOWNPROJECT_API AFollowActor : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Sets default values for this object's properties */
	AFollowActor();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Pawn")
		class UBehaviorTree *BTree; /**< Behaviour Tree used by this actor */

	/** Returns MaxSpeed */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		float GetMaxSpeed();

	/** Returns SatisfactionRad */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		float GetSatisfactionRad();

	/** Returns TimeToTarget */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		float GetTimeToTarget();

	/** Returns Target */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		AActor *GetTarget();

	/** Sets Target */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		void SetTarget(AActor *Actor);
	
private:
	float MaxSpeed;

	float SatisfactionRad;

	float TimeToTarget;

	AActor *Target;
};
