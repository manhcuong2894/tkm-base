#include "stdafx.h"
#include "NewPartyMatching.h"

SEASON3B::CNewPartyMatching::CNewPartyMatching()
{
}

SEASON3B::CNewPartyMatching::~CNewPartyMatching()
{
}

bool SEASON3B::CNewPartyMatching::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	return false;
}

void SEASON3B::CNewPartyMatching::Release()
{
}

void SEASON3B::CNewPartyMatching::SetPos(int x, int y)
{
}

bool SEASON3B::CNewPartyMatching::UpdateMouseEvent()
{
	return false;
}

bool SEASON3B::CNewPartyMatching::UpdateKeyEvent()
{
	return false;
}

bool SEASON3B::CNewPartyMatching::Update()
{
	return false;
}

bool SEASON3B::CNewPartyMatching::Render()
{
	if (m_iTabIndex == 0)
	{
		this->SearchPartyTap();
	}
	else if (m_iTabIndex == 1)
	{
		this->FindPartyMembertap();
	}
	return true;
}

float SEASON3B::CNewPartyMatching::GetLayerDepth()
{
	return 0.0f;
}

void SEASON3B::CNewPartyMatching::EventOrderWindows(double WindowsX, double WindowsY)
{
}

void SEASON3B::CNewPartyMatching::OpenningProcess()
{
}

void SEASON3B::CNewPartyMatching::ClosingProcess()
{
}

void SEASON3B::CNewPartyMatching::SearchPartyTap()
{
}

void SEASON3B::CNewPartyMatching::FindPartyMembertap()
{
}
