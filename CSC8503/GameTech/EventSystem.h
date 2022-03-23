#pragma once
#include <vector>
#include <list>
#include <String>
#include <map>

enum GAME_EVENT_ID;
struct EVENT;
typedef void(__stdcall* FUNC_EVENT_HANDLE)(const EVENT* pEvent, unsigned long long dwOwnerData);
struct EVENT_DEFINE
{
	typedef std::list< std::pair< FUNC_EVENT_HANDLE, unsigned long long > > REGISTER_STRUCT;
	GAME_EVENT_ID		idEvent;
	const char*				szEvent;
	bool				delayProcess;
	REGISTER_STRUCT		listFuncNotify;
};
struct EVENT {
	EVENT_DEFINE* pEventDef;
	std::vector<std::string>	vArg;
	bool operator == (const EVENT& other)
	{
		if (other.pEventDef != pEventDef) return false;
		if (other.vArg.size() != vArg.size()) return false;
		for (register size_t i = 0; i < vArg.size(); i++)
		{
			if (vArg[i] != other.vArg[i]) return false;
		}
		return true;
	}
};

class EventSystem {
public:
	virtual void	PushEvent(GAME_EVENT_ID id) = 0;
	virtual void	PushEvent(GAME_EVENT_ID id, int iArg0) = 0;
	virtual void	PushEvent(GAME_EVENT_ID id, const char* szArg0) = 0;
	virtual void	PushEvent(GAME_EVENT_ID id, const char* szArg0, const char* szArg1) = 0;
	virtual void	PushEvent(GAME_EVENT_ID id, const char* szArg0, const char* szArg1, int nArg2) = 0;
	virtual void	PushEvent(GAME_EVENT_ID id, int iArg0, int iArg1) = 0;
	virtual void	PushEvent(GAME_EVENT_ID id, const char* szArg0, const char* szArg1, int iArg2, int iArg3) = 0;




	virtual void	RegisterEventHandle(const std::string& nameEvent, FUNC_EVENT_HANDLE funHandle, unsigned long long dwOwnerData = NULL) = 0;

	virtual void	ProcessAllEvent(void) = 0;
};