#include "BTTast_CheckState.h"
#include "BEnemyController.h"
#include "BasicEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTast_CheckState::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	Controller = Cast<ABEnemyController>(OwnerComp.GetAIOwner());
	if (Controller != nullptr)
	{
		PawnAsNPC = Cast<ABasicEnemy>(Controller->GetPawn());
		if (PawnAsNPC != nullptr)
		{
			if (Controller->GetCurrentState() == IDLE)
			{
				Controller->GetBBoard()->SetValueAsBool(FName(TEXT("IsIdle")), true);
			}
			else
			{
				Controller->GetBBoard()->SetValueAsBool(FName(TEXT("IsIdle")), false);
			}
			return EBTNodeResult::Type::Succeeded;
		}
	}
	return EBTNodeResult::Type::Failed;
}
