#include "TDPGameInstance.h"

UTDPGameInstance::UTDPGameInstance()
{
	Volume = 0.5f; // Sets initial value of Volume
	Score = 0.0f; // Sets initial value of Score
}

float UTDPGameInstance::GetVolume()
{
	return Volume; // Returns Volume
}

void UTDPGameInstance::SetVolume(float Value)
{
	this->Volume = Value; // Sets Volume to Value
}

float UTDPGameInstance::GetScore()
{
	return Score; // Returns Score
}

void UTDPGameInstance::AddScore(float Value)
{
	this->Score = this->Score + Value; // Adds Value to Score
}

void UTDPGameInstance::MinusScore(float Value)
{
	if (Score - Value >= 0)
	{
		this->Score = Score - Value; // Takes Value away from score
	}
	else
	{
		this->Score = 0; // Sets Score to 0 if Value is too large
	}
}