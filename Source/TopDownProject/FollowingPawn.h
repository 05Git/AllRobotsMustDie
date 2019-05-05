#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FollowingPawn.generated.h"

UCLASS()
class TOPDOWNPROJECT_API AFollowingPawn : public APawn
{
	GENERATED_BODY()

public:
	/** Sets default values for this pawn's properties */
	AFollowingPawn();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns MaxSpeed */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		float GetMaxSpeed();

	/** Sets MaxSpeed */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		void SetMaxSpeed(float Speed);

	/** Returns SatisfactionRad */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		float GetSatisfactionRad();

	/** Sets SatisfactionRad */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		void SetSatisfactionRad(float Radius);

	/** Returns TimeToTarget */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		float GetTimeToTarget();

	/** Sets TimeToTarget */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		void SetTimeToTarget(float Time);

	/** Returns OverlappingEnemy */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		bool IsOverlappingEnemy();

	/** Sets OverlappingEnemy */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		void SetIsOverlappingEnemy(bool Overlap);
	
	/** Returns Attacking */
	UFUNCTION(BlueprintCallable, Category = "Attack")
		bool IsAttacking();

	/** Sets Attacking */
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void SetIsAttacking(bool Attack);

	/** Plays ExplosionSound */
	UFUNCTION(BlueprintCallable, Category = "Attack")
		void PlayExplosionSound();

	/** Returns Target */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		AActor *GetTarget();

	/** Sets Target */
	UFUNCTION(BlueprintCallable, Category = "Seek Target")
		void SetTarget(AActor *Actor);

	/** Determines where the pawn should move to next */
	void Move();

	/** Calculates the distance between two vectors in 2D space */
	float CalcDist(FVector Vect1, FVector Vect2);

private:
	float MaxSpeed; /**< MaxSpeed determines the maxmimum speed the pawn is allowed to move at */

	float SatisfactionRad; /**< SatisfactionRad determines the acceptable radius for the pawn to stop moving in */

	float TimeToTarget; /**< TimeToTarget determines the amount of time it should take the pawn to reach the target */

	bool OverlappingEnemy; /**< OverlappingEnemy determines if the pawn's hitbox is overlapping an enemy */

	bool Attacking; /**< Attacking determines if the pawn is currently attacking an enemy */

	AActor *Target; /**< Target points to the actor the pawn is currently following */

	FVector VelocityOut; /**< VelocityOut is determines the next location for the pawn to move to */

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase *ExplosionSound; /**< Sound file to play when this pawn attacks an enemy */
};
