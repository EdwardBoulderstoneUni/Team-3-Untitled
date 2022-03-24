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

void AudioManager::Play_Sound(SoundPreset preset, bool stopCurrent)
{
	if (stopCurrent)
	{
		for (auto& sound : m_Sounds)
		{
			m_Result = sound.second->channel->setPaused(true);
		}		
		m_Result = m_System->update();
		ERRCHECK(m_Result);
	}
	m_Result = m_System->playSound(m_Sounds.at(preset)->sound, nullptr, false, &m_Sounds.at(preset)->channel);
	ERRCHECK(m_Result);
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
	RegisterHandlers();
}

void AudioManager::LoadSoundFiles()
{
	m_Sounds.insert({ SoundPreset_InGame, new SoundTemplate("fightmusic.mp3", true, m_System) });
	m_Sounds.insert({ SoundPreset_MainMenu, new SoundTemplate("Beach_song.mp3", true, m_System) });
	m_Sounds.insert({ SoundPreset_Fire,new SoundTemplate("Bam sound effect.mp3", false, m_System) });
	m_Sounds.insert({ SoundPreset_Collision,new SoundTemplate("boink.mp3", false, m_System) });
}

void AudioManager::SoundTemplate::LoadSound(const char* fileName, bool loop, FMOD::System *system)
{
	std::string soundDirectory = NCL::Assets::SOUNDSDIR;
	FMOD::Sound* sound = nullptr;

	m_Result = system->createSound(soundDirectory.append(fileName).c_str(), FMOD_2D, nullptr, &sound);
	ERRCHECK(m_Result);

	m_Result = sound->setMode(loop ? FMOD_DEFAULT : FMOD_LOOP_OFF);
	ERRCHECK(m_Result);

	this->sound = sound;
	this->channel = nullptr;
}

AudioManager::~AudioManager()
{
	for (auto& sound : m_Sounds)
	{
		m_Result = sound.second->sound->release();
		ERRCHECK(m_Result);

		delete sound.second;
	}	
	m_Result = m_System->close();
	ERRCHECK(m_Result);
	m_Result = m_System->release();
	ERRCHECK(m_Result);
}
void AudioManager::_openFirHandle(const EVENT* pEvent, unsigned long long dwOwnerData)
{
	AudioManager::GetInstance().Play_Sound(AudioManager::SoundPreset::SoundPreset_Collision, false);
}
void AudioManager::RegisterHandlers() {
	YiEventSystem::GetMe()->RegisterEventHandle("OPEN_FIRE",_openFirHandle);
}