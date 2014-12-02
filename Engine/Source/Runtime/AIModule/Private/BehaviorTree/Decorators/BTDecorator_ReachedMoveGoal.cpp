// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "AIModulePrivate.h"
#include "BehaviorTree/Decorators/BTDecorator_ReachedMoveGoal.h"

UBTDecorator_ReachedMoveGoal::UBTDecorator_ReachedMoveGoal(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Reached move goal";

	// can't abort, it's not observing anything
	bAllowAbortLowerPri = false;
	bAllowAbortNone = false;
	bAllowAbortChildNodes = false;
	FlowAbortMode = EBTFlowAbortMode::None;
}

bool UBTDecorator_ReachedMoveGoal::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const 
{
	AAIController* AIOwner = OwnerComp.GetAIOwner();
	const bool bReachedGoal = AIOwner && AIOwner->GetPathFollowingComponent() && AIOwner->GetPathFollowingComponent()->DidMoveReachGoal();
	return bReachedGoal;
}

#if WITH_EDITOR

FName UBTDecorator_ReachedMoveGoal::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Decorator.ReachedMoveGoal.Icon");
}

#endif	// WITH_EDITOR
