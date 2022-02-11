// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSProjectile.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"


class UCameraComponent;
class USceneComponent;
class AHUD;

UCLASS()
class RAD_API AMyCharacter : public ACharacter
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Camera, meta = (AllowPrivateAccess="true"))
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleDefaultsOnly,Category=Mesh)
	USkeletalMeshComponent* FPSGun;



public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Weapon)
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FPSMuzzleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MagicUse")
	float Magic = 0.f;
	
	FTimerHandle MagicDelayHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void MoveForward(float Value);
	void OnStartSprint(void);
	void OnStopSprint(void);
	void MoveRight(float Value);
	void BeginCrouch(void);
	void EndCrouch(void); 
	void LookRightLeft(float AxisValue);
	void LookTopDown(float AxisValue);
	void OnFire();
	void ZoomIn();
	void ZoomBack();
	void InteractObject();
	void DelayOnMagicUp();
	void DelayOnMagicDown();
	void GatherMagic();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	float CrouchModifier = 0.8f;
	bool bPressedSprint = false;
	float SprintModifier = 3.0f;
	float SpeedModifier = 200.0f;
	float DefaultMaxWalkingSpeed = 0.1f;
	bool bCanJump = true;
	bool bZoom = false;
	float ViewForZoom = 0.1f;
	float ViewModifier = 0.85f;
	bool bGatherMagic = false;

};
