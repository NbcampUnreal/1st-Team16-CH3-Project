// Fill out your copyright notice in the Description page of Project Settings.


#include "TSBaseItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h" 
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATSBaseItem::ATSBaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 씬 컴포넌트
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	// 충돌 감지 스피어 컴포넌트 (아이템 획득 범위)
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);

	// 감지 범위 (아웃라인 표시용)
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetSphereRadius(200.0f); // 감지 범위 설정
	DetectionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DetectionSphere->SetupAttachment(Scene);

	// 스태틱 메쉬 컴포넌트
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	// 스태틱 메시의 충돌 감지 완전 비활성화
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetGenerateOverlapEvents(false);

	//이벤트 바인딩
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATSBaseItem::OnDetectionOverlap);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ATSBaseItem::OnDetectionEndOverlap);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ATSBaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ATSBaseItem::OnItemEndOverlap);

	OutlineMaterial = nullptr;
	OriginalMaterial = nullptr;
	bShowOutline = true;
}

//--------------------------- 아웃라인 온오프 -------------------------------

// 감지 범위에 들어오면 테두리 활성화
void ATSBaseItem::OnDetectionOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player") && bShowOutline)
	{
		if (StaticMesh)
		{
			// 기존 머티리얼을 저장 (MID가 아닌 원래 머티리얼을 가져와야 함)
			if (!OriginalMaterial)
			{
				OriginalMaterial = StaticMesh->GetMaterial(0);
			}

			if (OriginalMaterial)
			{
				// 새로운 MaterialInstanceDynamic 생성
				OutlineMaterial = UMaterialInstanceDynamic::Create(OriginalMaterial, this);
				if (OutlineMaterial)
				{
					OutlineMaterial->SetScalarParameterValue(TEXT("OutlineWidth"), 15.0f);
					OutlineMaterial->SetVectorParameterValue(TEXT("OutlineColor"), FLinearColor::Yellow);
					StaticMesh->SetMaterial(0, OutlineMaterial);
				}
			}
		}
	}
}

// 감지 범위를 벗어나면 테두리 제거
void ATSBaseItem::OnDetectionEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Player") && StaticMesh && OutlineMaterial)
	{
		// 기존 머티리얼로 복구 (MID가 아닌 원래 머티리얼을 사용해야 함)
		if (OriginalMaterial)
		{
			StaticMesh->SetMaterial(0, OriginalMaterial);
		}
	}
}

//--------------------------------------------------------------------

// 아이템과 겹쳐질 때 호출되는 함수 (ex.획득)
void ATSBaseItem::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		// 디버깅 메시지 출력
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!")));
		ActivateItem(OtherActor);
	}
}

// 벗어났을 때 호출되는 함수
void ATSBaseItem::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
		
}

// --------------------------------------------------------------------

// 아이템 활성화 시 발동 함수
void ATSBaseItem::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle = nullptr;

	if (PickupParticle)
	{
		// 파티클 생성
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),	// 파티클을 생성할 월드
			PickupParticle, // 생성할 파티클 에셋
			GetActorLocation(), // 아이템 위치값
			GetActorRotation(), // 아이템 회전값
			true // 파티클이 끝나면 자동으로 사라질지 여부
		);
	}

	if (PickupSound)
	{
		// 사운드 재생
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(), // 사운드를 재생할 월드
			PickupSound, // 재생할 사운드
			GetActorLocation() // 사운드 재생 위치
		);
	}

	if (Particle)
	{
		// 파티클이 끝나면 자동으로 사라지도록 설정
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[Particle]() //람다 함수
			{
				Particle->DeactivateSystem();
			},
			2.0f,
			false
		);
	}
}

// 아이템 제거 함수
void ATSBaseItem::DestroyItem()
{
	Destroy();
}

// 아이템 타입 반환 함수
FName ATSBaseItem::GetItemType() const
{
	return ItemType;
}
