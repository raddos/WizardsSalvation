// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	this->GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// setting camera component 
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());
	FPSCameraComponent->SetRelativeLocation( FVector(20.f, 0, 80.f));
	FPSCameraComponent->bUsePawnControlRotation = true;
	//setting for view

	ViewForZoom = FPSCameraComponent->FieldOfView;

	// setting gun component
	FPSGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSGun"));
	// seeable thro other peaople ( multiplayer only )
	FPSGun->SetOnlyOwnerSee(false);
	FPSGun->CastShadow = false;
	FPSGun->SetupAttachment(RootComponent);
	
	// cev oruzija
	FPSMuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponMuzzle"));
	FPSMuzzleLocation->SetupAttachment(FPSGun);

	// Mesh to not be seen
	this->GetMesh()->bOnlyOwnerSee = false;

	this->Magic = 1.f;




}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	FPSGun->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,true),TEXT("Weapon"));
	//set movement

	DefaultMaxWalkingSpeed =this-> GetCharacterMovement()->MaxWalkSpeed;
	FPSMuzzleLocation->SetRelativeLocation(FVector(30.f,0.f,10.f));
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Check if set up is correct 
	check(PlayerInputComponent);


	// Jump option
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveSideways", this, &AMyCharacter::MoveRight);
	//MouseInput
	PlayerInputComponent->BindAxis("Yaw", this, &AMyCharacter::LookRightLeft);
	PlayerInputComponent->BindAxis("Pitch", this, &AMyCharacter::LookTopDown);
	//Crouching
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMyCharacter::EndCrouch);

	//Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::OnFire);
	//Gather Magic 
	PlayerInputComponent->BindAction("GatherMagic", IE_Pressed, this, &AMyCharacter::GatherMagic);
	//Move faster
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::OnStartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::OnStopSprint);

	//Zoom in
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AMyCharacter::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AMyCharacter::ZoomBack);

	//Interact with Object
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMyCharacter::InteractObject);
}

void AMyCharacter::MoveForward(float Value) {

	if (Value != 0) {
		AddMovementInput(GetActorForwardVector(), Value);
	};
}


void AMyCharacter::MoveRight(float Value) {
	if (Value != 0) {
		AddMovementInput(GetActorRightVector(), Value);
	};
}


void AMyCharacter::LookRightLeft(float AxisValue) {
	
	this->AddControllerYawInput(AxisValue);

}

void AMyCharacter::LookTopDown(float AxisValue) {
	this->AddControllerPitchInput(AxisValue);

}

void AMyCharacter::BeginCrouch(void) {
	this->Crouch();
	this->GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkingSpeed / CrouchModifier;
}

void AMyCharacter::EndCrouch(void) {
	this->UnCrouch();
	this->GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkingSpeed;
}

void AMyCharacter::OnFire() {

	DelayOnMagicDown();
	if (ProjectileClass != nullptr && bZoom) {
		UWorld* const world = GetWorld();
		if (world != nullptr) {
			
			const FRotator SpawnRotation = GetControlRotation();

			FVector SpawnLocation = this-> GetActorLocation();
			if (FPSMuzzleLocation != nullptr && Magic>0) {
				
				SpawnLocation = FPSMuzzleLocation->GetComponentLocation();

				FActorSpawnParameters SpawnParameter;
				SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				//if (GEngine)
				//	GEngine->AddOnScreenDebugMessage(-10, 30.0f, FColor::Blue, TEXT("Puca!"));

				world->SpawnActor<AFPSProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameter);
			}
		}
	}
	
}

void AMyCharacter::GatherMagic() {
	bGatherMagic = true;
	GetWorld()->GetTimerManager().SetTimer(MagicDelayHandle, this, &AMyCharacter::DelayOnMagicUp, 1, false);

}

void AMyCharacter::OnStartSprint() {
	bPressedSprint = true;
	this->GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkingSpeed * SprintModifier + SpeedModifier;
}

void AMyCharacter::OnStopSprint() {
	bPressedSprint = false;
	this->GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkingSpeed + SpeedModifier;
}

void AMyCharacter::ZoomIn() {
	bZoom = true;
	FPSCameraComponent->SetFieldOfView(ViewForZoom * ViewModifier);
	
}

void AMyCharacter::ZoomBack() {
	bZoom = false;
	FPSCameraComponent->SetFieldOfView(ViewForZoom);
}

void AMyCharacter::InteractObject() {

	
}

void AMyCharacter::DelayOnMagicUp() {
	if (Magic <= 1.2 && Magic>=-25.f) {
		Magic += 0.20f;
	}
}

void AMyCharacter::DelayOnMagicDown() {
	if (Magic <= 1.2 && Magic >= -25.f) {
		Magic -= 0.25f;
	}
}
