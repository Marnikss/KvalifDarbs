// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_NPC/CilveksBase.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "../../../../Intermediate/ProjectFiles/Consts.h"
#include <HealthBar/HealthBarWidget.h>
#include <AI_NPC/NPC.h>
#include <Speele/Cilveks.h>
#include "Kismet/GameplayStatics.h"



// Sets default values
ACilveksBase::ACilveksBase() : 
	WidgetComponent{CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue"))},
	Health{MaxHealth},
	RightFistCollisionBox{CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox"))}
	
{
	
	

	PrimaryActorTick.bCanEverTick = true;
	if (WidgetComponent)
	{
		WidgetComponent->SetupAttachment(RootComponent);//RootComponent
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComponent->SetRelativeLocation(defs::HealthBarZ);
		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass{ TEXT("/Game/UI/BP_HealthBar") };
		if (WidgetClass.Succeeded())
		{
			WidgetComponent->SetWidgetClass((WidgetClass.Class));
		}
	}
	if (RightFistCollisionBox)
	{
		RightFistCollisionBox->SetBoxExtent(defs::RightFistBoxSize, false);
		FAttachmentTransformRules const Rules{
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false };
		RightFistCollisionBox->AttachToComponent(GetMesh(), Rules, defs::RightFistSocketName);
		RightFistCollisionBox->SetRelativeLocation(defs::CollisionBoxLocation);
	} 
}


// Called when the game starts or when spawned
void ACilveksBase::BeginPlay()
{
	
	Super::BeginPlay();


	
	RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACilveksBase::OnAttackOverlapBegin);
	RightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACilveksBase::OnAttackOverlapEnd);

	//UBoxComponent* AxeBladeColBox = FindComponentByClass<UBoxComponent>();
	
	
	//AxeBladeColBox->OnComponentBeginOverlap.AddDynamic(this, &ACilveksBase::PlayerAttackOverlapBegin);
	//AxeBladeColBox->OnComponentEndOverlap.AddDynamic(this, &ACilveksBase::PlayerAttackOverlapEnd);

	
}






void ACilveksBase::OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex, bool const FromSweep, FHitResult const& SweepResult)
{
	
	if (OtherActor == this)
	{
		//UE_LOG(LogTemp, Warning, TEXT("BIG OOPS"));
		return;
	}
	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("OnAttackOverlapBegin called"));
	//UE_LOG(LogTemp, Warning, TEXT("OnAttackOverlapBegin called"));
	// Check if the overlapping actor is an NPC
	//if (auto const Enemy = Cast<ANPC>(OtherActor))
	//{
		//auto const NewHealth = Enemy->GetHealth() - Enemy->GetMaxHealth() * 0.1f;
		//Enemy->SetHealth(NewHealth);
	//}
	// Check if the overlapping actor is the player character
	else if (auto const Player = Cast<ACilveks>(OtherActor))
	{
		auto const NewHealth = Player->GetHealth() - Player->GetMaxHealth() * 0.01f;
		Player->SetHealth(NewHealth);
	}
}


void ACilveksBase::OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* OtherComponent, int const OtherBodyIndex)
{
	
	
}


/* void ACilveksBase::OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex, bool const FromSweep, FHitResult const& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnAttackOverlapBegin called"));
	// Ignore overlaps with self
	//if (OtherActor == this)
	//{
		//UE_LOG(LogTemp, Warning, TEXT("BIG OOPS"));
		//return;
	//}
	// Check if the overlapping actor is an NPC
	//if (auto const Enemy = Cast<ANPC>(OtherActor))
	//{
		// Ignore overlaps with other NPCs

		//return;
	//}

	
	UE_LOG(LogTemp, Warning, TEXT("OnAttackOverlapBegin called"));
	// Check if the overlapping actor is an NPC
	if (auto const Enemy = Cast<ANPC>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("OtherActor class: %s"), *OtherActor->GetClass()->GetName());
		UE_LOG(LogTemp, Warning, TEXT("OnAttackOverlapBegin called"));
		// Deal damage to the NPC
		auto const NewHealth = Enemy->GetHealth() - Enemy->GetMaxHealth() * 0.1f;
		Enemy->SetHealth(NewHealth);
		UE_LOG(LogTemp, Warning, TEXT("NPCS NewHealth: %f"), NewHealth); // Log the new health value
	}
	// Check if the overlapping actor is the player character
	else if (auto const Player = Cast<ACilveks>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("playercharacter is the overlapping actor"));
		// Store the player character reference for later use
		CurrentlyOverlappingPlayer = Player;
	}
}


void ACilveksBase::OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* OtherComponent, int const OtherBodyIndex)
{
	
	// Check if the overlapping actor is the player character
	if (auto const Player = Cast<ACilveks>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("NPC HITTING OtherActor class: %s"), *OtherActor->GetClass()->GetName());
		// Check if the stored player character reference matches the overlapping actor
		if (Player == CurrentlyOverlappingPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnAttackOverlapEnd called"));
			// Deal damage to the player character
			auto const NewHealth = Player->GetHealth() - Player->GetMaxHealth() * 0.01f;
			Player->SetHealth(NewHealth);
			UE_LOG(LogTemp, Warning, TEXT("PLAYERS NewHealth: %f"), NewHealth); // Log the new health value
		}
	}
}
*/

















// Called every frame
void ACilveksBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (auto const widget = Cast<UHealthBarWidget>(WidgetComponent->GetUserWidgetObject()))
	{
		widget->SetBarValuePercent(Health / MaxHealth);

		WidgetComponent->SetVisibility((Health < MaxHealth) ? true : false);
	}
}

// Called to bind functionality to input
void ACilveksBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACilveksBase::GetHealth() const
{
	return Health;
}

float ACilveksBase::GetMaxHealth() const
{
	return MaxHealth;
}

void ACilveksBase::SetHealth(float const NewHealth)
{
	Health = NewHealth;
	if (Cast<ACilveks>(this))
	{
		if (Health <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("You Lose!"));
			//GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
		}
	}
	else if (Cast<ANPC>(this))
	{
		if (Health <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("You Win!"));
			//GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
		}
	}
}

void ACilveksBase::AttackStart() 
{
	UBoxComponent* AxeBladeColBox = FindComponentByClass<UBoxComponent>();

	RightFistCollisionBox->SetCollisionProfileName("Fist");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(true);

	//AxeBladeColBox->SetNotifyRigidBodyCollision(true);
}

void ACilveksBase::AttackEnd() 
{
	UBoxComponent* AxeBladeColBox = FindComponentByClass<UBoxComponent>();

	RightFistCollisionBox->SetCollisionProfileName("Fist");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(false);

	//AxeBladeColBox->SetNotifyRigidBodyCollision(false);
}

