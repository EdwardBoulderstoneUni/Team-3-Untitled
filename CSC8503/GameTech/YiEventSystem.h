#pragma once
#include "EventSystem.h"

enum GAME_EVENT_ID {
	PLAYER_OPEN_FIRE,
	PLAYER_THROW_GRENADE,
	OBJECT_DELETE,
	PLAYER_HIT,
	PLAYER_RESPWAN,
	GAME_OVER,
	PLAYER_COLOR_ZONE,
	GRENADE_DAMAGE_RANGE
};
class YiEventSystem:public EventSystem 
{
public:
	YiEventSystem();
	~YiEventSystem();
	void	PushEvent(GAME_EVENT_ID id, const std::vector < std::string > & vParam);

	virtual void	PushEvent(GAME_EVENT_ID id);
	virtual void	PushEvent(GAME_EVENT_ID id, int iArg0);
	virtual void	PushEvent(GAME_EVENT_ID id, float fArg0);
	virtual void	PushEvent(GAME_EVENT_ID id, const char* szArg0);
	virtual void	PushEvent(GAME_EVENT_ID id, const char* szArg0, const char* szArg1);
	virtual void	PushEvent(GAME_EVENT_ID id, const char* szArg0, const char* szArg1, int nArg2);
	virtual void	PushEvent(GAME_EVENT_ID id, int iArg0, int iArg1);
	virtual void	PushEvent(GAME_EVENT_ID id, const char* szArg0, const char* szArg1, int iArg2, int iArg3);

	virtual void	RegisterEventHandle(const std::string& nameEvent, FUNC_EVENT_HANDLE funHandle, unsigned long long uOwnerData = NULL);

	virtual void	ProcessAllEvent();

	static YiEventSystem* GetMe() { return p_self; }

private:
	virtual void	Initial();
	void	_PushEvent(const EVENT& event);
	void	_ProcessEvent(const EVENT& event);
	std::map< std::string, EVENT_DEFINE* >					m_mapEventIndex_AsName;
	std::map< GAME_EVENT_ID, EVENT_DEFINE* >			m_mapEventIndex_AsID;
	std::list< EVENT >		m_queueEvent;
	std::list< EVENT >		m_delayQueueEvent;
	static YiEventSystem* p_self;
};

