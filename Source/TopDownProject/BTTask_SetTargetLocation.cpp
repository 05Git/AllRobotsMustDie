#include "BTTask_SetTargetLocation.h"
#include "BEnemyController.h"
#include "BasicEnemy.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_SetTargetLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	Controller = Cast<ABEnemyController>(OwnerComp.GetAIOwner());
	// Checks if Controller is not a nullptr
	if (Controller != nullptr)
	{
		PawnAsNPC = Cast<ABasicEnemy>(Controller->GetPawn());
		// Checks if PawnAsNPC is not a nullptr
		if (PawnAsNPC != nullptr)
		{
			UNavigationSystemV1 *NavSys = UNavigationSystemV1::GetCurrent(GetWorld()); // Gets navmesh used by the world
			if (NavSys)
			{
				// Gets a random vector within a radious of 200.0f of the enemy pawn
				FVector NavTarget = NavSys->GetRandomPointInNavigableRadius(GetWorld(), PawnAsNPC->GetActorLocation(), 200.0f);
				// Sets Blackboard vairables Target to NavTarget and HasTarget to true
				Controller->GetBBoard()->SetValueAsVector(FName(TEXT("Target")), NavTarget);
				return EBTNodeResult::Type::Succeeded; // Returns Succeeded
			}
		}
	}
	return EBTNodeResult::Type::Failed; // Returns Failed
}
