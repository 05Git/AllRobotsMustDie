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
		class UBehaviorTree *BTree;

	UFUNCTION(BlueprintCallable, Category = "IsAttacking")
		bool GetIsAttacking();

	UFUNCTION(BlueprintCallable, Category = "IsAttacking")
		void SetIsAttacking(bool IsAttacking);

	UFUNCTION(BlueprintCallable, Category = "IsOverlapping")
		bool GetIsOverlapping();
	
	UFUNCTION(BlueprintCallable, Category = "IsOverlapping")
		void SetIsOverlapping(bool IsOverlapping);

	UFUNCTION(BlueprintCallable, Category = "Sound")
		void PlayElectricSound();

	void PlayOnHitSound();

	/** RecieveDamage function from DamageInterface */
	virtual void ReceiveDamage(float IncomingDamage) override;

	void SetHealth(float Health);

	/** GetHealthRemaining function from DamageInterface */
	virtual float GetHealthRemaining() override;

	void DeathSequence();

	float CalcFOV(FVector Vect1, FVector Vect2, FRotator Rot);

	bool PlayerInFOV(float Angle);

	float CalcDist(FVector Vect1, FVector Vect2);

	bool IsAlert();

	void SetAlert(bool Alertness);

private:
	float Health;

	float MI_PI;

	bool IsAttacking;

	bool IsOverlapping;

	bool Alert;

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase *ElectricSound;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase *ExplosionSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase *OnHitSound;
};
