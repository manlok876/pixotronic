// Copyright 2019 Denis Maximenko


#include "PT_BaseBike.h"

#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UnrealNetwork.h"

#include "PT_ArenaPlayerState.h"
#include "PT_TrailingComponent.h"

const FName APT_BaseBike::TurnLeftBinding("TurnLeft");
const FName APT_BaseBike::TurnRightBinding("TurnRight");
const FName APT_BaseBike::UseAbilityBinding("SpecialAbility");

const FVector APT_BaseBike::BikeMeshOffset(20.0f, 0.0f, 0.0f);

const float APT_BaseBike::TurningTouchArea = 0.4;

const FName APT_BaseBike::ColorParameterName("Color");

APT_BaseBike::APT_BaseBike()
{
	PrimaryActorTick.bCanEverTick = true;
	bAlwaysRelevant = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	// Init components

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	check(RootComponent != nullptr);
	
	TrailingComponent = CreateDefaultSubobject<UPT_TrailingComponent>(TEXT("TrailingComponent"));

	check(TrailingComponent != nullptr);
	TrailingComponent->bEditableWhenInherited = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

	check(CollisionBox != nullptr);
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(50, 50, 50));
	CollisionBox->SetRelativeLocation(FVector(50, 0, 0) + BikeMeshOffset);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	
	check(MeshComponent != nullptr);
	MeshComponent->bEditableWhenInherited = true;
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeLocation(BikeMeshOffset);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	IsDead = false;
	IsInputEnabled = true;
}

void APT_BaseBike::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APT_BaseBike, IsDead);
	DOREPLIFETIME(APT_BaseBike, IsInputEnabled);
}

void APT_BaseBike::OnConstruction(const FTransform& Transform)
{

}

void APT_BaseBike::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APT_BaseBike::OnCollide);
	}
}

void APT_BaseBike::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	APT_ArenaPlayerState* PossessingPlayerState = GetPlayerState<APT_ArenaPlayerState>();
	if (IsValid(PossessingPlayerState))
	{
		SetColor(PossessingPlayerState->PlayerColor);
	}

	SetOwner(NewController);
}

void APT_BaseBike::UpdateTransform_Implementation(const FTransform& NewTransform)
{
	SetActorTransform(NewTransform);
}

void APT_BaseBike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority()) {
		AddActorWorldOffset(GetActorForwardVector() * Speed * DeltaTime);
		UpdateTransform(GetActorTransform());
	}
}

void APT_BaseBike::SetColor_Implementation(const FLinearColor& NewColor)
{
	BikeColor = NewColor;

	if (!IsValid(MeshComponent))
	{
		return;
	}

	UMaterialInstanceDynamic* NewMaterial = UMaterialInstanceDynamic::Create(MeshComponent->GetMaterial(0), this);
	if (NewMaterial != nullptr)
	{
		NewMaterial->SetVectorParameterValue(ColorParameterName, NewColor);
		MeshComponent->SetMaterial(0, NewMaterial);
	}

	if (IsValid(TrailingComponent))
	{
		TrailingComponent->SetColor(NewColor);
	}
}

void APT_BaseBike::HandleTouchInput(ETouchIndex::Type FingerIndex, FVector Location)
{
	check(GEngine != nullptr);
	check(GEngine->GameViewport != nullptr);

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	float LeftTurnThreshold = ViewportSize.X * TurningTouchArea;
	float RightTurnThreshold = ViewportSize.X * (1 - TurningTouchArea);

	if (Location.X < LeftTurnThreshold)
	{
		TurnLeft();
	}
	else if (Location.X > RightTurnThreshold)
	{
		TurnRight();
	}
}

void APT_BaseBike::SetTurningEnabled_Implementation(bool Enable)
{
	UpdateTransform(GetActorTransform());
	IsInputEnabled = Enable;
}

bool APT_BaseBike::SetTurningEnabled_Validate(bool Enable)
{
	return !IsDead;
}

void APT_BaseBike::TurnLeft_Implementation()
{
	if (HasAuthority())
	{
		AddActorWorldRotation(FRotator(0, -90, 0));
		UpdateTransform(GetActorTransform());
		if (IsValid(TrailingComponent))
		{
			TrailingComponent->MakeTurnpoint();
		}
	}
}

bool APT_BaseBike::TurnLeft_Validate()
{
	return IsInputEnabled;
}

void APT_BaseBike::TurnRight_Implementation()
{
	if (HasAuthority())
	{
		AddActorWorldRotation(FRotator(0, 90, 0));
		UpdateTransform(GetActorTransform());
		if (IsValid(TrailingComponent))
		{
			TrailingComponent->MakeTurnpoint();
		}
	}
}

bool APT_BaseBike::TurnRight_Validate()
{
	return IsInputEnabled;
}

void APT_BaseBike::OnCollide_Implementation(UPrimitiveComponent* OverlappedComponent,
							 AActor* OtherActor, UPrimitiveComponent* OtherComp, 
							 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetActorTickEnabled(false);
	if (HasAuthority())
	{
		IsDead = true;
		SetTurningEnabled(false);
		if (IsValid(CollisionBox))
		{
			CollisionBox->OnComponentBeginOverlap.RemoveAll(this);
		}
	}

	OnDeath.Broadcast(this);
}

void APT_BaseBike::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TurnLeftBinding, IE_Pressed, this, &APT_BaseBike::TurnLeft);
	PlayerInputComponent->BindAction(TurnRightBinding, IE_Pressed, this, &APT_BaseBike::TurnRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &APT_BaseBike::HandleTouchInput);
}
