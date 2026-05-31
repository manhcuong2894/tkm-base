// w_BuffTimeControl.h: interface for the BuffTimeControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_W_BUFFTIMECONTROL_H__BBD38A0F_DFE7_4E2E_893A_39A5C7C0803C__INCLUDED_)
#define AFX_W_BUFFTIMECONTROL_H__BBD38A0F_DFE7_4E2E_893A_39A5C7C0803C__INCLUDED_

#pragma once

BoostSmartPointer(BuffTimeControl);

class BuffTimeControl
{
public:
	static BuffTimeControlPtr Make();
	virtual ~BuffTimeControl();

public:
	void RegisterBuffTime(eBuffState bufftype, uint32_t curbufftime);
	bool UnRegisterBuffTime(eBuffState bufftype);
	bool IsBuffTime(uint32_t bufftype);

public:
	void GetBuffStringTime(eBuffState bufftype, std::string& timeText);
	void GetBuffStringTime(uint32_t type, std::string& timeText, bool issecond = true);
	const uint32_t GetBuffTime(uint32_t type);
	void GetStringTime(uint32_t time, std::string& timeText, bool isSecond = true);

public:
	bool HandleWindowMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& result);

private:
	uint32_t CheckBuffTimeType(eBuffState bufftype);
	uint32_t GetBuffEventTime(uint32_t bufftimetype);
	uint32_t GetBuffMaxTime(eBuffState bufftype, uint32_t curbufftime = 0);
	bool CheckBuffTime(uint32_t type);
	BuffTimeControl();

private:
	struct BuffTimeInfo
	{
		eBuffState s_BuffType;
		uint32_t    s_CurBuffTime;
		uint32_t    s_EventBuffTime;

		BuffTimeInfo() : s_BuffType(eBuffNone), s_CurBuffTime(0), s_EventBuffTime(0) {}
	};
	typedef std::map<uint32_t, BuffTimeInfo>		BuffTimeInfoMap;

private:
	BuffTimeInfoMap		m_BuffTimeList;
};

#endif // !defined(AFX_W_BUFFTIMECONTROL_H__BBD38A0F_DFE7_4E2E_893A_39A5C7C0803C__INCLUDED_)
