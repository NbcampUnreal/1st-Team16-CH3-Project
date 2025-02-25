// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "AI/EnemyCharacter.h"
#include "Navigation/PathFollowingComponent.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	MoveToPatrolPoint();
}


void AEnemyAIController::MoveToPatrolPoint()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetPawn());
	if (!EnemyCharacter)
	{
		return;
	}

	if (EnemyCharacter->PatrolPoints.Num() == 0)
	{
		return;
	}

	MoveToActor(
		EnemyCharacter->PatrolPoints[CurrentPatrolIndex], // 이동할 위치
		5.0f,	// 목표지점으로부터 몇 유닛 이내에 도달하면 멈출지
		true,	// 목표와 겹치면 스탑
		true,	// 가는 도중 장애물이 있으면 회피해서 이동할지
		false,	// 기본 이동모드에서 좌우 회전 없이 이동할지 (true면 오로지 직진)
		nullptr,// 경로 필터
		true	// 경로를 못찾았으니 부분 경로가 있으면 이동할지 여부
	);

	CurrentPatrolIndex = (CurrentPatrolIndex + 1) % EnemyCharacter->PatrolPoints.Num();
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.Code == EPathFollowingResult::Success)
	{
		MoveToPatrolPoint();
	}
}
