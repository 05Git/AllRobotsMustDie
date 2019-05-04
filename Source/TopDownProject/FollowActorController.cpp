#include "FollowActorController.h"
#include "FollowActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

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
}

void AFollowActorController::Possess(APawn *InPawn)
{
	Super::Possess(InPawn); // Possesses a pawn
	PawnAsNPC = Cast<AFollowActor>(GetPawn()); // Casts PawnAsNPC as possessed pawn
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
}