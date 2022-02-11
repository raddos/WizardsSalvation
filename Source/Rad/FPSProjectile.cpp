// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"


// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setting collison component for projectile collieder
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollion"));
	CollisionComponent->InitSphereRadius(5.f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);

	//Set player collider options
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	
	//Set collider as root component
	RootComponent = CollisionComponent;
	 
	//set Projectile movement settings
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->bIsHomingProjectile = false;
	
	
	// kill bullte after time
	InitialLifeSpan = 5.f;

	//Setting ApplyDamage component
 //	DamageTypeClass.GetDefaultObject();
	// ovaj deo nije tacan jel istigator je puska a na projectile je causer
//	EventInstigator->GetPawn();
	//damage causer
//	DamageCauser->GetWorld();

}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector ImpactNormal, const FHitResult& Hit) {
	

	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr) 
	{
		/*if (OtherActor->ActorHasTag("Enemy") && GEngine) {

			GEngine->AddOnScreenDebugMessage(-10, 30.0f, FColor::Blue, TEXT("Nasao je tag"));
			UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, EventInstigator, DamageCauser, DamageTypeClass);

		}
		else*/ 
		if (OtherComponent->IsSimulatingPhysics())
		{
			//Pogledat dal je lokacija na aktori ili sebi (GetActorLocation());// good mehanick malo te baci u nazad

			OtherComponent->AddImpulseAtLocation(GetVelocity()*ImpactModifier,this->GetActorLocation());
			Destroy();
		}
	}

}

