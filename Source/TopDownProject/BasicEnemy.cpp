#include "BasicEnemy.h"
#include "PlayerChar.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Public/EngineUtils.h"
#include "Engine.h"
#include "TDPGameInstance.h"
#include "BehaviorTree/BehaviorTree.h"

ABasicEnemy::ABasicEnemy()
{
	PrimaryActorTick.bCanEverTick = true; // Set to call Tick every frame
	MI_PI = 3.14159265f; // Sets value of MI_PI
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
	if (IncomingDamage >= 0) // Checks if IncomingDamage is greater than or equal to 0
	{
		Health -= IncomingDamage; // Reduces health by IncomingDamage
	}
}

float ABasicEnemy::GetHealthRemaining()
{
	return Health; // Returns Heath
}

void ABasicEnemy::SetHealth(float Health)
{
	this->Health = Health; // Sets Health to Health
}

bool ABasicEnemy::GetIsAttacking()
{
	return IsAttacking; // Returns IsAttacking
}

void ABasicEnemy::SetIsAttacking(bool IsAttacking)
{
	this->IsAttacking = IsAttacking; // Sets IsAttacking to IsAttacking
}

bool ABasicEnemy::GetIsOverlapping()
{
	return IsOverlapping; // Returns IsOverlapping
}

void ABasicEnemy::SetIsOverlapping(bool IsOverlapping)
{
	this->IsOverlapping = IsOverlapping; // Sets IsOverlapping to IsOverlapping
}

bool ABasicEnemy::IsAlert()
{
	return Alert; // Returns Alert
}

void ABasicEnemy::SetAlert(bool Alertness)
{
	this->Alert = Alertness; // Sets Alert to Alertness
}

void ABasicEnemy::PlayElectricSound()
{
	// Plays ElectricSound
	if (ElectricSound != nullptr)
	{
		UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		UGameplayStatics::PlaySoundAtLocation(this, ElectricSound, this->GetActorLocation(), Instance->GetVolume());
	}
}

void ABasicEnemy::PlayOnHitSound()
{
	// Plays OnHitSound
	if (OnHitSound != nullptr)
	{
		UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		UGameplayStatics::PlaySoundAtLocation(this, OnHitSound, this->GetActorLocation(), Instance->GetVolume());
	}
}

void ABasicEnemy::DeathSequence()
{
	UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	Instance->AddScore(30.0f); // Increases score by 30
	// Plays ExplosionSound
	if (ExplosionSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, this->GetActorLocation(), Instance->GetVolume());
	}
}

float ABasicEnemy::CalcFOV(FVector Vect1, FVector Vect2, FRotator Rot)
{
	float Yaw = ConvDegreeToRadian(Rot.Yaw); // Converts Rot.Yaw to Radian
	// Gets X and Y orientation of Rot
	FVector Orientation;
	Orientation.X = FMath::Cos(Yaw);
	Orientation.Y = FMath::Sin(Yaw);
	// Creates vector from Vect1 to Vect2
	FVector ToPlayer;
	ToPlayer.X = Vect1.X - Vect2.X;
	ToPlayer.Y = Vect1.Y - Vect2.Y;
	// Gets magnitude of ToPlayer and divides current X, Y and Z values by magnitude
	float Magnitude = FMath::Sqrt(FMath::Square(ToPlayer.X) + FMath::Square(ToPlayer.Y) + FMath::Square(ToPlayer.Z));
	ToPlayer.X = ToPlayer.X / Magnitude;
	ToPlayer.Y = ToPlayer.Y / Magnitude;
	ToPlayer.Z = ToPlayer.Z / Magnitude;
	float Dot = (ToPlayer.X * Orientation.X) + (ToPlayer.Y * Orientation.Y) + (ToPlayer.Z * Orientation.Z); // Gets dot product of ToPlayer and Orientation
	return FMath::Acos(Dot); // Returns Acos of dot product
}

bool ABasicEnemy::PlayerInFOV(float Angle)
{
	// Checks if Angle is within field of view, returns true if it is and false if it isn't
	if (ConvRadianToDegree(Angle) < 160 / 2)
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
	// Squares the difference between X, Y and Z coordinates of Vectors 1 and 2
	FVector Diff;
	Diff.X = FMath::Square(Vect2.X - Vect1.X);
	Diff.Y = FMath::Square(Vect2.Y - Vect1.Y);
	Diff.Z = FMath::Square(Vect2.Z - Vect1.Z);
	return FMath::Sqrt(Diff.X + Diff.Y + Diff.Z); // Returns the square root of the squared differences added together
}

float ABasicEnemy::ConvRadianToDegree(float Rad)
{
	return Rad * (180 / MI_PI); // Returns the converted degree
}

float ABasicEnemy::ConvDegreeToRadian(float Degree)
{
	return Degree * (MI_PI / 180); // Returns the converted radian
}