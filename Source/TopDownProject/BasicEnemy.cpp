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
	if (IncomingDamage >= 0)
	{
		Health -= IncomingDamage;
	}
}

void ABasicEnemy::SetHealth(float Health)
{
	this->Health = Health;
}

float ABasicEnemy::GetHealthRemaining()
{
	return Health;
}

bool ABasicEnemy::GetIsAttacking()
{
	return IsAttacking;
}

void ABasicEnemy::SetIsAttacking(bool IsAttacking)
{
	this->IsAttacking = IsAttacking;
}

bool ABasicEnemy::GetIsOverlapping()
{
	return IsOverlapping;
}

void ABasicEnemy::SetIsOverlapping(bool IsOverlapping)
{
	this->IsOverlapping = IsOverlapping;
}

void ABasicEnemy::PlayElectricSound()
{
	if (ElectricSound != nullptr)
	{
		UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		UGameplayStatics::PlaySoundAtLocation(this, ElectricSound, this->GetActorLocation(), Instance->GetVolume());
	}
}

void ABasicEnemy::DeathSequence()
{
	if (ExplosionSound != nullptr)
	{
		UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, this->GetActorLocation(), Instance->GetVolume());
	}
}

float ABasicEnemy::CalcFOV(FVector Vect1, FVector Vect2, FRotator Rot)
{
	float Yaw = Rot.Yaw * (MI_PI / 180);
	FVector Orientation;
	Orientation.X = FMath::Cos(Yaw);
	Orientation.Y = FMath::Sin(Yaw);
	FVector ToPlayer;
	ToPlayer.X = Vect1.X - Vect2.X;
	ToPlayer.Y = Vect1.Y - Vect2.Y;
	float Magnitude = FMath::Sqrt(FMath::Square(ToPlayer.X) + FMath::Square(ToPlayer.Y) + FMath::Square(ToPlayer.Z));
	ToPlayer.X = ToPlayer.X / Magnitude;
	ToPlayer.Y = ToPlayer.Y / Magnitude;
	ToPlayer.Z = ToPlayer.Z / Magnitude;
	float Dot = (ToPlayer.X * Orientation.X) + (ToPlayer.Y * Orientation.Y) + (ToPlayer.Z * Orientation.Z);
	return FMath::Acos(Dot);
}

bool ABasicEnemy::PlayerInFOV(float Angle)
{
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
	FVector Diff;
	Diff.X = FMath::Square(Vect2.X - Vect1.X);
	Diff.Y = FMath::Square(Vect2.Y - Vect1.Y);
	Diff.Z = FMath::Square(Vect2.Z - Vect1.Z);
	return FMath::Sqrt(Diff.X + Diff.Y + Diff.Z);
}

bool ABasicEnemy::IsAlert()
{
	return Alert;
}

void ABasicEnemy::SetAlert(bool Alertness)
{
	this->Alert = Alertness;
}

void ABasicEnemy::PlayOnHitSound()
{
	if (OnHitSound != nullptr)
	{
		UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		UGameplayStatics::PlaySoundAtLocation(this, OnHitSound, this->GetActorLocation(), Instance->GetVolume());
	}
}
