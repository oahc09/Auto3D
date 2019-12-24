#include "AudioSource.h"
#include "Engine/Components/Sound.h"
#include "Audio.h"
#include "Debug/Log.h"
#include "AudioBuffer.h"
#include "IO/ResourceRef.h"
#include "Resource/ResourceCache.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "Core/Modules/ModuleManager.h"
#include "Debug/DebugNew.h"


namespace Auto3D 
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AAudioSource)
	.constructor<>()
		.property("sound", &AAudioSource::GetSoundAttr, &AAudioSource::SetSoundAttr)
		(
			metadata(SERIALIZABLE, "")
		)
		.property("pitch", &AAudioSource::GetPitch, &AAudioSource::SetPitch)
		(
			metadata(SERIALIZABLE, "")
		)
		.property("gain", &AAudioSource::GetGain, &AAudioSource::SetGain)
		(
			metadata(SERIALIZABLE, "")
		)
		.property("vel", &AAudioSource::GetVel, &AAudioSource::SetVel)
		(
			metadata(SERIALIZABLE, "")
		)
	;
}

AAudioSource::AAudioSource() :
	_pitch(1.0f),
	_gain(1.0f),
	_vel(0.0f, 0.0f, 0.1f)
{
	_audio = GModuleManager::Get().AudioModule();
}

AAudioSource::~AAudioSource()
{
}

void AAudioSource::RegisterObject()
{
	//RegisterFactory<AAudioSource>();
}

void AAudioSource::Play(int delayTime)
{
	if (!_audio)
		WarningString("Miss audio, can'y play this source.");
	_audio->SourcePlay(_buffer->Source(), delayTime);
}

void AAudioSource::Pause(int delayTime)
{
	if (!_audio)
		WarningString("Miss audio, can'y pause this source.");
	_audio->SourcePause(_buffer->Source(), delayTime);
}

void AAudioSource::Stop(int delayTime)
{
	if (!_audio)
		WarningString("Miss audio, can'y stop this source.");
	_audio->SourceStop(_buffer->Source(), delayTime);
}

void AAudioSource::Rewind(int delayTime)
{
	if (!_audio)
		WarningString("Miss audio, can'y rewind this source.");
	_audio->SourceRewind(_buffer->Source(), delayTime);
}

EAudioSourceState::Type AAudioSource::GetState()
{
	return _audio->GetState(GetBuffer()->Source());
}


void AAudioSource::SetSound(ASound* sound)
{
	_sound = sound;
	_buffer = MakeShared<FAudioBuffer>();
	_buffer->Create(sound);
	_audio->AddSource(GetBuffer()->Source(), this);

	_audio->SetPitch(GetBuffer()->Source(), _pitch);
	_audio->SetGain(GetBuffer()->Source(), _gain);
	_audio->SetVel(GetBuffer()->Source(), _vel);
}

void AAudioSource::SetSoundAttr(FResourceRef sound)
{
	FResourceModule* cache = GModuleManager::Get().CacheModule();
	SetSound(cache->LoadResource<ASound>(sound._name));
}

FResourceRef AAudioSource::GetSoundAttr() const
{
	return FResourceRef(AAudioSource::GetTypeHashStatic(), ResourceName(_sound.Get()));
}
}
