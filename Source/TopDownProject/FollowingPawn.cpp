#include "FollowingPawn.h"
#include "PlayerChar.h"
#include "BasicEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine.h"

// Sets default values
AFollowingPawn::AFollowingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MaxSpeed = 100.0f;
	SatisfactionRad = 10.0f;
	TimeToTarget = 2.0f;
}

// Called when the game starts or when spawned
void AFollowingPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFollowingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFollowingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AFollowingPawn::GetMaxSpeed()
{
	return MaxSpeed;
}

float AFollowingPawn::GetSatisfactionRad()
{
	return SatisfactionRad;
}

float AFollowingPawn::GetTimeToTarget()
{
	return TimeToTarget;
}

AActor *AFollowingPawn::GetTarget()
{
	return Target;
}

void AFollowingPawn::SetTarget(AActor *Actor)
{
	this->Target = Actor;
}

void AFollowingPawn::SelectTarget()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Finding target...")));
	}
	APlayerChar *Player = Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Target = Player;
	float ClosestDistance = 500.0f;
	for (TActorIterator<ABasicEnemy> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		float Distance = Iterator->CalcDist(this->GetActorLocation(), Iterator->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			if (ABasicEnemy *OtherActor = Cast<ABasicEnemy>(Iterator->Raycast(this->GetActorLocation(), Iterator->GetActorLocation())))
			{
				Target = *Iterator;
				ClosestDistance = Distance;
			}
		}
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Target set")));
	}
}

void AFollowingPawn::Move()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Starting movement calculations...")));
	}
	FVector Position = this->GetActorLocation();
	FVector Target = GetTarget()->GetActorLocation();
	FVector VelocityOut;
	float Magnitude;
	VelocityOut.X = Target.X - Position.X;
	VelocityOut.Y = Target.Y - Position.Y;
	VelocityOut.Z = Target.Z - Position.Z;
	Magnitude = FMath::Sqrt((VelocityOut.X * VelocityOut.X)
		+ (VelocityOut.Y * VelocityOut.Y)
		+ (VelocityOut.Z * VelocityOut.Z));
	if (Magnitude < GetSatisfactionRad())
	{
		VelocityOut.X = 0.0f;
		VelocityOut.Y = 0.0f;
		VelocityOut.Z = 0.0f;
		return;
	}
	VelocityOut.X /= GetTimeToTarget();
	VelocityOut.Y /= GetTimeToTarget();
	VelocityOut.Z /= GetTimeToTarget();
	Magnitude = FMath::Sqrt((VelocityOut.X * VelocityOut.X)
		+ (VelocityOut.Y * VelocityOut.Y)
		+ (VelocityOut.Z * VelocityOut.Z));
	if (Magnitude > GetMaxSpeed())
	{
		VelocityOut.X = VelocityOut.X / Magnitude;
		VelocityOut.Y = VelocityOut.Y / Magnitude;
		VelocityOut.Z = VelocityOut.Z / Magnitude;
		VelocityOut.X *= GetMaxSpeed();
		VelocityOut.Y *= GetMaxSpeed();
		VelocityOut.Z *= GetMaxSpeed();
	}
	AddActorLocalOffset(VelocityOut, true);
	//MoveToLocation(VelocityOut, 5.0f, true, true, false, false, 0, true);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Moved")));
	}
}
