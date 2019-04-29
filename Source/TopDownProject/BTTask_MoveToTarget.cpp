#include "BTTask_MoveToTarget.h"
#include "BEnemyController.h"
#include "BasicEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine.h"

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	Controller = Cast<ABEnemyController>(OwnerComp.GetAIOwner());
	if (Controller != nullptr)
	{
		PawnAsNPC = Cast<ABasicEnemy>(Controller->GetPawn());
		if (PawnAsNPC != nullptr)
		{
			FVector Target = Controller->GetBBoard()->GetValueAsVector(TEXT("Target"));
			EPathFollowingRequestResult::Type MoveToTarget = Controller->MoveToLocation(Target, 0.0f, true, true, false, false, 0, true);
			if (MoveToTarget == EPathFollowingRequestResult::Type::AlreadyAtGoal || MoveToTarget == EPathFollowingRequestResult::Type::RequestSuccessful)
			{
				Controller->GetBBoard()->SetValueAsBool(FName(TEXT("HasTarget")), false);
				return EBTNodeResult::Type::Succeeded;
			}
		}
	}
	return EBTNodeResult::Type::Failed;
}
