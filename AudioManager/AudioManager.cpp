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

void AudioManager::Play_Sound()
{
	bool playing = false;
	m_Result = m_Channel->isPlaying(&playing);

	if (!playing)
	{
		m_Result = m_System->playSound(m_Sound, nullptr, false, &m_Channel);
		ERRCHECK(m_Result);
	}
}

void AudioManager::Update(float dt)
{
	m_Result = m_System->update();
	ERRCHECK(m_Result);

	unsigned int ms = 0;
	unsigned int lenms = 0;
	bool playing = false;
	bool paused = false;
	int channelsplaying = 0;

	/*	if (m_Channel)
		{
			FMOD::Sound* currentsound = 0;

			m_Result = m_Channel->isPlaying(&playing);

			m_Result = m_Channel->getPaused(&paused);

			m_Result = m_Channel->getPosition(&ms, FMOD_TIMEUNIT_MS);

			m_Channel->getCurrentSound(&currentsound);
			if (currentsound)
			{
				m_Result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);

			}
		}*/
}

AudioManager::AudioManager()
{
	void* extradriverdata = nullptr;
	m_Result = System_Create(&m_System);
	ERRCHECK(m_Result);
	m_Result = m_System->init(32, FMOD_INIT_NORMAL, extradriverdata);
	ERRCHECK(m_Result);
	m_Channel = nullptr;
	m_Sound = nullptr;


	//temp
	std::string testSoundFilePath = NCL::Assets::SOUNDSDIR;
	testSoundFilePath.append("drumloop.wav");
	m_Result = m_System->createSound(testSoundFilePath.c_str(),
		FMOD_2D, nullptr, &m_Sound);
	ERRCHECK(m_Result);

	m_Result = m_Sound->setMode(FMOD_LOOP_OFF);
	/* drumloop.wav has embedded loop points which automatically makes looping turn on, */
	ERRCHECK(m_Result);
	/* so turn it off here.  We could have also just put FMOD_LOOP_OFF in the above CreateSound call. */

	m_Sound->setDefaults(12000, 128);
}

AudioManager::~AudioManager()
{
	m_Result = m_Sound->release();
	ERRCHECK(m_Result);
	m_Result = m_System->close();
	ERRCHECK(m_Result);
	m_Result = m_System->release();
	ERRCHECK(m_Result);
}
