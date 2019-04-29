#include "Bullet.h"
#include "BasicEnemy.h"
#include "TDPGameInstance.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ABullet::ABullet()
{
	/** Set to call Tick every frame */
	PrimaryActorTick.bCanEverTick = true;

	/** Creates and sets Static Mesh component */
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	Mesh->BodyInstance.SetCollisionProfileName("Projectile");
	Mesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	/** Creates and sets Projectile Movement component */
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	Movement->UpdatedComponent = Mesh;
	Movement->InitialSpeed = 5000.0f;
	Movement->MaxSpeed = 10000.0f;
	Movement->bRotationFollowsVelocity = true;
	Movement->bShouldBounce = false;
	Movement->ProjectileGravityScale = 0.0f;

	/** Destroys projectile after 3 seconds */
	InitialLifeSpan = 3.0f;

	BaseDamage = 20.0f;
	CritMultiplier = 2.0f;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();	
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor,
	UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	if (ABasicEnemy *BEnemy = Cast<ABasicEnemy>(Hit.GetActor()))
	{
		if (!BEnemy->IsAlert())
		{
			BEnemy->ReceiveDamage(BaseDamage * CritMultiplier);
			if (CritSound != nullptr)
			{
				UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				UGameplayStatics::PlaySoundAtLocation(this, CritSound, this->GetActorLocation(), Instance->GetVolume());
			}
		}
		else
		{
			BEnemy->ReceiveDamage(BaseDamage);
		}
		BEnemy->PlayOnHitSound();
		BEnemy->SetAlert(true);
	}
	/** Destroys the bullet */
	Destroy();
}
