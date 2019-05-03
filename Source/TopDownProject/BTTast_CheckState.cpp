#include "BTTast_CheckState.h"
#include "BEnemyController.h"
#include "BasicEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTast_CheckState::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	Controller = Cast<ABEnemyController>(OwnerComp.GetAIOwner());
	// Checks if Controller is not a nullptr
	if (Controller != nullptr)
	{
		PawnAsNPC = Cast<ABasicEnemy>(Controller->GetPawn());
		// Checks if PawnAsNPC is not a nullptr
		if (PawnAsNPC != nullptr)
		{
			// Checks if Controller's CurrentState is set to Idle
			if (Controller->GetCurrentState() == IDLE)
			{
				Controller->GetBBoard()->SetValueAsBool(FName(TEXT("IsIdle")), true); // Sets Blackboard bool IsIdle to true
				Controller->GetBBoard()->SetValueAsBool(FName(TEXT("IsChasing")), false); // Sets Blackboard bool IsChasing to false
			}
			else if (Controller->GetCurrentState() == CHASE)
			{
				Controller->GetBBoard()->SetValueAsBool(FName(TEXT("IsIdle")), false); // Sets Blackboard bool IsIdle to false
				Controller->GetBBoard()->SetValueAsBool(FName(TEXT("IsChasing")), true); // Sets Blackboard bool IsChasing to true
			}
			else
			{
				Controller->GetBBoard()->SetValueAsBool(FName(TEXT("IsIdle")), false); // Sets Blackboard bool IsIdle to false
				Controller->GetBBoard()->SetValueAsBool(FName(TEXT("IsChasing")), false); // Sets Blackboard bool IsChasing to false
			}
			return EBTNodeResult::Type::Succeeded; // Returns Succeeded
		}
	}
	return EBTNodeResult::Type::Failed; // Returns Failed
}
