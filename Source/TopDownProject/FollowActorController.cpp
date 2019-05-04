#include "FollowActorController.h"
#include "FollowingPawn.h"
#include "PlayerChar.h"
#include "BasicEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine.h"

AFollowActorController::AFollowActorController()
{
	PrimaryActorTick.bCanEverTick = true; // Set to call Tick every frame

	// Creates Blackboard and BehaviourTree components
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	check(BlackboardComp);
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	check(BehaviorTreeComp);
}

void AFollowActorController::BeginPlay()
{
	Super::BeginPlay();
}

void AFollowActorController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SelectTarget();
	Move();
}

void AFollowActorController::Possess(APawn *InPawn)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Possessing Actor...")));
	}
	Super::Possess(InPawn); // Possesses a pawn
	PawnAsNPC = Cast<AFollowingPawn>(GetPawn()); // Casts PawnAsNPC as possessed pawn
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Possessed Actor")));
	}
	// Checks if PawnAsNPC is valid
	if (PawnAsNPC != nullptr)
	{
		// Checks is BTree is not a nullptr
		if (PawnAsNPC->BTree != nullptr)
		{
			// Checks if BlackboardAsset is not a nullptr
			if (PawnAsNPC->BTree->BlackboardAsset != nullptr)
			{
				BlackboardComp->InitializeBlackboard(*(PawnAsNPC->BTree->BlackboardAsset)); // Initialises Blackboard
			}
			BehaviorTreeComp->StartTree(*(PawnAsNPC->BTree)); // Starts BehaviourTree
		}
	}
}

void AFollowActorController::UnPossess()
{
	Super::UnPossess(); // Unpossesses pawn
	PawnAsNPC = nullptr;
}

void AFollowActorController::SelectTarget()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Finding target...")));
	}
	APlayerChar *Player = Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	PawnAsNPC->SetTarget(Player);
	float ClosestDistance = 500.0f;
	for (TActorIterator<ABasicEnemy> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		float Distance = Iterator->CalcDist(PawnAsNPC->GetActorLocation(), Iterator->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			if (ABasicEnemy *OtherActor = Cast<ABasicEnemy>(Iterator->Raycast(PawnAsNPC->GetActorLocation(), Iterator->GetActorLocation())))
			{
				PawnAsNPC->SetTarget(*Iterator);
				ClosestDistance = Distance;
			}
		}
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Target set")));
	}
}

void AFollowActorController::Move()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Starting movement calculations...")));
	}
	FVector Position = PawnAsNPC->GetActorLocation();
	FVector Target = PawnAsNPC->GetTarget()->GetActorLocation();
	FVector VelocityOut;
	float Magnitude;
	VelocityOut.X = Target.X - Position.X;
	VelocityOut.Y = Target.Y - Position.Y;
	VelocityOut.Z = Target.Z - Position.Z;
	Magnitude = FMath::Sqrt((VelocityOut.X * VelocityOut.X)
		+ (VelocityOut.Y * VelocityOut.Y)
		+ (VelocityOut.Z * VelocityOut.Z));
	if (Magnitude < PawnAsNPC->GetSatisfactionRad())
	{
		VelocityOut.X = 0.0f;
		VelocityOut.Y = 0.0f;
		VelocityOut.Z = 0.0f;
		return;
	}
	VelocityOut.X /= PawnAsNPC->GetTimeToTarget();
	VelocityOut.Y /= PawnAsNPC->GetTimeToTarget();
	VelocityOut.Z /= PawnAsNPC->GetTimeToTarget();
	Magnitude = FMath::Sqrt((VelocityOut.X * VelocityOut.X)
		+ (VelocityOut.Y * VelocityOut.Y)
		+ (VelocityOut.Z * VelocityOut.Z));
	if (Magnitude > PawnAsNPC->GetMaxSpeed())
	{
		VelocityOut.X = VelocityOut.X / Magnitude;
		VelocityOut.Y = VelocityOut.Y / Magnitude;
		VelocityOut.Z = VelocityOut.Z / Magnitude;
		VelocityOut.X *= PawnAsNPC->GetMaxSpeed();
		VelocityOut.Y *= PawnAsNPC->GetMaxSpeed();
		VelocityOut.Z *= PawnAsNPC->GetMaxSpeed();
	}
	MoveToLocation(VelocityOut, 5.0f, true, true, false, false, 0, true);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Moved")));
	}
}

UBlackboardComponent *AFollowActorController::GetBBoard()
{
	return BlackboardComp;
}

UBehaviorTreeComponent *AFollowActorController::GetBTree()
{
	return BehaviorTreeComp;
}