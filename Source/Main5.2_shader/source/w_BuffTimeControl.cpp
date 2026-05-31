// w_BuffTimeControl.cpp: implementation of the BuffTimeControl class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZzzInfomation.h"
#include "UIManager.h"
#include "ItemAddOptioninfo.h"
#include "w_BuffTimeControl.h"

BuffTimeControlPtr BuffTimeControl::Make()
{
	BuffTimeControlPtr bufftimecontrol(new BuffTimeControl);
	return bufftimecontrol;
}

BuffTimeControl::BuffTimeControl()
{

}

BuffTimeControl::~BuffTimeControl()
{
	for (BuffTimeInfoMap::iterator iter = m_BuffTimeList.begin(); iter != m_BuffTimeList.end(); )
	{
		BuffTimeInfoMap::iterator tempiter = iter;
		++iter;

		uint32_t bufftimetype = static_cast<uint32_t>((*tempiter).first);

		::KillTimer(gwinhandle->GethWnd(), bufftimetype);
		m_BuffTimeList.erase(tempiter);
	}

	m_BuffTimeList.clear();
}

uint32_t BuffTimeControl::CheckBuffTimeType(eBuffState bufftype)
{
	if (g_IsBuffClass(bufftype) == eBuffClass_Count)
	{
		return eBuffTime_None;
	}

	BuffInfo bInfo = g_BuffInfo(bufftype);

	return uint32_t(1005 + bInfo.s_BuffEffectType);
}

uint32_t BuffTimeControl::GetBuffEventTime(uint32_t bufftimetype)
{
	return 1000;
}

uint32_t BuffTimeControl::GetBuffMaxTime(eBuffState bufftype, uint32_t curbufftime)
{
	if (curbufftime == 0)
	{
		const BuffInfo& buffinfo = g_BuffInfo(bufftype);

		if (buffinfo.s_ItemType == 0xff)
		{
			return -1;
		}
		else
		{
			const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo->GetItemAddOtioninfo(ITEMINDEX(buffinfo.s_ItemType, buffinfo.s_ItemIndex));

			if (Item_data.m_Time == 0)
				return -1;

			return Item_data.m_Time;
		}
	}

	return curbufftime;
}

bool BuffTimeControl::IsBuffTime(uint32_t bufftype)
{
	BuffTimeInfoMap::iterator iter = m_BuffTimeList.find(bufftype);

	return (iter != m_BuffTimeList.end());
}

void BuffTimeControl::RegisterBuffTime(eBuffState bufftype, uint32_t curbufftime)
{
	uint32_t bufftimetype = this->CheckBuffTimeType(bufftype);

	curbufftime = this->GetBuffMaxTime(bufftype, curbufftime);

	if (bufftimetype)
	{
		if (curbufftime != -1)
		{
			if (!this->IsBuffTime(bufftimetype))
			{
				BuffTimeInfo buffinfo;
				buffinfo.s_BuffType = bufftype;
				buffinfo.s_CurBuffTime = curbufftime * 1000;
				buffinfo.s_EventBuffTime = GetTickCount();
				m_BuffTimeList.insert(std::make_pair(bufftimetype, buffinfo));
				::SetTimer(gwinhandle->GethWnd(), bufftimetype, 900, NULL);
			}
		}
	}
}

bool BuffTimeControl::UnRegisterBuffTime(eBuffState bufftype)
{
	uint32_t bufftimetype = this->CheckBuffTimeType(bufftype);

	BuffTimeInfoMap::iterator iter = m_BuffTimeList.find(bufftimetype);

	if (iter != m_BuffTimeList.end())
	{
		::KillTimer(gwinhandle->GethWnd(), bufftimetype);
		g_ConsoleDebug->Write(MCD_NORMAL, "[Buff End] No. %d\r\n", bufftimetype);
		m_BuffTimeList.erase(iter);
		return true;
	}
	return false;
}

void BuffTimeControl::GetBuffStringTime(eBuffState bufftype, std::string& timeText)
{
	for (BuffTimeInfoMap::iterator iter = m_BuffTimeList.begin(); iter != m_BuffTimeList.end(); ++iter)
	{
		BuffTimeInfo& bufftimeinfo = (*iter).second;

		if (bufftimeinfo.s_BuffType == bufftype)
		{
			float fTime = bufftimeinfo.s_CurBuffTime * 0.001f;
			GetStringTime(fTime, timeText, true);
		}
	}
}

void BuffTimeControl::GetBuffStringTime(uint32_t type, std::string& timeText, bool issecond)
{
	BuffTimeInfoMap::iterator iter = m_BuffTimeList.find(type);

	if (iter != m_BuffTimeList.end())
	{
		BuffTimeInfo& bufftimeinfo = (*iter).second;
		float fTime = bufftimeinfo.s_CurBuffTime * 0.001f;
		GetStringTime(fTime, timeText, issecond);
	}
}

const uint32_t BuffTimeControl::GetBuffTime(uint32_t type)
{
	BuffTimeInfoMap::iterator iter = m_BuffTimeList.find(type);

	if (iter != m_BuffTimeList.end())
	{
		BuffTimeInfo& bufftimeinfo = (*iter).second;

		return bufftimeinfo.s_CurBuffTime;
	}
	return 0;
}

void BuffTimeControl::GetStringTime(uint32_t time, std::string& timeText, bool isSecond)
{
	char buffer[100];

	if (isSecond)
	{
		uint32_t day = time / (1440 * 60);
		uint32_t oClock = (time - (day * (1440 * 60))) / 3600;
		uint32_t minutes = (time - ((oClock * 3600) + (day * (1440 * 60)))) / 60;
		uint32_t second = time % 60;

		if (day != 0)
		{
			std::sprintf(buffer, "%d %s %d %s %d %s %d %s", day, GlobalText[2298], oClock, GlobalText[2299], minutes, GlobalText[2300], second, GlobalText[2301]);
			timeText = buffer;
		}
		else if (day == 0 && oClock != 0)
		{
			std::sprintf(buffer, "%d %s %d %s %d %s", oClock, GlobalText[2299], minutes, GlobalText[2300], second, GlobalText[2301]);
			timeText = buffer;
		}
		else if (day == 0 && oClock == 0 && minutes != 0)
		{
			std::sprintf(buffer, "%d %s %d %s", minutes, GlobalText[2300], second, GlobalText[2301]);
			timeText = buffer;
		}
		else if (day == 0 && oClock == 0 && minutes == 0)
		{
			std::sprintf(buffer, "%s", GlobalText[2308]);
			timeText = buffer;
		}
	}
	else
	{
		uint32_t day = time / 1440;
		uint32_t oClock = (time - (day * 1440)) / 60;
		uint32_t minutes = time % 60;

		if (day != 0)
		{
			std::sprintf(buffer, "%d %s %d %s %d %s", day, GlobalText[2298], oClock, GlobalText[2299], minutes, GlobalText[2300]);
			timeText = buffer;
		}
		else if (day == 0 && oClock != 0)
		{
			std::sprintf(buffer, "%d %s %d %s", oClock, GlobalText[2299], minutes, GlobalText[2300]);
			timeText = buffer;
		}
		else if (day == 0 && oClock == 0 && minutes != 0)
		{
			std::sprintf(buffer, "%d %s", minutes, GlobalText[2300]);
			timeText = buffer;
		}
	}
}

bool BuffTimeControl::CheckBuffTime(uint32_t type)
{
	BuffTimeInfoMap::iterator iter = m_BuffTimeList.find(type);

	if (iter != m_BuffTimeList.end())
	{
		BuffTimeInfo& bufftimeinfo = (*iter).second;

		uint32_t iCurBufftime = bufftimeinfo.s_CurBuffTime;
		if (iCurBufftime > GetTickCount() - bufftimeinfo.s_EventBuffTime)
		{
			iCurBufftime -= GetTickCount() - bufftimeinfo.s_EventBuffTime;
		}
		else
		{
			iCurBufftime = 0;
		}
		bufftimeinfo.s_EventBuffTime = GetTickCount();

		if (iCurBufftime <= 0)
		{
			bufftimeinfo.s_CurBuffTime = 0;
			return false;
		}
		else
		{
			bufftimeinfo.s_CurBuffTime = iCurBufftime;
			return true;
		}
	}
	return false;
}

bool BuffTimeControl::HandleWindowMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& result)
{
	if (message == WM_TIMER)
	{
		if (wParam != eBuffTime_None && wParam >= eBuffTime_Hellowin)
		{
			if (!CheckBuffTime(static_cast<uint32_t>(wParam)))
			{
				KillTimer(gwinhandle->GethWnd(), static_cast<uint32_t>(wParam));
			}
			return true;
		}
	}
	return false;
}
