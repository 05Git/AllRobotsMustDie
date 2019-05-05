#include "FollowingPawn.h"
#include "PlayerChar.h"
#include "BasicEnemy.h"
#include "TDPGameInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "EngineUtils.h"

AFollowingPawn::AFollowingPawn()
{
	PrimaryActorTick.bCanEverTick = true; // Set this pawn to call Tick() every frame
	
	MaxSpeed = 1000.0f; // Sets initial value for MaxSpeed
	SatisfactionRad = 200.0f; // Sets initial value for SatisfactionRad
	TimeToTarget = 0.6f; // Sets initial value for TimeToTarget
	OverlappingEnemy = false; // Sets initial value for OverlappingEnemy
	Attacking = false; // Sets initial value for Attacking
}

void AFollowingPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AFollowingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SelectTarget(); // Selects a target
	Move(); // Gets new position with movement algorithm
	FVector NewPosition = GetActorLocation(); // Creates vector for new position
	NewPosition.X += VelocityOut.X * DeltaTime; // Sets new X position
	NewPosition.Y += VelocityOut.Y * DeltaTime; // Sets new Y position
	NewPosition.Z += VelocityOut.Z * DeltaTime; // Sets new Z position
	SetActorLocation(NewPosition, true); // Moves pawn towards NewPosition
}

void AFollowingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AFollowingPawn::GetMaxSpeed()
{
	return MaxSpeed; // Returns MaxSpeed
}

void AFollowingPawn::SetMaxSpeed(float Speed)
{
	MaxSpeed = Speed; // Sets MaxSpeed to Speed
}

float AFollowingPawn::GetSatisfactionRad()
{
	return SatisfactionRad; // Returns SatisfactionRad
}

void AFollowingPawn::SetSatisfactionRad(float Radius)
{
	SatisfactionRad = Radius; // Sets SatisfactionRad to Radius
}

float AFollowingPawn::GetTimeToTarget()
{
	return TimeToTarget; // Returns TimeToTarget
}

void AFollowingPawn::SetTimeToTarget(float Time)
{
	TimeToTarget = Time; // Sets TimeToTarget to Time
}

bool AFollowingPawn::IsOverlappingEnemy()
{
	return OverlappingEnemy; // Returns OverlappingEnemy
}

void AFollowingPawn::SetIsOverlappingEnemy(bool Overlap)
{
	OverlappingEnemy = Overlap; // Sets OverlappingEnemy to Overlap
}

bool AFollowingPawn::IsAttacking()
{
	return Attacking; // Returns Attacking
}

void AFollowingPawn::SetIsAttacking(bool Attack)
{
	Attacking = Attack; // Sets Attacking to Attack
}

void AFollowingPawn::PlayExplosionSound()
{
	// Plays ExplosionSound
	if (ExplosionSound != nullptr)
	{
		UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, this->GetActorLocation(), Instance->GetVolume());
	}
}

AActor *AFollowingPawn::GetTarget()
{
	return Target; // Returns Target
}

void AFollowingPawn::SetTarget(AActor *Actor)
{
	Target = Actor; // Sets Target to Actor
}

void AFollowingPawn::SelectTarget()
{
	APlayerChar *Player = Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Target = Player; // Sets target to player
	SatisfactionRad = 200.0f; // Sets SatisfactionRad to 200.0f
	float ClosestDistance = 3000.0f; // Sets ClosestDistance to 3000.0f, used to determine closest enemy
	// Iterates through each BasicEnemy in the level
	for (TActorIterator<ABasicEnemy> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		float Distance = Iterator->CalcDist(Iterator->GetActorLocation(), GetActorLocation()); // Calculates distance between Iterator and pawn
		// Checks if Distance is less than ClosestDistance
		if (Distance < ClosestDistance)
		{
			// Performs a raycast to enemy, checks if returned actor was a BasicEnemy
			if (ABasicEnemy *OtherActor = Cast<ABasicEnemy>(Iterator->Raycast(GetActorLocation(), Iterator->GetActorLocation())))
			{
				Target = OtherActor; // Sets Target to OtherActor
				SatisfactionRad = 10.0f; // Sets SatisfactionRad to 10.0f
				ClosestDistance = Distance; // Sets ClosestDistance to Distance
			}
		}
	}
}

void AFollowingPawn::Move()
{
	FVector Position = GetActorLocation(); // Gets pawn's position
	FVector Target = GetTarget()->GetActorLocation(); // Gets Target's position
	float Magnitude; // Variable to store the magnitude of VelocityOut
	VelocityOut.X = Target.X - Position.X; // Sets VelocityOut.X
	VelocityOut.Y = Target.Y - Position.Y; // Sets VelocityOut.Y
	VelocityOut.Z = Target.Z - Position.Z; // Sets VelocityOut.Z
	// Sets Magnitude of VelocityOut
	Magnitude = FMath::Sqrt((VelocityOut.X * VelocityOut.X)
		+ (VelocityOut.Y * VelocityOut.Y)
		+ (VelocityOut.Z * VelocityOut.Z));
	// Checks if Magnitude is less than SatisfactionRad
	if (Magnitude < SatisfactionRad)
	{
		// Sets VelocityOut to 0.0f and returns
		VelocityOut.X = 0.0f;
		VelocityOut.Y = 0.0f;
		VelocityOut.Z = 0.0f;
		return;
	}
	// Divides VelocityOut by TimeToTarget
	VelocityOut.X /= TimeToTarget;
	VelocityOut.Y /= TimeToTarget;
	VelocityOut.Z /= TimeToTarget;
	// Sets new value of Magnitude
	Magnitude = FMath::Sqrt((VelocityOut.X * VelocityOut.X)
		+ (VelocityOut.Y * VelocityOut.Y)
		+ (VelocityOut.Z * VelocityOut.Z));
	// Checks if Magnitude is greater than MaxSpeed
	if (Magnitude > MaxSpeed)
	{
		// Divides VelocityOut by Magnitude
		VelocityOut.X = VelocityOut.X / Magnitude;
		VelocityOut.Y = VelocityOut.Y / Magnitude;
		VelocityOut.Z = VelocityOut.Z / Magnitude;
		// Multiplies VelocityOut by MaxSpeed
		VelocityOut.X *= MaxSpeed;
		VelocityOut.Y *= MaxSpeed;
		VelocityOut.Z *= MaxSpeed;
	}
}

float AFollowingPawn::CalcDist(FVector Vect1, FVector Vect2)
{
	// Squares the difference between X and Y coordinates of Vect1 and Vect2
	FVector Diff;
	Diff.X = FMath::Square(Vect2.X - Vect1.X);
	Diff.Y = FMath::Square(Vect2.Y - Vect1.Y);
	return FMath::Sqrt(Diff.X + Diff.Y); // Returns the square root of the squared differences added together
}