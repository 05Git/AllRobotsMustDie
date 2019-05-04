#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FollowActorController.generated.h"

/**
 * FollowActorController
 * AIController used to control FollowActor
 */
UCLASS()
class TOPDOWNPROJECT_API AFollowActorController : public AAIController
{
	GENERATED_BODY()
	
public:
	/** Sets default values for this object's properties */
	AFollowActorController();
	
protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadonly, Category = "Pawn")
		class AFollowingPawn *PawnAsNPC; /**< Actor for the controller to possess */

	UPROPERTY(BlueprintReadWrite, Category = "Behavior")
		class UBlackboardComponent *BlackboardComp; /**< BlackboardComponent for this controller to use, taken from possessed actor */

	UPROPERTY(BlueprintReadWrite, Category = "Behavior")
		class UBehaviorTreeComponent *BehaviorTreeComp; /**< BehaviourTreeComponent for this controller to use, taken from possessed actor */

public:
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Possesses a Pawn */
	virtual void Possess(APawn *InPawn) override;

	/** Unpossesses currently possessed pawn */
	virtual void UnPossess() override;

	void SelectTarget();

	void Move();
	
	UBlackboardComponent *GetBBoard();

	UBehaviorTreeComponent *GetBTree();
};
