#include "BTTask_MoveToTarget.h"
#include "BEnemyController.h"
#include "BasicEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine.h"

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	Controller = Cast<ABEnemyController>(OwnerComp.GetAIOwner());
	// Checks if Controller is not a nullptr
	if (Controller != nullptr)
	{
		PawnAsNPC = Cast<ABasicEnemy>(Controller->GetPawn());
		// Checks if PawnAsNPC is not a nullptr
		if (PawnAsNPC != nullptr)
		{
			// Gets target vector from blackboard
			FVector Target = Controller->GetBBoard()->GetValueAsVector(TEXT("Target"));
			// Requests enemy to move to target using navmesh
			EPathFollowingRequestResult::Type MoveToTarget = Controller->MoveToLocation(Target, 0.0f, true, true, false, false, 0, true);
			// Checks if request is successful or already at goal
			if (MoveToTarget == EPathFollowingRequestResult::Type::AlreadyAtGoal || MoveToTarget == EPathFollowingRequestResult::Type::RequestSuccessful)
			{
				Controller->GetBBoard()->SetValueAsBool(FName(TEXT("HasTarget")), false); // Sets Blackboard bool HasTarget to false
				return EBTNodeResult::Type::Succeeded; // Returns Succeeded
			}
		}
	}
	return EBTNodeResult::Type::Failed; // Returns Failed
}
