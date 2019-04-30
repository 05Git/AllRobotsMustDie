#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BEnemyController.generated.h"

enum states
{
	SPAWN,
	IDLE,
	CHASE,
	RETREAT,
	ATTACK,
	DIE
};

enum events
{
	onEnter,
	onUpdate,
	onExit
};

/**
 * BEnemyController
 * AI controller for BasicEnemy
 */
UCLASS()
class TOPDOWNPROJECT_API ABEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	/** Sets default values for this character's properties */
	ABEnemyController();
	
protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "NPC")
		class ABasicEnemy *PawnAsNPC; /**< BasicEnemy for this controller to possess */
	
	UPROPERTY(BlueprintReadWrite, Category = "Behavior")
		class UBlackboardComponent *BlackboardComp; /**< BlackboardComponent for this controller to use, taken from possessed BasicEnemy */
	
	UPROPERTY(BlueprintReadWrite, Category = "Behavior")
		class UBehaviorTreeComponent *BehaviorTreeComp; /**< BehaviourTreeComponent for this controller to use, taken from possessed BasicEnemy */

public:
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Possesses a Pawn */
	virtual void Possess(APawn* InPawn) override;

	/** Unpossesses currently possessed pawn */
	virtual void UnPossess() override;

	/** Sets PawnAsNPC->Alert to false */
	void Alertness();

	/** Returns CurrentState */
	states GetCurrentState();

	/** Returns BBoard */
	UBlackboardComponent *GetBBoard();

	/** Returns BTree */
	UBehaviorTreeComponent *GetBTree();

private:
	float Distance; /**< Distance determines how far away the player is from the possessed pawn */

	float FOV; /**< FOV determines if the player is in the possessed pawn's field of vision */

	float LengthOfSight; /**< LengthOfSight determines the maximum distance at which the possessed pawn is able to see the player */

	bool InFOV; /**< InFOV determines if the player currently in the possessed pawn's field of vision */

	states CurrentState; /**< CurrentState determines the state the controller is currently in */

	states NextState; /**< NextState determines the next state for the controller to transition into */

	events CurrentEvent; /**< CurrentEvent determines the current event of this controller's state */

	FTimerHandle AlertCoolDown; /**< AlertCoolDown determines how long to wait to call Alertness */
};
