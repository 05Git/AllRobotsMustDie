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
		class ABasicEnemy *PawnAsNPC;

	UPROPERTY(BlueprintReadWrite, Category = "Behavior")
		class UBlackboardComponent *BlackboardComp;

	UPROPERTY(BlueprintReadWrite, Category = "Behavior")
		class UBehaviorTreeComponent *BehaviorTreeComp;

public:
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	virtual void Possess(APawn* InPawn) override;

	virtual void UnPossess() override;

	void Alertness();

	states GetCurrentState();

	UBlackboardComponent *GetBBoard();

	UBehaviorTreeComponent *GetBTree();

private:
	float Distance;

	float FOV;

	float LengthOfSight;

	bool InFOV;

	states CurrentState;

	states NextState;

	events CurrentEvent;

	FTimerHandle AlertCoolDown;
};
