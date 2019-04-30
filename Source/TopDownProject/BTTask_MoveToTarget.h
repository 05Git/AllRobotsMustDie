#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToTarget.generated.h"

/**
 * MoveToTarget
 * Moves an enemy to a target
 */
UCLASS()
class TOPDOWNPROJECT_API UBTTask_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	/** Performs task, returns succeeded if successful or failed if failure */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
	
private:
	class ABEnemyController *Controller; /**< Pointer to enemy AI controller */

	class ABasicEnemy *PawnAsNPC; /**< Pointer to enemy */
};
