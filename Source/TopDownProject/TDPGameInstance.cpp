#include "TDPGameInstance.h"

UTDPGameInstance::UTDPGameInstance()
{
	Volume = 0.5f; // Sets initial value of Volume
}

float UTDPGameInstance::GetVolume()
{
	return Volume; // Returns Volume
}

void UTDPGameInstance::SetVolume(float Volume)
{
	this->Volume = Volume; // Sets Volume to Volume
}
