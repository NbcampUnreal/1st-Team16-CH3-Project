// Fill out your copyright notice in the Description page of Project Settings.


#include "TSBaseItem.h"
#include "Components/SphereComponent.h"
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

	// 콜리전 컴포넌트
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);

	// 스태틱 메쉬 컴포넌트
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	//이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ATSBaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ATSBaseItem::OnItemEndOverlap);
}


// 아이템이 겹쳐질 때 호출되는 함수
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
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("EndOverlap!")));
	ActivateItem(OtherActor);
}

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
