#include "BEnemyController.h"
#include "BasicEnemy.h"
#include "PlayerChar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

ABEnemyController::ABEnemyController()
{
	/** Set to call Tick every frame */
	PrimaryActorTick.bCanEverTick = true;

	/** Creates Blackboard and BehaviourTree components */
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	check(BlackboardComp);
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	check(BehaviorTreeComp);

	/** Sets CurrentState and CurrentEvent */
	CurrentState = SPAWN;
	CurrentEvent = onEnter;
}

void ABEnemyController::BeginPlay()
{
	Super::BeginPlay();
}

void ABEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** Sets LengthOfSight depending on if Alert is true or false */
	if (PawnAsNPC->IsAlert())
	{
		LengthOfSight = 2000.0f;
	}
	else
	{
		LengthOfSight = 1500.0f;
	}

	/** Performs calculations to determine distance from possessed pawn and player,
	and to determine if the player is in the possessed pawn's FOV */
	APlayerChar *Player = Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player != nullptr)
	{
		/** Calculates distance between player and possessed pawn */
		Distance = PawnAsNPC->CalcDist(Player->GetActorLocation(), PawnAsNPC->GetActorLocation());
		/** Calculates possessed pawn's FOV */
		FOV = PawnAsNPC->CalcFOV(Player->GetActorLocation(), PawnAsNPC->GetActorLocation(), PawnAsNPC->GetActorRotation());
		/** Determines if the player is in possessed pawn's FOV */
		InFOV = PawnAsNPC->PlayerInFOV(FOV);
		/** Checks if InFOV is true and Distance is less than or equal to LengthOfSight */
		if (InFOV && Distance <= LengthOfSight)
		{
			/** Sets possessed pawn's Alert to true */
			PawnAsNPC->SetAlert(true);
		}
	}

	/** Checks if possessed pawn is alert */
	if (!PawnAsNPC->IsAlert())
	{
		/** Iterates through each BasicEnemy in the world */
		for (TActorIterator<ABasicEnemy> Iterator(GetWorld()); Iterator; ++Iterator)
		{
			/** Calculates distance between possessed pawn and currrent BasicEnemy in iterator */
			float AlertRange = PawnAsNPC->CalcDist(Iterator->GetActorLocation(), PawnAsNPC->GetActorLocation());
			/** Checks if AlertRange is less than or equal to 500.0f and BasicEnemy in iterator is alert */
			if (AlertRange <= 500.0f && Iterator->IsAlert())
			{
				/** Sets possessed pawn's alert to true */
				PawnAsNPC->SetAlert(true);
			}
		}
	}

	/** Sets rotation of possessed pawn to face forward */
	FRotator Rotation = FRotator(0.0f, PawnAsNPC->GetActorRotation().Yaw, 0.0f);
	PawnAsNPC->SetActorRotation(Rotation);

	/** Checks CurrentState and CurrentEvent */
	if (CurrentState == SPAWN)
	{
		switch (CurrentEvent)
		{
		case onEnter:
			/** Sets Health, IsAttacking and IsOverlapping of possessed pawn */
			PawnAsNPC->SetHealth(50.0f);
			PawnAsNPC->SetIsAttacking(false);
			PawnAsNPC->SetIsOverlapping(false);
			CurrentEvent = onUpdate;
			break;
		case onUpdate:
			/** Determines which state to transition to depending on distance and alert status */
			if (Distance <= LengthOfSight && Distance > 100.0f && PawnAsNPC->IsAlert())
			{
				NextState = CHASE;
			}
			else if (Distance <= 100.0f && PawnAsNPC->IsAlert())
			{
				NextState = ATTACK;
			}
			else
			{
				NextState = IDLE;
			}
			CurrentEvent = onExit;
			break;
		case onExit:
			/** Sets CurrentState to NextState and CurrentEvent to onEnter */
			CurrentState = NextState;
			CurrentEvent = onEnter;
			break;
		}
	}
	else if (CurrentState == IDLE)
	{
		switch (CurrentEvent)
		{
		case onEnter:
			CurrentEvent = onUpdate;
			break;
		case onUpdate:
			/** Determines which state to transition to depending on distance and alert status */
			if (Distance <= LengthOfSight && Distance > 100.0f && PawnAsNPC->IsAlert())
			{
				NextState = CHASE;
				CurrentEvent = onExit;
			}
			else if (Distance <= 100.0f && PawnAsNPC->IsAlert())
			{
				NextState = ATTACK;
				CurrentEvent = onExit;
			}
			if (PawnAsNPC->GetHealthRemaining() <= 0)
			{
				NextState = DIE;
				CurrentEvent = onExit;
			}
			break;
		case onExit:
			/** Sets CurrentState to NextState and CurrentEvent to onEnter */
			CurrentState = NextState;
			CurrentEvent = onEnter;
			break;
		}
	}
	else if (CurrentState == CHASE)
	{
		switch (CurrentEvent)
		{
		case onEnter:
			CurrentEvent = onUpdate;
			break;
		case onUpdate:
		{
			/** Gets the navmesh used by the world */
			UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			if (NavSys)
			{
				if (Player != nullptr)
				{
					/** Gets player location and moves possessed pawn towards it using the navmesh */
					FVector PlayerLoc = Player->GetActorLocation();
					this->MoveToLocation(PlayerLoc, 0.0f, true, true, false, false, 0, true);
				}
			}
			/** Determines which state to transition to depending on distance and alert status */
			if (Distance > LengthOfSight)
			{
				/** Sets possessed pawn's alert to false after 60 seconds if the player goes beyond LengthOfSight */
				GetWorldTimerManager().SetTimer(AlertCoolDown, this, &ABEnemyController::Alertness, 60.0f);
				NextState = IDLE;
				CurrentEvent = onExit;
			}
			else if (Distance <= 100.0f && PawnAsNPC->IsAlert())
			{
				NextState = ATTACK;
				CurrentEvent = onExit;
			}
			if (PawnAsNPC->GetHealthRemaining() <= 0)
			{
				NextState = DIE;
				CurrentEvent = onExit;
			}
		}
		break;
		case onExit:
			/** Sets CurrentState to NextState and CurrentEvent to onEnter */
			CurrentState = NextState;
			CurrentEvent = onEnter;
			break;
		}
	}
	else if (CurrentState == ATTACK)
	{
		switch (CurrentEvent)
		{
		case onEnter:
			CurrentEvent = onUpdate;
			break;
		case onUpdate:
			/** Determines which state to transition to depending on distance and alert status */
			if (Distance > LengthOfSight)
			{
				GetWorldTimerManager().SetTimer(AlertCoolDown, this, &ABEnemyController::Alertness, 100.0f);
				NextState = IDLE;
				CurrentEvent = onExit;
			}
			else if (Distance <= LengthOfSight && Distance > 100.0f && PawnAsNPC->IsAlert())
			{
				NextState = CHASE;
				CurrentEvent = onExit;
			}
			if (PawnAsNPC->GetHealthRemaining() <= 0)
			{
				NextState = DIE;
				CurrentEvent = onExit;
			}
			break;
		case onExit:
			/** Sets CurrentState to NextState and CurrentEvent to onEnter */
			CurrentState = NextState;
			CurrentEvent = onEnter;
			break;
		}
	}
	else if (CurrentState == DIE)
	{
		switch (CurrentEvent)
		{
		case onEnter:
			CurrentEvent = onUpdate;
			break;
		case onUpdate:
		{
			/** Plays possessed pawn's death sequence */
			PawnAsNPC->DeathSequence();
			CurrentEvent = onExit;
		}
		break;
		case onExit:
			/** Destroys and unpossesses pawn */
			PawnAsNPC->Destroy();
			UnPossess();
			break;
		}
	}
}

void ABEnemyController::Possess(APawn *InPawn)
{
	/** Possesses pawn and casts as a BasicEnemy */
	Super::Possess(InPawn);
	PawnAsNPC = Cast<ABasicEnemy>(GetPawn());
	/** Checks if PawnAsNPC is not a nullptr */
	if (PawnAsNPC != nullptr)
	{
		/** Creates movement component and sets MaxWalkSpeed to 300.0f */
		UCharacterMovementComponent *MoveComp = PawnAsNPC->GetCharacterMovement();
		MoveComp->MaxWalkSpeed = 300.0f;
		/** Checks is BTree is not a nullptr */
		if (PawnAsNPC->BTree != nullptr)
		{
			/** Checks if BlackboardAsset is not a nullptr */
			if (PawnAsNPC->BTree->BlackboardAsset != nullptr)
			{
				/** Initialises Blackboard */
				BlackboardComp->InitializeBlackboard(*(PawnAsNPC->BTree->BlackboardAsset));
			}
			/** Starts BehaviourTree */
			BehaviorTreeComp->StartTree(*(PawnAsNPC->BTree));
		}
	}
}

void ABEnemyController::UnPossess()
{
	/** Unpossesses pawn and sets it to a nullptr */
	Super::UnPossess();
	PawnAsNPC = nullptr;
}

void ABEnemyController::Alertness()
{
	/** Sets possessed pawn's alert to false */
	PawnAsNPC->SetAlert(false);
}

states ABEnemyController::GetCurrentState()
{
	/** Returns CurrentState */
	return CurrentState;
}

UBlackboardComponent *ABEnemyController::GetBBoard()
{
	/** Returns BlackboardComp */
	return BlackboardComp;
}

UBehaviorTreeComponent *ABEnemyController::GetBTree()
{
	/** Returns BehaviourTreeComp */
	return BehaviorTreeComp;
}
