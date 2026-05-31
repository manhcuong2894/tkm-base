#include "stdafx.h"
#include "NewUISystem.h"
#include "NewUIEventProgresstime.h"

SEASON3B::CNewUIEventProgresstime::CNewUIEventProgresstime()
{
	this->m_pNewUIMng = NULL;
	this->m_Pos.x = 0;
	this->m_Pos.y = 0;
	this->Init();
}

SEASON3B::CNewUIEventProgresstime::~CNewUIEventProgresstime()
{
	this->Release();
}

bool SEASON3B::CNewUIEventProgresstime::Create(CNewUIManager* pNewUIMng, float x, float y)
{
	if (pNewUIMng)
	{
		this->m_pNewUIMng = pNewUIMng;

		this->m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_EVENT_TIME, this);

		this->LoadImages();

		this->SetPos(x, y);

		this->SetButtonInfo();

		this->Show(false);

		return true;
	}
	else
	{
		return false;
	}
}

void SEASON3B::CNewUIEventProgresstime::Release()
{
	if (this->m_pNewUIMng)
	{
		this->m_pNewUIMng->RemoveUIObj(this);

		this->UnloadImages();
	}
}

void SEASON3B::CNewUIEventProgresstime::Init()
{
	this->m_EventIndex = 0;
	this->m_EventTotal[0] = 0;
	this->m_EventTotal[1] = 0;
	this->m_EventTotal[2] = 0;
	this->m_iViewCount = 10;
	this->m_EventTimeTickCount = 0;
}

void SEASON3B::CNewUIEventProgresstime::SetPos(float x, float y)
{
	this->m_Pos.x = x;
	this->m_Pos.y = y;
}

void SEASON3B::CNewUIEventProgresstime::SetButtonInfo()
{
}

void SEASON3B::CNewUIEventProgresstime::LoadImages()
{
	LoadBitmap("Interface\\HUD\\top_back_3.tga", IMAGE_TOP_BACK3, GL_LINEAR);
}

void SEASON3B::CNewUIEventProgresstime::UnloadImages()
{
	DeleteBitmap(IMAGE_TOP_BACK3);
}

bool SEASON3B::CNewUIEventProgresstime::UpdateKeyEvent()
{
	if (IsVisible() == true)
	{
		if (SEASON3B::IsPress(VK_ESCAPE))
		{
			g_pNewUISystem->Hide(INTERFACE_EVENT_TIME);
			return false;
		}
	}
	return true;
}

bool SEASON3B::CNewUIEventProgresstime::UpdateMouseEvent()
{
	if (SEASON3B::CheckMouseIn(this->m_Pos.x, this->m_Pos.y, 214.f, 230.f))
	{
		if (SEASON3B::CheckMouseIn(this->m_Pos.x + 192.f, this->m_Pos.y + 3.5f, 19.f, 19.f) && SEASON3B::IsRelease(VK_LBUTTON))
		{
			g_pNewUISystem->Hide(INTERFACE_EVENT_TIME);
			return false;
		}
	}
	return !SEASON3B::CheckMouseIn(this->m_Pos.x, this->m_Pos.y, 214.f, 230.f);
}

bool SEASON3B::CNewUIEventProgresstime::Render()
{
	EnableAlphaTest(true);

	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderFrame();

	RenderText();

	DisableAlphaBlend();

	return true;
}

bool SEASON3B::CNewUIEventProgresstime::Update()
{
	return true;
}

float SEASON3B::CNewUIEventProgresstime::GetLayerDepth()
{
	return 10.0f;
}

void SEASON3B::CNewUIEventProgresstime::OpenningProcess()
{
}

void SEASON3B::CNewUIEventProgresstime::ClosingProcess()
{
}

void SEASON3B::CNewUIEventProgresstime::RenderFrame()
{
	float x = m_Pos.x;
	float y = m_Pos.y;

	RenderImageF(IMAGE_TOP_BACK3, x, y, 214.f, 230.f, 0.0, 0.0, 854.0, 917.0); //-- background
}

void SEASON3B::CNewUIEventProgresstime::RenderText()
{
}
