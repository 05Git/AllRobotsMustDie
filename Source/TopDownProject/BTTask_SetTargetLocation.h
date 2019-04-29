#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetTargetLocation.generated.h"

/**
 * SetTargetLocation
 * Sets a location for an enemy to walk towards while in Idle state
 */
UCLASS()
class TOPDOWNPROJECT_API UBTTask_SetTargetLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
	
private:
	class ABEnemyController *Controller;

	class ABasicEnemy *PawnAsNPC;
};
