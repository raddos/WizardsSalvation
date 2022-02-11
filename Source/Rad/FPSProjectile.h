// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS(config = Game)
class RAD_API AFPSProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly,Category=Projectile)
	USphereComponent* CollisionComponent;

	UProjectileMovementComponent* ProjectileMovement;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

public:	

	// Sets default values for this actor's properties
	AFPSProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComponent,FVector ImpactNormal,const FHitResult& Hit);

//	AController* EventInstigator;
//	AActor* DamageCauser;
//	const TSubclassOf< class UDamageType > DamageTypeClass;

	USphereComponent* GetCollisionComponent() const { return CollisionComponent; }

	UProjectileMovementComponent* GetProjectileMovementComponent()const { return ProjectileMovement; }

private:
	float ImpactModifier = 100.f;
	float BaseDamage = 5.f;
};

