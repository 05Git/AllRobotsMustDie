#include "BTTask_SetTargetLocation.h"
#include "BEnemyController.h"
#include "BasicEnemy.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine.h"

EBTNodeResult::Type UBTTask_SetTargetLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	Controller = Cast<ABEnemyController>(OwnerComp.GetAIOwner());
	if (Controller != nullptr)
	{
		PawnAsNPC = Cast<ABasicEnemy>(Controller->GetPawn());
		if (PawnAsNPC != nullptr)
		{
			UNavigationSystemV1 *NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			if (NavSys)
			{
				FVector NavTarget = NavSys->GetRandomPointInNavigableRadius(GetWorld(), PawnAsNPC->GetActorLocation(), 200.0f);
				Controller->GetBBoard()->SetValueAsVector(FName(TEXT("Target")), NavTarget);
				Controller->GetBBoard()->SetValueAsBool(FName(TEXT("HasTarget")), true);
				return EBTNodeResult::Type::Succeeded;
			}
		}
	}
	return EBTNodeResult::Type::Failed;
}
