#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

/**
* Bullet
* Basic projectile used by characters with guns
*/
UCLASS()
class TOPDOWNPROJECT_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Sets default values for this character's properties */
	ABullet();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Controls collision events depending on other actor*/
	UFUNCTION()
		void OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor,
			 UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

private:
	float BaseDamage;

	float CritMultiplier;

	UPROPERTY(EditAnywhere, Category = "Static Mesh")
		class UStaticMeshComponent *Mesh; /**< Static Mesh component */

	class UProjectileMovementComponent *Movement; /**< Projectile movement component */

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase *CritSound;
};
