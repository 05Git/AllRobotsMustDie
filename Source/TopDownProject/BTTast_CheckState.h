#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTast_CheckState.generated.h"

/**
 * CheckState
 * Checks the state of an enemy to determine what steps to take
 */
UCLASS()
class TOPDOWNPROJECT_API UBTTast_CheckState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	/** Performs task, returns succeeded if successful or failed if failure */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

private:
	class ABEnemyController *Controller; /**< Pointer to enemy AI controller */

	class ABasicEnemy *PawnAsNPC; /**< Pointer to enemy */
};
