#include "BasicEnemy.h"
#include "PlayerChar.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Public/EngineUtils.h"
#include "Engine.h"
#include "TDPGameInstance.h"

ABasicEnemy::ABasicEnemy()
{
	/** Set to call Tick every frame */
	PrimaryActorTick.bCanEverTick = true;

	/** Sets value of MI_PI */
	MI_PI = 3.14159265f;
}

void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABasicEnemy::ReceiveDamage(float IncomingDamage)
{
	/** Checks if IncomingDamage is greater than or equal to 0 */
	if (IncomingDamage >= 0)
	{
		/** Reduces health by IncomingDamage */
		Health -= IncomingDamage;
	}
}

float ABasicEnemy::GetHealthRemaining()
{
	/** Returns Heath */
	return Health;
}

void ABasicEnemy::SetHealth(float Health)
{
	/** Sets Health to Health */
	this->Health = Health;
}

bool ABasicEnemy::GetIsAttacking()
{
	/** Returns IsAttacking */
	return IsAttacking;
}

void ABasicEnemy::SetIsAttacking(bool IsAttacking)
{
	/** Sets IsAttacking to IsAttacking */
	this->IsAttacking = IsAttacking;
}

bool ABasicEnemy::GetIsOverlapping()
{
	/** Returns IsOverlapping */
	return IsOverlapping;
}

void ABasicEnemy::SetIsOverlapping(bool IsOverlapping)
{
	/** Sets IsOverlapping to IsOverlapping */
	this->IsOverlapping = IsOverlapping;
}

bool ABasicEnemy::IsAlert()
{
	/** Returns Alert */
	return Alert;
}

void ABasicEnemy::SetAlert(bool Alertness)
{
	/** Sets Alert to Alertness */
	this->Alert = Alertness;
}

void ABasicEnemy::PlayElectricSound()
{
	/** Plays ElectricSound */
	if (ElectricSound != nullptr)
	{
		UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		UGameplayStatics::PlaySoundAtLocation(this, ElectricSound, this->GetActorLocation(), Instance->GetVolume());
	}
}

void ABasicEnemy::PlayOnHitSound()
{
	/** Plays OnHitSound */
	if (OnHitSound != nullptr)
	{
		UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		UGameplayStatics::PlaySoundAtLocation(this, OnHitSound, this->GetActorLocation(), Instance->GetVolume());
	}
}

void ABasicEnemy::DeathSequence()
{
	/** Plays ExplosionSound */
	if (ExplosionSound != nullptr)
	{
		UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, this->GetActorLocation(), Instance->GetVolume());
	}
}

float ABasicEnemy::CalcFOV(FVector Vect1, FVector Vect2, FRotator Rot)
{
	/** Converts Rot.Yaw to Radian */
	float Yaw = Rot.Yaw * (MI_PI / 180);
	/** Gets X and Y orientation of Rot */
	FVector Orientation;
	Orientation.X = FMath::Cos(Yaw);
	Orientation.Y = FMath::Sin(Yaw);
	/** Creates vector from Vect1 to Vect2 */
	FVector ToPlayer;
	ToPlayer.X = Vect1.X - Vect2.X;
	ToPlayer.Y = Vect1.Y - Vect2.Y;
	/** Gets magnitude of ToPlayer and divides current X, Y and Z values by magnitude */
	float Magnitude = FMath::Sqrt(FMath::Square(ToPlayer.X) + FMath::Square(ToPlayer.Y) + FMath::Square(ToPlayer.Z));
	ToPlayer.X = ToPlayer.X / Magnitude;
	ToPlayer.Y = ToPlayer.Y / Magnitude;
	ToPlayer.Z = ToPlayer.Z / Magnitude;
	/** Gets dot product of ToPlayer and Orientation */
	float Dot = (ToPlayer.X * Orientation.X) + (ToPlayer.Y * Orientation.Y) + (ToPlayer.Z * Orientation.Z);
	/** Returns Acos of dot product */
	return FMath::Acos(Dot);
}

bool ABasicEnemy::PlayerInFOV(float Angle)
{
	/** Checks if Angle is within field of view, returns true if it is and false if it isn't */
	if (Angle * (160 / MI_PI) < 180 / 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float ABasicEnemy::CalcDist(FVector Vect1, FVector Vect2)
{
	/** Squares the difference between X, Y and Z coordinates of Vectors 1 and 2 */
	FVector Diff;
	Diff.X = FMath::Square(Vect2.X - Vect1.X);
	Diff.Y = FMath::Square(Vect2.Y - Vect1.Y);
	Diff.Z = FMath::Square(Vect2.Z - Vect1.Z);
	/** Returns the square root of the squared differences added together */
	return FMath::Sqrt(Diff.X + Diff.Y + Diff.Z);
}
