#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageInterface.h"
#include "PlayerChar.generated.h"

/**
* PlayerChar
* The player character
*/
UCLASS(Blueprintable)
class TOPDOWNPROJECT_API APlayerChar : public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

public:
	/** Sets default values for this character's properties */
	APlayerChar();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called when the player attacks */
	void Attack();

	/** Sets the value of MoveSpeedUp */
	void MoveUp(float Value);

	/** Sets the value of MoveSpeedRight */
	void MoveRight(float Value);

	/** Sets IsAttacking to false, called 2.5 seconds after the player attacks */
	void CanAttack();

	/** Sets MouseHeld to true */
	void MousePressed();

	/** Sets MouseHeld to false	*/
	void MouseReleased();

	/** Returns Paused */
	UFUNCTION(BlueprintCallable, Category = "Pause")
		bool GetPaused();

	/** Sets Paused */
	UFUNCTION(BlueprintCallable, Category = "Pause")
		void SetPaused(bool Pause);

	/** Returns Health */
	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealth();

	/** Returns AnimSpeed */
	UFUNCTION(BlueprintCallable, Category = "Animation")
		float GetAnimX();

	/** Returns AnimDirection */
	UFUNCTION(BlueprintCallable, Category = "Animation")
		float GetAnimY();

	/** RecieveDamage function from DamageInterface */
	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void ReceiveDamage(float IncomingDamage) override;

	/** GetHealthRemaining function from DamageInterface */
	virtual float GetHealthRemaining() override;

	/** Exits the game */
	void PauseGame();
	
private:
	float Health; /**< Health determines how much health the player currently has */

	float MaxHealth; /**< MaxHealth determines the maximum amount of health a player is allowed to have */

	float MoveSpeedUp; /**< MoveSpeedUp determines movement along the X axis */

	float MoveSpeedRight; /**< MoveSpeedRight determines movement along the Y axis */

	float FireOffset; /**< FireOffset determines the distance from the player to spawn a bullet */

	float AnimX; /**< AnimX determines the value for Speed in the player's animation blueprint */

	float AnimY; /**< AnimY determines the value for Direction in the player's animation blueprint */

	FVector MousePos; /**< MousePos stores the cursor position */

	FVector MouseDir; /**< MouseDir stores the cursor direction */

	bool IsAttacking; /**< IsAttacking determines if the player is currently attacking */

	bool MouseHeld; /**< MouseHeld determines if the player is currently holding down the left mouse button */

	bool Paused; /**< Paused determines if the game is paused or not */

	FTimerHandle AttackCoolDown; /**< AttackCoolDown determines the time is takes for IsAttacking to reset */

	UPROPERTY(EditAnywhere, Category = "Camera")
		class UCameraComponent *Camera; /**< Camera component for the top down view */
	
	UPROPERTY(EditAnywhere, Category = "Camera")
		class USpringArmComponent *Boom; /**< Spring Arm component for the top down view */

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase *FireSound; /**< Sound file to play when a bullet is created */

	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<class ABullet> Projectile; /**< Type of Bullet the player is currently able to use */

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<class UUserWidget> GameOver; /**< Widget to use when the player dies */

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<class UUserWidget> PauseMenu; /**< Widget to use when the player pauses the game */

	UPROPERTY()
		class UUserWidget *CurrentWidget; /**< Current widget to draw on the HUD */
};
