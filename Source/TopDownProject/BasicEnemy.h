#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageInterface.h"
#include "BasicEnemy.generated.h"

/**
* BasicEnemy
* Basic enemy character
*/
UCLASS()
class TOPDOWNPROJECT_API ABasicEnemy : public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

public:
	/** Sets default values for this character's properties */
	ABasicEnemy();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Pawn")
		class UBehaviorTree *BTree; /**< Behaviour Tree used by this character */

	/** Returns IsAttacking */
	UFUNCTION(BlueprintCallable, Category = "IsAttacking")
		bool GetIsAttacking();

	/** Sets IsAttacking */
	UFUNCTION(BlueprintCallable, Category = "IsAttacking")
		void SetIsAttacking(bool IsAttacking);

	/** Returns IsOverlapping */
	UFUNCTION(BlueprintCallable, Category = "IsOverlapping")
		bool GetIsOverlapping();
	
	/** Sets IsOverlapping */
	UFUNCTION(BlueprintCallable, Category = "IsOverlapping")
		void SetIsOverlapping(bool IsOverlapping);

	/** Returns Alert */
	bool IsAlert();

	/** Sets Alert */
	void SetAlert(bool Alertness);

	/** Plays ElectricSound */
	UFUNCTION(BlueprintCallable, Category = "Sound")
		void PlayElectricSound();

	/** Plays OnHitSound */
	void PlayOnHitSound();

	/** RecieveDamage function from DamageInterface */
	virtual void ReceiveDamage(float IncomingDamage) override;

	/** Sets Health */
	void SetHealth(float Health);

	/** GetHealthRemaining function from DamageInterface */
	virtual float GetHealthRemaining() override;

	/** Performs actions associated with this character's death */
	void DeathSequence();

	/** Calculates this characters field of vision */
	float CalcFOV(FVector Vect1, FVector Vect2, FRotator Rot);

	/** Checks if the player character is in range of this character's fov */
	bool PlayerInFOV(float Angle);

	/** Calculates the distance between two vectors */
	float CalcDist(FVector Vect1, FVector Vect2);

	/** Converts a radian to degrees */
	float ConvRadianToDegree(float Rad);

	/** Converts a degree to a radian */
	float ConvDegreeToRadian(float Degree);

private:
	float Health; /**< Health stores how much health this character has */

	float MI_PI; /**< Set float value used in radian and degree conversions */

	bool IsAttacking; /**< IsAttacking determines if this character is currently attacking */

	bool IsOverlapping; /**< IsOverlapping determines if this character's hitbox is overlapping with the player */

	bool Alert; /**< Alert determines if this character is aware of the player */

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase *ElectricSound; /**< Sound file to play when this character attacks */
	
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase *ExplosionSound; /**< Sound file to play when this character dies */

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase *OnHitSound; /**< Sound file to play when this character is hit with a bullet */
};
