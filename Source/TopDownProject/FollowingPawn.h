#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FollowingPawn.generated.h"

UCLASS()
class TOPDOWNPROJECT_API AFollowingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFollowingPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
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

	void SelectTarget();

	void Move();

private:
	float MaxSpeed;

	float SatisfactionRad;

	float TimeToTarget;

	AActor *Target;
};
