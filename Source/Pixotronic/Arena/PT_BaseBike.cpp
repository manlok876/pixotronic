// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_BaseBike.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"

#include "PT_TrailingComponent.h"

const FName APT_BaseBike::TurnLeftBinding("TurnLeft");
const FName APT_BaseBike::TurnRightBinding("TurnRight");

const FVector APT_BaseBike::BikeMeshOffset(100.0f, 0.0f, 0.0f);

APT_BaseBike::APT_BaseBike()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	// Init components

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

	check(CollisionBox != nullptr);
	RootComponent = CollisionBox;
	CollisionBox->SetBoxExtent(FVector(100, 50, 50));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APT_BaseBike::OnCollide);

	TrailingComponent = CreateDefaultSubobject<UPT_TrailingComponent>(TEXT("TrailingComponent"));

	check(TrailingComponent != nullptr);
	TrailingComponent->SetupAttachment(RootComponent);
	TrailingComponent->SetRelativeLocation(-BikeMeshOffset);
	TrailingComponent->bEditableWhenInherited = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	
	check(MeshComponent != nullptr);
	MeshComponent->bEditableWhenInherited = true;
	MeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BikeMesh(TEXT("/Game/Geometry/Meshes/Sphere.Sphere"));
	if (BikeMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(BikeMesh.Object);
	}
	MeshComponent->SetRelativeScale3D(FVector(2.0f, 1.0f, 1.0f));
	MeshComponent->SetSimulatePhysics(false);


	// Init other variables

	Speed = 500;
}

void APT_BaseBike::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APT_BaseBike::OnConstruction(const FTransform& Transform)
{

}

void APT_BaseBike::BeginPlay()
{
	Super::BeginPlay();

}

void APT_BaseBike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority()) {
		AddActorLocalOffset(FVector(1, 0, 0) * Speed * DeltaTime);
	}
}

void APT_BaseBike::TurnLeft()
{
	AddActorWorldRotation(FRotator(0, -90, 0));
	if (IsValid(TrailingComponent))
	{
		TrailingComponent->MakeTurnpoint();
	}
}

void APT_BaseBike::TurnRight()
{
	AddActorWorldRotation(FRotator(0, 90, 0));
	if (IsValid(TrailingComponent))
	{
		TrailingComponent->MakeTurnpoint();
	}
}

void APT_BaseBike::OnCollide(UPrimitiveComponent* OverlappedComponent, 
							 AActor* OtherActor, UPrimitiveComponent* OtherComp, 
							 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Destroy();
}

void APT_BaseBike::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TurnLeftBinding, IE_Pressed, this, &APT_BaseBike::TurnLeft);
	PlayerInputComponent->BindAction(TurnRightBinding, IE_Pressed, this, &APT_BaseBike::TurnRight);
}
