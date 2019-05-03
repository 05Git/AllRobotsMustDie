#include "BTTask_CheckPlayerView.h"
#include "BEnemyController.h"
#include "BasicEnemy.h"
#include "PlayerChar.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_CheckPlayerView::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	Controller = Cast<ABEnemyController>(OwnerComp.GetAIOwner());
	// Checks if Controller is not a nullptr
	if (Controller != nullptr)
	{
		PawnAsNPC = Cast<ABasicEnemy>(Controller->GetPawn());
		// Checks if PawnAsNPC is not a nullptr
		if (PawnAsNPC != nullptr)
		{
			Player = Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			// Checks if Player is not a nullptr
			if (Player != nullptr)
			{
				FRotator PlayerRot = Player->GetViewRotation(); // Gets the direction the player is looking in
				
				return EBTNodeResult::Type::Succeeded;
			}
		}
	}
	return EBTNodeResult::Type::Failed;
}
