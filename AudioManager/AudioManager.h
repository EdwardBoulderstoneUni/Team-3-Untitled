#pragma once
#include <map>
#include "../CSC8503/GameTech/YiEventSystem.h"
namespace FMOD
{
	class System;
	class Channel;
	class Sound;
}

class AudioManager
{
public:
	enum SoundPreset;
	struct SoundTemplate
	{
		FMOD::Sound* sound;
		FMOD::Channel* channel;	
		SoundTemplate(const char* name, bool aLoop, FMOD::System* system) { LoadSound(name, aLoop, system); }
		void LoadSound(const char* name, bool loop, FMOD::System *system);
	};
private:
	static AudioManager* m_Instance;
	AudioManager();
	void LoadSoundFiles();

	FMOD::System* m_System;
	FMOD::Channel* m_Channel;
	std::map<SoundPreset, SoundTemplate*>m_Sounds;

public:
	enum SoundPreset
	{
		SoundPreset_MainMenu,
		SoundPreset_InGame,
		SoundPreset_Collision,
		SoundPreset_MAX
	};
	~AudioManager();
	static AudioManager& GetInstance() { return *m_Instance; }
	static void Startup();
	static void Cleanup();
	
	void Play_Sound(SoundPreset preset, bool stopCurrent = true);
	void Update(float dt);
private:
	void RegisterHandlers();
	static void _openFirHandle(const EVENT* pEvent, unsigned long long dwOwnerData);
};
