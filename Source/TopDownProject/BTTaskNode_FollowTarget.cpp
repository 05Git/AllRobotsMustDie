#include "BTTaskNode_FollowTarget.h"
#include "FollowActorController.h"
#include "FollowActor.h"

EBTNodeResult::Type UBTTaskNode_FollowTarget::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	Controller = Cast<AFollowActorController>(OwnerComp.GetAIOwner());
	if (Controller != nullptr)
	{
		Actor = Cast<AFollowActor>(Controller->GetPawn());
		if (Actor != nullptr)
		{
			FVector Position = Actor->GetActorLocation();
			FVector Target = Actor->GetTarget()->GetActorLocation();
			FVector VelocityOut;
			float Magnitude;
			VelocityOut.X = Target.X - Position.X;
			VelocityOut.Y = Target.Y - Position.Y;
			VelocityOut.Z = Target.Z - Position.Z;
			Magnitude = FMath::Sqrt((VelocityOut.X * VelocityOut.X)
				+ (VelocityOut.Y * VelocityOut.Y)
				+ (VelocityOut.Z * VelocityOut.Z));
			if (Magnitude < Actor->GetSatisfactionRad())
			{
				VelocityOut.X = 0.0f;
				VelocityOut.Y = 0.0f;
				VelocityOut.Z = 0.0f;
				return EBTNodeResult::Type::Succeeded;
			}
			VelocityOut.X /= Actor->GetTimeToTarget();
			VelocityOut.Y /= Actor->GetTimeToTarget();
			VelocityOut.Z /= Actor->GetTimeToTarget();
			Magnitude = FMath::Sqrt((VelocityOut.X * VelocityOut.X)
				+ (VelocityOut.Y * VelocityOut.Y)
				+ (VelocityOut.Z * VelocityOut.Z));
			if (Magnitude > Actor->GetMaxSpeed())
			{
				VelocityOut.X = VelocityOut.X / Magnitude;
				VelocityOut.Y = VelocityOut.Y / Magnitude;
				VelocityOut.Z = VelocityOut.Z / Magnitude;
				VelocityOut.X *= Actor->GetMaxSpeed();
				VelocityOut.Y *= Actor->GetMaxSpeed();
				VelocityOut.Z *= Actor->GetMaxSpeed();
			}
		}
	}
	return EBTNodeResult::Type::Failed;
}
