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

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	check(BlackboardComp);
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	check(BehaviorTreeComp);

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

	if (PawnAsNPC->IsAlert())
	{
		LengthOfSight = 2000.0f;
	}
	else
	{
		LengthOfSight = 1500.0f;
	}

	APlayerChar *Player = Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player != nullptr)
	{
		Distance = PawnAsNPC->CalcDist(Player->GetActorLocation(), PawnAsNPC->GetActorLocation());
		FOV = PawnAsNPC->CalcFOV(Player->GetActorLocation(), PawnAsNPC->GetActorLocation(), PawnAsNPC->GetActorRotation());
		InFOV = PawnAsNPC->PlayerInFOV(FOV);
		if (InFOV && Distance <= LengthOfSight)
		{
			PawnAsNPC->SetAlert(true);
		}
	}

	if (!PawnAsNPC->IsAlert())
	{
		for (TActorIterator<ABasicEnemy> Iterator(GetWorld()); Iterator; ++Iterator)
		{
			float AlertRange = PawnAsNPC->CalcDist(Iterator->GetActorLocation(), PawnAsNPC->GetActorLocation());
			if (Distance <= 800.0f && Iterator->IsAlert())
			{
				PawnAsNPC->SetAlert(true);
			}
		}
	}

	FRotator Rotation = FRotator(0.0f, PawnAsNPC->GetActorRotation().Yaw, 0.0f);
	PawnAsNPC->SetActorRotation(Rotation);

	if (CurrentState == SPAWN)
	{
		switch (CurrentEvent)
		{
		case onEnter:
			PawnAsNPC->SetHealth(50.0f);
			PawnAsNPC->SetIsAttacking(false);
			PawnAsNPC->SetIsOverlapping(false);
			CurrentEvent = onUpdate;
			break;
		case onUpdate:
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
			UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			if (NavSys)
			{
				if (Player)
				{
					FVector PlayerLoc = Player->GetActorLocation();
					this->MoveToLocation(PlayerLoc, 0.0f, true, true, false, false, 0, true);
				}
			}
			if (Distance > LengthOfSight)
			{
				GetWorldTimerManager().SetTimer(AlertCoolDown, this, &ABEnemyController::Alertness, 100.0f);
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
			PawnAsNPC->DeathSequence();
			CurrentEvent = onExit;
		}
		break;
		case onExit:
			PawnAsNPC->Destroy();
			UnPossess();
			break;
		}
	}
}

void ABEnemyController::Possess(APawn *InPawn)
{
	Super::Possess(InPawn);
	PawnAsNPC = Cast<ABasicEnemy>(GetPawn());

	if (PawnAsNPC != nullptr)
	{
		UCharacterMovementComponent *MoveComp = PawnAsNPC->GetCharacterMovement();
		MoveComp->MaxWalkSpeed = 300.0f;
		if (PawnAsNPC->BTree != nullptr)
		{
			if (PawnAsNPC->BTree->BlackboardAsset != nullptr)
			{
				BlackboardComp->InitializeBlackboard(*(PawnAsNPC->BTree->BlackboardAsset));
			}
			BehaviorTreeComp->StartTree(*(PawnAsNPC->BTree));
		}
	}
}

void ABEnemyController::UnPossess()
{
	Super::UnPossess();
	PawnAsNPC = nullptr;
}

void ABEnemyController::Alertness()
{
	PawnAsNPC->SetAlert(false);
}

states ABEnemyController::GetCurrentState()
{
	return CurrentState;
}

UBlackboardComponent *ABEnemyController::GetBBoard()
{
	return BlackboardComp;
}

UBehaviorTreeComponent *ABEnemyController::GetBTree()
{
	return BehaviorTreeComp;
}
