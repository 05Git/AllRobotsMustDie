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

	UFUNCTION(BlueprintCallable, Category = "Pause")
		bool GetPaused();

	UFUNCTION(BlueprintCallable, Category = "Pause")
		void SetPaused(bool Pause);

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealth();

	/** RecieveDamage function from DamageInterface */
	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual void ReceiveDamage(float IncomingDamage) override;

	/** GetHealthRemaining function from DamageInterface */
	virtual float GetHealthRemaining() override;

	/** Exits the game */
	void QuitGame();
	
private:
	float Health;

	float MaxHealth;

	float MoveSpeedUp; /**< MoveSpeedUp determines movement along the X axis */

	float MoveSpeedRight; /**< MoveSpeedRight determines movement along the Y axis */

	float FireOffset; /**< FireOffset determines the distance from the player to spawn a bullet */

	FVector MousePos; /**< MousePos stores the cursor position */

	FVector MouseDir; /**< MouseDir stores the cursor direction */

	bool IsAttacking; /**< IsAttacking determines if the player is currently attacking */

	bool MouseHeld; /**< MouseHeld determines if the player is currently holding down the left mouse button */

	bool Paused;

	FTimerHandle AttackCoolDown; /**< AttackCoolDown determines the time is takes for IsAttacking to reset */

	UPROPERTY(EditAnywhere, Category = "Camera")
		class UCameraComponent *Camera; /**< Camera component for the top down view */
	
	UPROPERTY(EditAnywhere, Category = "Camera")
		class USpringArmComponent *Boom; /**< Spring Arm component for the top down view */

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundBase *FireSound; /**< Sound file to play when a bullet is created */

	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<class ABullet> Projectile;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<class UUserWidget> GameOver;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<class UUserWidget> PauseMenu;

	UPROPERTY()
		class UUserWidget *CurrentWidget;

};
