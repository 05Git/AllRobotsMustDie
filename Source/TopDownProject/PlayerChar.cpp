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
 	
	PrimaryActorTick.bCanEverTick = true; // Set to call Tick every frame

	Health = 100.0f; // Sets initial value of Health
	MaxHealth = 100.0f; // Sets initial value of MaxHealth
	MoveSpeedUp = 0.0f; // Sets initial value of MoveSpeedUp
	MoveSpeedRight = 0.0f; // Sets initial value of MoveSpeedRight
	FireOffset = 80.0f; // Sets initial value of FireOffset
	AnimX = 0.0f; // Sets initial value of AnimX
	AnimY = 0.0f; // Sets initial value of AnimY
	IsAttacking = false; // Sets initial value of IsAttacking
	MouseHeld = false; // Sets initial value of MouseHeld
	Paused = false; // Sets initial value of Paused
	
	// Creates Camera and Spring Arm components
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

	// Moves player along X and Y axis depending on input
	FVector Move = FVector(MoveSpeedUp * DeltaTime, MoveSpeedRight * DeltaTime, 0.0f);
	AddActorWorldOffset(Move, true);

	// Rotates player to face the mouse cursor
	APlayerController *Controller = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	Controller->DeprojectMousePositionToWorld(MousePos, MouseDir);
	FRotator PlayerRot = this->GetActorRotation();
	FRotator MouseRot = MouseDir.Rotation();
	FRotator Rotation = FRotator(PlayerRot.Pitch, MouseRot.Yaw, PlayerRot.Roll);
	AddActorLocalRotation(Rotation, true);

	// If MouseHeld is true, then Attack is called
	if (MouseHeld)
	{
		Attack();
	}

	// If Paused is false, then InputMode is set to GameOnly
	if (!Paused)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller);
	}

	// Checks if Health is equal to or less than 0
	if (GetHealthRemaining() <= 0)
	{
		Controller->SetPause(true); // Pauses the game
		Paused = true; // Sets Paused to true
		UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		Instance->MinusScore(Instance->GetScore()); // Sets Score to 0
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), GameOver);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport(); // Adds gameOver widget to viewport
		}
	}
}

void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Sets input components for attacking, moveing and pausing the game
	InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerChar::MousePressed);
	InputComponent->BindAction("Attack", IE_Released, this, &APlayerChar::MouseReleased);
	InputComponent->BindAction("Pause", IE_Pressed, this, &APlayerChar::PauseGame);
	InputComponent->BindAxis("MoveUp", this, &APlayerChar::MoveUp);
	InputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
}

void APlayerChar::Attack()
{
	// Checks if IsAttacking is false
	if (!IsAttacking)
	{
		// Gets location to spawn bullet
		FRotator SpawnRot = this->GetActorRotation();
		FVector SpawnLoc = this->GetActorLocation() + (this->GetActorForwardVector() * FireOffset);
		UWorld *const World = GetWorld();
		// Checks if World is not null
		if (World != NULL)
		{
			// Spawns a bullet
			if (Projectile)
			{
				World->SpawnActor<ABullet>(Projectile, SpawnLoc, SpawnRot);
			}
			// Plays FireSound
			if (FireSound != nullptr)
			{
				UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, this->GetActorLocation(), Instance->GetVolume());
			}
		}
		// Gets world timer manager and sets a timer to call CanAttack after 0.5 seconds, then sets IsAttacking to false
		World->GetTimerManager().SetTimer(AttackCoolDown, this, &APlayerChar::CanAttack, 0.5f);
		IsAttacking = true;
	}
}

void APlayerChar::MoveUp(float Value)
{
	// Checks if Value is not 0
	if (Value != 0.0f)
	{
		MoveSpeedUp = Value * 800.0f; // Sets MoveSpeedUp to Value * 800.0f
		if (Value > 0.0f)
		{
			AnimY = 100.0f; // Sets AnimY to 100
		}
		else
		{
			AnimY = -100.0f; // Sets AnimY to -100
		}
	}
	else
	{
		MoveSpeedUp = 0.0f; // Sets MoveSpeedUp to 0.0f
		AnimY = 0.0f; // Sets AnimY to 0
	}
}

void APlayerChar::MoveRight(float Value)
{
	// Checks if Value is not 0
	if (Value != 0.0f)
	{
		MoveSpeedRight = Value * 800.0f; // Sets MoveSpeedRight to Value * 800.0f
		if (Value > 0.0f)
		{
			AnimX = 100.0f; // Sets AnimX to 100
		}
		else
		{
			AnimX = -100.0f; // Sets AnimX to -100
		}
	}
	else
	{
		MoveSpeedRight = 0.0f; // Sets MoveSpeedRight to 0.0f
		AnimX = 0.0f; // Sets AnimX to 0
	}
}

void APlayerChar::CanAttack()
{
	IsAttacking = false; // Sets IsAttacking to false
}

void APlayerChar::MousePressed()
{
	MouseHeld = true; // Sets MouseHeld to true
}

void APlayerChar::MouseReleased()
{
	MouseHeld = false; // Sets MouseHeld to false
}

bool APlayerChar::GetPaused()
{
	return Paused; // Returns Paused
}

void APlayerChar::SetPaused(bool Pause)
{
	this->Paused = Pause; // Sets Paused to Pause
}

float APlayerChar::GetHealth()
{
	return Health / MaxHealth; // Returns Health / MaxHealth
}

float APlayerChar::GetAnimX()
{
	return AnimX;
}

float APlayerChar::GetAnimY()
{
	return AnimY;
}

void APlayerChar::ReceiveDamage(float IncomingDamage)
{
	// Checks if IncomingDamage is greater than or equal to 0
	if (IncomingDamage >= 0)
	{
		Health -= IncomingDamage; // Reduces Health by IncomingDamage
		UTDPGameInstance *Instance = Cast<UTDPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		Instance->MinusScore(20.0f); // Reduces Score by 20
	}
}

float APlayerChar::GetHealthRemaining()
{
	return Health; // Returns Health
}

void APlayerChar::PauseGame()
{
	// Checks if Paused is false
	if (!Paused)
	{
		// Calls player controller and pauses game, sets Paused to true and creates the PauseMenu widget
		APlayerController *Controller = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		Controller->SetPause(true);
		Paused = true;
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenu);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, CurrentWidget, EMouseLockMode::DoNotLock);
		}
	}
}
