#pragma once
#include "EventSystem.h"

enum GAME_EVENT_ID {
	NONE,
	PLAYER_ENTER_WORLD,
	PLAYER_OPEN_FIRE,
	OBJECT_DELETE,
	PLAYER_HIT,
	PLAYER_RESPWAN,
	GAME_OVER,
	PLAYER_COLOR_ZONE,
};
class YiEventSystem:public EventSystem 
{
public:
	YiEventSystem();
	~YiEventSystem();
	void	PushEvent(GAME_EVENT_ID id, const std::vector < std::string > & vParam);

	virtual void	PushEvent(GAME_EVENT_ID id);
	virtual void	PushEvent(GAME_EVENT_ID id, INT iArg0);
	virtual void	PushEvent(GAME_EVENT_ID id, FLOAT fArg0);
	virtual void	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0);
	virtual void	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1);
	virtual void	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT nArg2);
	virtual void	PushEvent(GAME_EVENT_ID id, INT iArg0, INT iArg1);
	virtual void	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT iArg2, INT iArg3);

	virtual void	RegisterEventHandle(const std::string& nameEvent, FUNC_EVENT_HANDLE funHandle, DWORD64 uOwnerData = NULL);

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

