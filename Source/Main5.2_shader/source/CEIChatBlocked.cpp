#include "stdafx.h"
#include "ReadScript.h"
#include "UIControls.h"
#include "NewUISystem.h"
#include "CEIChatBlocked.h"
#include "Utilities/Log/muConsoleDebug.h"

SEASON3B::CEIChatBlocked::CEIChatBlocked()
{
	m_Pos.x = 0;
	m_Pos.y = 0;
	targetList = NULL;
	m_BtnRemove = NULL;
	m_blockedInputBox = NULL;
}

SEASON3B::CEIChatBlocked::~CEIChatBlocked()
{
	this->Release();
}

void SEASON3B::CEIChatBlocked::Create(int x, int y)
{
	targetList = new CUIListBox();
	m_BtnRemove = new CNewUIButton();
	m_blockedInputBox = new CUITextInputBox;

	targetList->Create((x + 20.f), (y + 100.f), 150, 80, 6, 50);


	m_blockedInputBox->Init(gwinhandle->GethWnd(), 80, 14, MAX_ID_SIZE + 1);
	m_blockedInputBox->SetTextColor(255, 255, 230, 210);
	m_blockedInputBox->SetBackColor(0, 0, 0, 25);
	m_blockedInputBox->SetFont(g_hFont);
	m_blockedInputBox->SetState(UISTATE_NORMAL);
	m_blockedInputBox->SetPlaceHolder("Escribe aqui...");


	m_BtnRemove->ChangeText(GlobalText[3506]);
	m_BtnRemove->ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnRemove->ChangeButtonImgState(true, BITMAP_btn_empty_very_small, true);
	m_BtnRemove->ChangeButtonInfo((x + 116.f), (y + 162.f), 53, 23);

	this->SetPos(x, y);

	this->OpenInstance();

	this->LoadImages();
}

void SEASON3B::CEIChatBlocked::Release()
{
	this->UnLoadImages();

	SAFE_DELETE(targetList);
	SAFE_DELETE(m_BtnRemove);
	SAFE_DELETE(m_blockedInputBox);
}

void SEASON3B::CEIChatBlocked::LoadImages()
{
	LoadBitmap("Interface\\HUD\\newui_input_box.tga", BITMAP_input_box, GL_LINEAR);
}

void SEASON3B::CEIChatBlocked::UnLoadImages()
{
	DeleteBitmap(BITMAP_input_box);
}

void SEASON3B::CEIChatBlocked::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	targetList->SetPos((x + 20.f), (y + 75.f));

	m_BtnRemove->SetPos((x + 116.f), (y + 162.f));

	m_blockedInputBox->SetPosition((x + 50), (y + 52));
}

void SEASON3B::CEIChatBlocked::Render()
{
	SEASON3B::window_backmsg(m_Pos.x, m_Pos.y, 190, 200);

	SEASON3B::RenderImageF(BITMAP_input_box, (m_Pos.x + 40.f), (m_Pos.y + 50.f), 100.f, 18.f, 0.f, 0.f, 100.f, 18.f);

	SEASON3B::RenderImageF(IMAGE_MARK_POIN, (m_Pos.x + 20.f), (m_Pos.y + 35.f), 8.f, 8.f, 0.f, 0.f, 14.f, 14.f);

	m_BtnRemove->Render();

	m_blockedInputBox->Render();

	g_pNameWindow->RenderTooltip((m_Pos.x + 20.f), (m_Pos.y + 75.f), 155.f, 80.f, true);

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(CLRDW_GOLD);
	g_pRenderText->SetBgColor(CLRDW_ALPHA);
	g_pRenderText->RenderText((m_Pos.x + 32.f), (m_Pos.y + 35.f), GlobalText[125], 0);

	targetList->Render(false);
}

bool SEASON3B::CEIChatBlocked::Update(CNewUIObj* pObj)
{
	if (m_blockedInputBox)
	{
		if (m_blockedInputBox->HaveFocus() && pObj->GetRelatedWnd() != m_blockedInputBox->GetHandle())
		{
			pObj->SetRelatedWnd(m_blockedInputBox->GetHandle());
		}

		if (pObj->IsVisible())
		{
			m_blockedInputBox->DoAction();
		}
	}

	return false;
}

bool SEASON3B::CEIChatBlocked::UpdateMouseEvent()
{
	if (CheckMouseIn(m_Pos.x, m_Pos.y, 190, 200))
	{
		targetList->UpdateMouseEvent();

		if (m_BtnRemove->UpdateMouseEvent())
		{
			targetList->DelSelectedListBoxText();

			this->SaveInstance();

			return true;
		}
	}
	return SEASON3B::CheckMouseIn(m_Pos.x, m_Pos.y, 190, 200);
}

void SEASON3B::CEIChatBlocked::OpenInstance()
{
	if ((SMDFile = fopen("Data\\ChatBlocked.txt", "rb")) == NULL)
		return;

	targetList->ClearText();

	SMDToken Token;
	while (true)
	{
		Token = (*GetToken)();//¹øÈ£
		if (Token == END)
			break;
		if (Token == NAME && strcmp("end", TokenString) == NULL)
			break;

		targetList->AddText(TokenString);
	}
	fclose(SMDFile);
}

void SEASON3B::CEIChatBlocked::SaveInstance()
{
	FILE* fp = fopen("Data\\ChatBlocked.txt", "wb"); // Modo "rb" (lectura binaria)

	if (fp == NULL)
	{
		return;
	}

	const std::vector<unicode::t_string>& textList = targetList->GetData();

	unicode::t_string strLine = "\n";

	for (size_t i = 0; i < textList.size(); ++i)
	{
		fwrite(textList[i].data(), 1, textList[i].size(), fp);
		fwrite(strLine.data(), 1, strLine.size(), fp);
	}

	fclose(fp);
}

void SEASON3B::CEIChatBlocked::EnterInstance()
{
	char szWhisperID[MAX_ID_SIZE + 1] = { '\0' };

	if (m_blockedInputBox)
	{
		m_blockedInputBox->GetText(szWhisperID, MAX_ID_SIZE + 1);

		targetList->AddText(szWhisperID);

		if (szWhisperID[0] != '\0')
		{
			this->SaveInstance();
		}

		m_blockedInputBox->SetText("");
	}
}

bool SEASON3B::CEIChatBlocked::CheckInstance(const unicode::t_char* pzText)
{
	if (SceneFlag == MAIN_SCENE)
	{
		if (targetList != NULL)
			return targetList->CheckText(pzText);
	}

	return false;
}

void SEASON3B::CEIChatBlocked::SetText(const unicode::t_char* pzText)
{
	if (m_blockedInputBox)
		return m_blockedInputBox->SetText(pzText);
}

bool SEASON3B::CEIChatBlocked::HaveFocus()
{
	if (m_blockedInputBox)
		return m_blockedInputBox->HaveFocus();

	return false;
}
