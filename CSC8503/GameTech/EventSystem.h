#pragma once
#include <wtypes.h>
#include <vector>
#include <list>
#include <String>
#include <map>

enum GAME_EVENT_ID;
struct EVENT;
typedef void(__stdcall* FUNC_EVENT_HANDLE)(const EVENT* pEvent, DWORD64 dwOwnerData);
struct EVENT_DEFINE
{
	typedef std::list< std::pair< FUNC_EVENT_HANDLE, DWORD64 > > REGISTER_STRUCT;
	GAME_EVENT_ID		idEvent;
	LPCTSTR				szEvent;
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
	virtual void	PushEvent(GAME_EVENT_ID id, INT iArg0) = 0;
	virtual void	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0) = 0;
	virtual void	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1) = 0;
	virtual void	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT nArg2) = 0;
	virtual void	PushEvent(GAME_EVENT_ID id, INT iArg0, INT iArg1) = 0;
	virtual void	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT iArg2, INT iArg3) = 0;




	virtual void	RegisterEventHandle(const std::string& nameEvent, FUNC_EVENT_HANDLE funHandle, DWORD64 dwOwnerData = NULL) = 0;

	virtual void	ProcessAllEvent(void) = 0;
};