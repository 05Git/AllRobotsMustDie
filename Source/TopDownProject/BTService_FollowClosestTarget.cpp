#include "BTService_FollowClosestTarget.h"
#include "FollowActor.h"
#include "FollowActorController.h"
#include "BasicEnemy.h"
#include "PlayerChar.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

void UBTService_FollowClosestTarget::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaTime)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaTime);

	Controller = Cast<AFollowActorController>(OwnerComp.GetAIOwner());
	if (Controller != nullptr)
	{
		Actor = Cast<AFollowActor>(Controller->GetPawn());
		if (Actor != nullptr)
		{
			ClosestActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			ClosestDistance = 500.0f;
			for (TActorIterator<ABasicEnemy> Iterator(GetWorld()); Iterator; ++Iterator)
			{
				float Distance = Iterator->CalcDist(Actor->GetActorLocation(), Iterator->GetActorLocation());
				if (Distance < ClosestDistance)
				{
					if (ABasicEnemy *Actor = Cast<ABasicEnemy>(Iterator->Raycast(Actor->GetActorLocation(), Iterator->GetActorLocation())))
					{
						ClosestActor = *Iterator;
						ClosestDistance = Distance;
					}
				}
			}
			Actor->SetTarget(ClosestActor);
		}
	}
}
