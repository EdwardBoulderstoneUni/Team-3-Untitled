#include "YiEventSystem.h"
#include "../../Common/GameTimer.h"
YiEventSystem* YiEventSystem::p_self = NULL;
EVENT_DEFINE g_GlobalEvent[] = {
	{PLAYER_OPEN_FIRE,"OPEN_FIRE"},
};
YiEventSystem::YiEventSystem()
{
	p_self = this;
	Initial();
}

YiEventSystem::~YiEventSystem()
{
}

void YiEventSystem::Initial()
{
	INT nEventNum = sizeof(g_GlobalEvent) / sizeof(EVENT_DEFINE);

	for (INT i = 0; i < nEventNum; i++)
	{
		m_mapEventIndex_AsName[g_GlobalEvent[i].szEvent] = &(g_GlobalEvent[i]);

		m_mapEventIndex_AsID[g_GlobalEvent[i].idEvent] = &(g_GlobalEvent[i]);
	}
}

void YiEventSystem::_PushEvent(const EVENT& event)
{
	if (!event.pEventDef) return;

	if (event.pEventDef->delayProcess)
	{
		m_delayQueueEvent.push_back(event);
	}
	else
	{
		m_queueEvent.push_back(event);
	}
}

void YiEventSystem::_ProcessEvent(const EVENT& event)
{
	EVENT_DEFINE* pEventDef = event.pEventDef;
	if (!pEventDef) return;


	if (!(pEventDef->listFuncNotify.empty()))
	{
		EVENT_DEFINE::REGISTER_STRUCT::iterator it;
		for (it = pEventDef->listFuncNotify.begin(); it != pEventDef->listFuncNotify.end(); it++)
		{
			if ((*it).first)((*it).first)(&event, (*it).second);
		}
	}
}

void YiEventSystem::PushEvent(GAME_EVENT_ID id, const std::vector<std::string>& vParam)
{
	if (m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsID[id];

	event.vArg = vParam;

	_PushEvent(event);
}

void YiEventSystem::PushEvent(GAME_EVENT_ID id)
{
	if (m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsID[id];

	_PushEvent(event);
}

void YiEventSystem::PushEvent(GAME_EVENT_ID id, INT iArg0)
{
	if (m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsID[id];

	CHAR szTemp[32];
	_snprintf_s(szTemp, 32, "%d", iArg0);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

void YiEventSystem::PushEvent(GAME_EVENT_ID id, FLOAT fArg0)
{
	if (m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsID[id];

	CHAR szTemp[32];
	_snprintf_s(szTemp, 32, "%f", fArg0);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

void YiEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0)
{
	if (m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsID[id];

	event.vArg.push_back(szArg0);

	_PushEvent(event);
}

void YiEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1)
{
	if (m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsID[id];

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);

	_PushEvent(event);
}

void YiEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT nArg2)
{
	if (m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsID[id];

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);
	CHAR szTemp[32];
	_snprintf_s(szTemp, 32, "%d", nArg2);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

void YiEventSystem::PushEvent(GAME_EVENT_ID id, INT iArg0, INT iArg1)
{
	if (m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsID[id];

	CHAR szTemp[32];

	_snprintf_s(szTemp, 32, "%d", iArg0);
	event.vArg.push_back(szTemp);
	_snprintf_s(szTemp, 32, "%d", iArg1);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

void YiEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT iArg2, INT iArg3)
{
	if (m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef = m_mapEventIndex_AsID[id];

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);

	CHAR szTemp[32];
	_snprintf_s(szTemp, 32, "%d", iArg2);
	event.vArg.push_back(szTemp);
	_snprintf_s(szTemp, 32, "%d", iArg3);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

void YiEventSystem::RegisterEventHandle(const std::string& nameEvent, FUNC_EVENT_HANDLE funHandle, UINT uOwnerData)
{
	if (!funHandle) return;

	EVENT_DEFINE* pEvent = m_mapEventIndex_AsName[nameEvent];
	if (!pEvent) return;

	pEvent->listFuncNotify.push_back(std::make_pair(funHandle, uOwnerData));
}

void YiEventSystem::ProcessAllEvent()
{
	if (!(m_delayQueueEvent.empty()))
	{
		const UINT WORK_STEP = 2;
		NCL::GameTimer t;
		t.GetTimeDeltaSeconds();
		t.Tick();
		float updateTime = t.GetTimeDeltaSeconds();
		if (updateTime >= WORK_STEP)
		{
		
			const EVENT& event = *(m_delayQueueEvent.begin());

			_ProcessEvent(event);

			m_delayQueueEvent.erase(m_delayQueueEvent.begin());
		}
	}

	register std::list< EVENT >::iterator it;
	for (it = m_queueEvent.begin(); it != m_queueEvent.end(); it++)
	{
		const EVENT& event = *it;

		
		bool bMultiPushed = false;
		for (register std::list< EVENT >::iterator itPrev = m_queueEvent.begin(); itPrev != it; itPrev++)
		{
			if (*itPrev == *it)
			{
				bMultiPushed = true;
				break;
			}
		}

		if (bMultiPushed) continue;

		_ProcessEvent(event);
	}
	m_queueEvent.clear();
}
