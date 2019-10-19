// Fill out your copyright notice in the Description page of Project Settings.


#include "PT_BaseBike.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"

#include "PT_TrailingComponent.h"

const FName APT_BaseBike::TurnLeftBinding("TurnLeft");
const FName APT_BaseBike::TurnRightBinding("TurnRight");
const FName APT_BaseBike::UseAbilityBinding("SpecialAbility");

const FVector APT_BaseBike::BikeMeshOffset(120.0f, 0.0f, 0.0f);

APT_BaseBike::APT_BaseBike()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	// Init components

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	check(RootComponent != nullptr);
	
	TrailingComponent = CreateDefaultSubobject<UPT_TrailingComponent>(TEXT("TrailingComponent"));

	check(TrailingComponent != nullptr);
	TrailingComponent->bEditableWhenInherited = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

	check(CollisionBox != nullptr);
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(100, 50, 50));
	CollisionBox->SetRelativeLocation(BikeMeshOffset);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	
	check(MeshComponent != nullptr);
	MeshComponent->bEditableWhenInherited = true;
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeLocation(BikeMeshOffset);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BikeMesh(TEXT("/Game/Assets/Meshes/Sphere.Sphere"));
	if (BikeMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(BikeMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> BikeMaterial(TEXT("/Game/Assets/Materials/BikeMaterial.BikeMaterial")); 
	if (BikeMesh.Succeeded())
	{
		MeshComponent->SetMaterial(0, BikeMaterial.Object);
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
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APT_BaseBike::OnCollide);
}

void APT_BaseBike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority()) {
		AddActorWorldOffset(GetActorForwardVector() * Speed * DeltaTime);
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
	MeshComponent->SetHiddenInGame(true);

	CollisionBox->SetHiddenInGame(true);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorTickEnabled(false);
}

void APT_BaseBike::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TurnLeftBinding, IE_Pressed, this, &APT_BaseBike::TurnLeft);
	PlayerInputComponent->BindAction(TurnRightBinding, IE_Pressed, this, &APT_BaseBike::TurnRight);
}
