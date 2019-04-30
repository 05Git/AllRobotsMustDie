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
	/** Sets default values for this object's properties */
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
	float BaseDamage; /**< BaseDamage determines the basic amount of damage a bullet will do */

	float CritMultiplier; /**< CritMultiplier determinesa the amount BaseDamage is multiplied by when performing a critical hit */

	UPROPERTY(EditAnywhere, Category = "Static Mesh")
		class UStaticMeshComponent *Mesh; /**< Static Mesh component */

	class UProjectileMovementComponent *Movement; /**< Projectile movement component */

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase *CritSound; /**< Sound file to play when a critical hit is performed */
};
