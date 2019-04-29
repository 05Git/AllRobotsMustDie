#include "PlayerChar.h"
#include "Bullet.h"
#include "TDPGameInstance.h"
#include "Components/InputComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/RotationMatrix.h"
#include "Sound/SoundBase.h"
#include "Engine.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

APlayerChar::APlayerChar()
{
 	/** Set to call Tick every frame */
	PrimaryActorTick.bCanEverTick = true;

	/** Sets initial values for Health, MoveSpeedUp, MoveSpeedRight, IsAttacking and MouseHeld */
	Health = 100.0f;
	MaxHealth = 100.0f;
	MoveSpeedUp = 0.0f;
	MoveSpeedRight = 0.0f;
	FireOffset = 80.0f;
	IsAttacking = false;
	MouseHeld = false;
	Paused = false;
	
	/** Creates Camera and Spring Arm components */
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Boom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	Boom->SetupAttachment(RootComponent);
	Camera->SetupAttachment(Boom, USpringArmComponent::SocketName);
}

void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** Moves player along X and Y axis depending on input */
	FVector Move = FVector(MoveSpeedUp * DeltaTime, MoveSpeedRight * DeltaTime, 0.0f);
	AddActorWorldOffset(Move, true);

	/** Rotates player to face the mouse cursor */
	APlayerController *Controller = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	Controller->DeprojectMousePositionToWorld(MousePos, MouseDir);
	FRotator PlayerRot = this->GetActorRotation();
	FRotator MouseRot = MouseDir.Rotation();
	FRotator Rotation = FRotator(PlayerRot.Pitch, MouseRot.Yaw, PlayerRot.Roll);
	AddActorLocalRotation(Rotation, true);

	/** If MouseHeld is true, then Attack is called */
	if (MouseHeld)
	{
		Attack();
	}

	if (Paused == false)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller);
	}

	if (GetHealthRemaining() <= 0)
	{
		Controller->SetPause(true);
		Paused = true;
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), GameOver);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** Sets input components for attacking and movement */
	InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerChar::MousePressed);
	InputComponent->BindAction("Attack", IE_Released, this, &APlayerChar::MouseReleased);
	InputComponent->BindAction("Quit", IE_Pressed, this, &APlayerChar::QuitGame);
	InputComponent->BindAxis("MoveUp", this, &APlayerChar::MoveUp);
	InputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
}

void APlayerChar::Attack()
{
	/** Checks if IsAttacking is false */
	if (!IsAttacking)
	{
		/** Gets location to spawn bullet */
		FRotator SpawnRot = this->GetActorRotation();
		FVector SpawnLoc = this->GetActorLocation() + (this->GetActorForwardVector() * FireOffset);
		UWorld *const World = GetWorld();
		/** Checks if World is not null */
		if (World != NULL)
		{
			/** Spawns a bullet */
			if (Projectile)
			{
				World->SpawnActor<ABullet>(Projectile, SpawnLoc, SpawnRot);
			}
			if (FireSound != nullptr)
			{
				UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, this->GetActorLocation(), Instance->GetVolume());
			}
		}
		/** Gets world timer manager and sets a timer to call CanAttack after 0.5 seconds, then sets IsAttacking to false */
		World->GetTimerManager().SetTimer(AttackCoolDown, this, &APlayerChar::CanAttack, 0.5f);
		IsAttacking = true;
	}
}

void APlayerChar::MoveUp(float Value)
{
	/** Checks if Value is not 0*/
	if (Value != 0.0f)
	{
		/** Sets MoveSpeedUp */
		MoveSpeedUp = Value * 800.0f;
	}
	else
	{
		/** Sets MoveSpeedUp */
		MoveSpeedUp = 0.0f;
	}
}

void APlayerChar::MoveRight(float Value)
{
	/** Checks if Value is not 0*/
	if (Value != 0.0f)
	{
		/** Sets MoveSpeedRight */
		MoveSpeedRight = Value * 800.0f;
	}
	else
	{
		/** Sets MoveSpeedRight */
		MoveSpeedRight = 0.0f;
	}
}

void APlayerChar::CanAttack()
{
	/** Sets IsAttacking to false */
	IsAttacking = false;
}

void APlayerChar::MousePressed()
{
	/** Sets MouseHeld to true */
	MouseHeld = true;
}

void APlayerChar::MouseReleased()
{
	/** Sets MouseHeld to false */
	MouseHeld = false;
}

bool APlayerChar::GetPaused()
{
	return Paused;
}

void APlayerChar::SetPaused(bool Pause)
{
	this->Paused = Pause;
}

float APlayerChar::GetHealth()
{
	return Health / MaxHealth;
}

void APlayerChar::ReceiveDamage(float IncomingDamage)
{
	if (IncomingDamage >= 0)
	{
		Health -= IncomingDamage;
	}
}

float APlayerChar::GetHealthRemaining()
{
	return Health;
}

void APlayerChar::QuitGame()
{
	if (Paused == false)
	{
		APlayerController *Controller = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		Controller->SetPause(true);
		Paused = true;
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenu);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			UWidgetBlueprintLibrary::SetInputMode_UIOnly(Controller, CurrentWidget, false);
		}
	}
}
