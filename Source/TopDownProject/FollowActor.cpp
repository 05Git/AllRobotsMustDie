#include "FollowActor.h"

AFollowActor::AFollowActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxSpeed = 100.0f;
	SatisfactionRad = 10.0f;
	TimeToTarget = 2.0f;
}

void AFollowActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFollowActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AFollowActor::GetMaxSpeed()
{
	return MaxSpeed;
}

float AFollowActor::GetSatisfactionRad()
{
	return SatisfactionRad;
}

float AFollowActor::GetTimeToTarget()
{
	return TimeToTarget;
}

AActor *AFollowActor::GetTarget()
{
	return Target;
}

void AFollowActor::SetTarget(AActor *Actor)
{
	this->Target = Actor;
}