#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_FollowTarget.generated.h"

/**
 * FollowTarget
 * Moves FollowActor towards its target
 */
UCLASS()
class TOPDOWNPROJECT_API UBTTaskNode_FollowTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
	
private:
	class AFollowActorController *Controller;

	class AFollowActor *Actor;
};
