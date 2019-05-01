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
	PrimaryActorTick.bCanEverTick = true; // Set to call Tick every frame

	// Creates and sets Static Mesh component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	Mesh->BodyInstance.SetCollisionProfileName("Projectile");
	Mesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	// Creates and sets Projectile Movement component
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	Movement->UpdatedComponent = Mesh;
	Movement->InitialSpeed = 5000.0f;
	Movement->MaxSpeed = 10000.0f;
	Movement->bRotationFollowsVelocity = true;
	Movement->bShouldBounce = false;
	Movement->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 3.0f; // Destroys projectile after 3 seconds

	BaseDamage = 20.0f; // Sets BaseDamage
	CritMultiplier = 2.0f; // Sets CritMultiplier
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
	// Checks if bullet collided with a BasicEnemy
	if (ABasicEnemy *BEnemy = Cast<ABasicEnemy>(Hit.GetActor()))
	{
		// Checks if BasicEnemy is not alert
		if (!BEnemy->IsAlert())
		{
			BEnemy->ReceiveDamage(BaseDamage * CritMultiplier); // Assigns BaseDamage * CritMultiplier
			UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			Instance->AddScore(15.0f); // Increases Score by 15
			// Plays CritSound
			if (CritSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, CritSound, this->GetActorLocation(), Instance->GetVolume());
			}
		}
		else
		{
			BEnemy->ReceiveDamage(BaseDamage); // Assigns BaseDamage
		}
		BEnemy->PlayOnHitSound(); // Plays BasicEnemy's OnHitSound
		BEnemy->SetAlert(true); // Sets BasiEnemy's alert to true
	}
	Destroy(); // Destroys this actor
}
