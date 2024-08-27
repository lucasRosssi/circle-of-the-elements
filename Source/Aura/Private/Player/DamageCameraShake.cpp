// Copyright Lucas Rossi


#include "Player/DamageCameraShake.h"

UDamageCameraShake::UDamageCameraShake()
{
	OscillationDuration = 0.33f;
	OscillationBlendInTime = 0.11f;
	OscillationBlendOutTime = 0.05f;

	LocOscillation.X.Amplitude = 10.f;
	LocOscillation.X.Frequency = 2.f;
	LocOscillation.X.InitialOffset = EOO_OffsetRandom;
	LocOscillation.X.Waveform = EOscillatorWaveform::PerlinNoise;
	
	LocOscillation.Y.Amplitude = 10.f;
	LocOscillation.Y.Frequency = 2.f;
	LocOscillation.Y.InitialOffset = EOO_OffsetRandom;
	LocOscillation.Y.Waveform = EOscillatorWaveform::PerlinNoise;

	LocOscillation.Z.Amplitude = 10.f;
	LocOscillation.Z.Frequency = 2.f;
	LocOscillation.Z.InitialOffset = EOO_OffsetRandom;
	LocOscillation.Z.Waveform = EOscillatorWaveform::PerlinNoise;
	
}
