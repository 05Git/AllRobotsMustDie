#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckPlayerView.generated.h"

/**
 * CheckPlayerView
 * Checks if the enemy is being aimed at by the player
 */
UCLASS()
class TOPDOWNPROJECT_API UBTTask_CheckPlayerView : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	/** Performs task, returns succeeded if successful or failed if failure */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

private:
	class ABEnemyController *Controller; /**< Pointer to enemy AI controller */

	class ABasicEnemy *PawnAsNPC; /**< Pointer to enemy */

	class APlayerChar *Player; /**< Pointer to the player character */
};
