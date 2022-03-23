#include "AudioManager.h"
#include "fmod.hpp"
#include "..//Common/Assets.h"

#define ERRCHECK(_result) 
FMOD_RESULT m_Result;

AudioManager* AudioManager::m_Instance = nullptr;

void AudioManager::Startup()
{
	m_Instance = new AudioManager();
}

void AudioManager::Cleanup()
{
	delete m_Instance;
}

void AudioManager::Play_Sound(SoundPreset preset)
{
	//bool playing = false;
	//m_Result = m_Channel->isPlaying(&playing);

	//if (!playing)
	//{
	m_Result = m_System->playSound(m_Sounds.at(preset), nullptr, false, &m_Channel);
	ERRCHECK(m_Result);
	//}
}

void AudioManager::Update(float dt)
{
	m_Result = m_System->update();
	ERRCHECK(m_Result);

}

AudioManager::AudioManager()
{
	void* extradriverdata = nullptr;
	m_Result = System_Create(&m_System);
	ERRCHECK(m_Result);
	m_Result = m_System->init(32, FMOD_INIT_NORMAL, extradriverdata);
	ERRCHECK(m_Result);
	m_Channel = nullptr;

	LoadSoundFiles();
}

void AudioManager::LoadSoundFiles()
{
	
}

FMOD::Sound* AudioManager::GetSound(const char* fileName, bool loop, int frequency, int priority)
{
	std::string soundDirectory = NCL::Assets::SOUNDSDIR;
	FMOD::Sound* sound = nullptr;

	m_Result = m_System->createSound(soundDirectory.append(fileName).c_str(), FMOD_2D, nullptr, &sound);
	ERRCHECK(m_Result);

	m_Result = sound->setMode(loop ? FMOD_DEFAULT : FMOD_LOOP_OFF);
	ERRCHECK(m_Result);

	sound->setDefaults(frequency, priority);
	return sound;
}

AudioManager::~AudioManager()
{
	for (auto& sound : m_Sounds)
	{
		m_Result = sound.second->release();
		ERRCHECK(m_Result);
	}	
	m_Result = m_System->close();
	ERRCHECK(m_Result);
	m_Result = m_System->release();
	ERRCHECK(m_Result);
}
