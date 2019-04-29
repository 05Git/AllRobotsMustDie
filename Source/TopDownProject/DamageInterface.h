#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * DamageInterface
 * Interface to implement damage
 * Inherited by other classes
 */
class TOPDOWNPROJECT_API IDamageInterface
{
	GENERATED_BODY()

public:
	/** Assigns damage to a character */
	virtual void ReceiveDamage(float IncomingDamage) = 0;
	
	/** Gets the health value of a character */
	virtual float GetHealthRemaining() = 0;
};
