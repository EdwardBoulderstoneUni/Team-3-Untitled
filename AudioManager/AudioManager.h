#pragma once
#include <map>
namespace FMOD
{
	class System;
	class Channel;
	class Sound;
}

class AudioManager
{
	static AudioManager* m_Instance;
	AudioManager();
	void LoadSoundFiles();
	FMOD::Sound* GetSound(const char* fileName, bool loop = false, int frequency = 12000, int priority = 128);
	enum SoundPreset;
	FMOD::System* m_System;
	FMOD::Channel* m_Channel;

	std::map<SoundPreset, FMOD::Sound*>m_Sounds;
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
	
	void Play_Sound(SoundPreset preset);
	void Update(float dt);
};
