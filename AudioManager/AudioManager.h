#pragma once
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


	FMOD::System* m_System;
	FMOD::Channel* m_Channel;
	FMOD::Sound* m_Sound;

public:
	~AudioManager();
	static AudioManager& GetInstance() { return *m_Instance; }
	static void Startup();
	static void Cleanup();
	//test function, replace with proper implementation in future
	void Play_Sound();
	void Update(float dt);
};
