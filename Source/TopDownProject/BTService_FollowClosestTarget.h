#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FollowClosestTarget.generated.h"

/**
 * FollowClosestTarget
 * Finds the closest target for FollowActor to follow
 * Defaults to the player if there are no other available targets
 */
UCLASS()
class TOPDOWNPROJECT_API UBTService_FollowClosestTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaTime) override;
	
private:
	class AFollowActorController *Controller;

	class AFollowActor *Actor;

	AActor *ClosestActor;

	float ClosestDistance;
};
