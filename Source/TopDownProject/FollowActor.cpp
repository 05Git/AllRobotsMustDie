#include "FollowActor.h"

AFollowActor::AFollowActor()
{
	PrimaryActorTick.bCanEverTick = true;
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

AActor AFollowActor::SetTarget(AActor *Actor)
{
	this->Target = Actor;
}