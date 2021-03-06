#pragma once
#include "Component/AudioComponent.h"
#include "Audio/Audio.h"
#include "Resource/ResourceRef.h"

namespace Auto3D 
{

class OSound;
class FAudioBuffer;
/// Responsible for the operation of 3d sound source
class AUTO_API AAudioSourceComponent : public AAudioComponent
{
	DECLARE_A_CLASS(AAudioSourceComponent, AAudioComponent)
public:
	/// The constructor
	AAudioSourceComponent();
	/// The destructor
	~AAudioSourceComponent();

	/// Plays the active audioclip at (future) scheduled time. If time < 0 it specifies a delay
	void Play(int delayTime = 0);
	/// Pauses the active audioclip
	void Pause(int delayTime = 0);
	/// Stops the active audio clip
	void Stop(int delayTime = 0);
	/// Rewind audio clip
	void Rewind(int delayTime = 0);
	/// Attach buffer for point
	void SetSound(OSound* sound);
	/// Get audio buffer
	FAudioBuffer* GetBuffer() { return  _buffer; }
	/// Get source state
	EAudioSourceState::Type GetState();

public:
	void SetSoundAttr(FResourceRef sound);
	FResourceRef GetSoundAttr() const;

	void SetPitch(float pitch) { _pitch = pitch; }
	float GetPitch() { return _pitch; }

	void SetGain(float gain) { _gain = gain; }
	float GetGain() { return _gain; }

	void SetVel(const TVector3F& vel) { _vel = vel; }
	const TVector3F& GetVel() { return _vel; }
private:
	/// AAudio sound resource
	OSound* _sound;
	/// AAudio buffer
	FAudioBuffer* _buffer;
	/// Pitch
	float _pitch;
	/// Gain
	float _gain;
	/// Vel
	TVector3F _vel;
};

}