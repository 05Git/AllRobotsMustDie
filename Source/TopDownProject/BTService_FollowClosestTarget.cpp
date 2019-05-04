#include "BTService_FollowClosestTarget.h"
#include "FollowActor.h"
#include "FollowActorController.h"
#include "BasicEnemy.h"
#include "PlayerChar.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine.h"

void UBTService_FollowClosestTarget::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaTime)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaTime);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Service starting")));
	}
	Controller = Cast<AFollowActorController>(OwnerComp.GetAIOwner());
	if (Controller != nullptr)
	{
		Actor = Cast<AFollowActor>(Controller->GetPawn());
		if (Actor != nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Finding closest actor...")));
			}
			ClosestActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			ClosestDistance = 500.0f;
			for (TActorIterator<ABasicEnemy> Iterator(GetWorld()); Iterator; ++Iterator)
			{
				float Distance = Iterator->CalcDist(Actor->GetActorLocation(), Iterator->GetActorLocation());
				if (Distance < ClosestDistance)
				{
					if (ABasicEnemy *OtherActor = Cast<ABasicEnemy>(Iterator->Raycast(Actor->GetActorLocation(), Iterator->GetActorLocation())))
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
