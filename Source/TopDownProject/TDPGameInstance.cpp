#include "TDPGameInstance.h"

UTDPGameInstance::UTDPGameInstance()
{
	Volume = 0.5f;
}

float UTDPGameInstance::GetVolume()
{
	return Volume;
}

void UTDPGameInstance::SetVolume(float Volume)
{
	this->Volume = Volume;
}
