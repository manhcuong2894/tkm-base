#include "stdafx.h"
#include "CB_DangKyInGame.h"

#include <cctype>
#include <ctime>

#include "CBTMessageBox.h"
#include "DSPlaySound.h"
#include "NewUISystem.h"
#include "UIBaseDef.h"
#include "UIControls.h"
#include "UIMng.h"
#include "ZzzInterface.h"
#include "wsclientinline.h"

using namespace SEASON3B;

namespace
{
	bool CheckChuoiKyTuDacBiet(const char* str)
	{
		if (str == NULL)
		{
			return false;
		}

		for (const char* current = str; *current != 0; current++)
		{
			const char ch = *current;

			if (!isdigit((unsigned char)ch) && !(ch >= 'a' && ch <= 'z') && !(ch >= 'A' && ch <= 'Z'))
			{
				return false;
			}
		}

		return true;
	}
}

CB_DangKyInGame* CB_DangKyInGame::Instance()
{
	static CB_DangKyInGame s_Instance;
	return &s_Instance;
}

CB_DangKyInGame::CB_DangKyInGame()
{
	this->Clear();
	this->ResetCaptcha();
}

CB_DangKyInGame::~CB_DangKyInGame()
{
	for (int i = 0; i < TYPE_INPUT_DKTK::eMaxINPUT; i++)
	{
		SAFE_DELETE(CInputData[i]);
	}

	SAFE_DELETE(CInputCaptCha);
}

void CB_DangKyInGame::Clear()
{
	for (int i = 0; i < TYPE_INPUT_DKTK::eMaxINPUT; i++)
	{
		CInputData[i] = NULL;
	}

	CInputCaptCha = NULL;
	this->TimeSendRegTK = GetTickCount();
	this->OpenDKTK = false;
	this->EnterPressed = false;
}

bool CB_DangKyInGame::IsOpen() const
{
	return this->OpenDKTK;
}

void CB_DangKyInGame::Close()
{
	this->OpenDKTK = false;
	this->EnterPressed = false;

	for (int i = 0; i < TYPE_INPUT_DKTK::eMaxINPUT; i++)
	{
		if (this->CInputData[i])
		{
			this->CInputData[i]->SetState(UISTATE_HIDE);
		}
	}

	if (this->CInputCaptCha)
	{
		this->CInputCaptCha->SetState(UISTATE_HIDE);
	}
}

void CB_DangKyInGame::ResetCaptcha()
{
	static bool seeded = false;

	if (!seeded)
	{
		srand((unsigned int)time(NULL));
		seeded = true;
	}

	this->Captcha.clear();

	for (int i = 0; i < 4; i++)
	{
		this->Captcha.push_back((char)('0' + (rand() % 10)));
	}

	if (this->CInputCaptCha)
	{
		this->CInputCaptCha->SetText("");
	}
}

void CB_DangKyInGame::DrawBar(float x, float y, float w, float h, float r, float g, float b, float a)
{
	EnableAlphaTest(true);
	glColor4f(r, g, b, a);
	RenderColor(x, y, w, h, 0.0f, 0);
	EndRenderColor();
	glColor3f(1.0f, 1.0f, 1.0f);
	EnableAlphaTest(false);
}

void CB_DangKyInGame::DrawText(HFONT font, float x, float y, DWORD color, int width, int align, const char* text, ...)
{
	char buff[256] = { 0 };
	va_list args;
	va_start(args, text);
	vsprintf_s(buff, text, args);
	va_end(args);

	DWORD oldTextColor = g_pRenderText->GetTextColor();
	DWORD oldBgColor = g_pRenderText->GetBgColor();
	HFONT oldFont = g_pRenderText->GetFont();

	g_pRenderText->SetFont(font);
	g_pRenderText->SetTextColor(color);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText((int)x, (int)y, buff, width, 0, align);

	g_pRenderText->SetFont(oldFont);
	g_pRenderText->SetTextColor(oldTextColor);
	g_pRenderText->SetBgColor(oldBgColor);
}

bool CB_DangKyInGame::DrawButton(float x, float y, float w, float h, const char* text)
{
	bool click = false;
	const bool hover = (SEASON3B::CheckMouseIn(x, y, w, h) == TRUE);

	this->DrawBar(x, y, w, h, hover ? 0.42f : 0.22f, hover ? 0.22f : 0.12f, 0.05f, 0.88f);

	if (hover && SEASON3B::IsRelease(VK_LBUTTON))
	{
		PlayBuffer(SOUND_CLICK01);
		click = true;
	}

	this->DrawText(g_hFontBold, x, y + 4.0f, 0xFFFFFFFF, (int)w, RT3_SORT_CENTER, text);
	return click;
}

bool CB_DangKyInGame::RenderInput(float x, float y, float w, float h, CUITextInputBox*& input, UIOPTIONS option, int maxChar, bool isPass)
{
	if (input == NULL)
	{
		input = new CUITextInputBox;
		input->Init(gwinhandle->GethWnd(), w, h, maxChar, isPass);
		input->SetPosition(x, y);
		input->SetTextColor(255, 255, 255, 255);
		input->SetBackColor(0, 0, 0, 0);
		input->SetFont(g_hFont);
		input->SetState(UISTATE_NORMAL);
		input->SetOption(option);
		return false;
	}

	this->DrawBar(x - 3.0f, y - 3.0f, w + 6.0f, h + 5.0f, 0.0f, 0.0f, 0.0f, 0.85f);
	if (input->GetState() == UISTATE_HIDE)
	{
		input->SetState(UISTATE_NORMAL);
	}
	input->SetPosition(x, y);
	input->Render();

	if (SEASON3B::CheckMouseIn(x - 3.0f, y - 3.0f, w + 6.0f, h + 5.0f) == TRUE && SEASON3B::IsRelease(VK_LBUTTON))
	{
		input->GiveFocus(TRUE);
	}

	return true;
}

bool CB_DangKyInGame::CheckCaptcha(const char* input) const
{
	return input != NULL && this->Captcha.compare(input) == 0;
}

bool CB_DangKyInGame::HandleConfirmSubmit()
{
	char captcha[5] = { 0 };

	if (this->CInputCaptCha == NULL)
	{
		return false;
	}

	this->CInputCaptCha->GetText(captcha, sizeof(captcha));

	if (!this->CheckCaptcha(captcha))
	{
		leaf::CBTMessageBox(gwinhandle->GethWnd(), "Sai ma Captcha", "Dang ky tai khoan", MB_OK, true);
		this->ResetCaptcha();
		return false;
	}

	return this->RequsetDKTK();
}

void CB_DangKyInGame::OpenOnOff()
{
	this->OpenDKTK = !this->OpenDKTK;

	if (this->OpenDKTK)
	{
		this->ResetCaptcha();
	}
	else
	{
		this->Close();
	}
}

bool CB_DangKyInGame::RenderWindow(int X, int Y)
{
	(void)X;
	(void)Y;

	if (!this->OpenDKTK)
	{
		return false;
	}

	const float windowW = 262.0f;
	const float windowH = 250.0f;
	float startX = (640.0f * 0.5f) - (windowW * 0.5f);
	float startY = 150.0f;

	window_backmsg(startX, startY, windowW, windowH);
	this->DrawText(g_hFontBold, startX, startY + 11.0f, 0xFFFFFFFF, (int)windowW, RT3_SORT_CENTER, "Dang ky tai khoan");

	const float closeX = startX + windowW - 27.0f;
	const float closeY = startY + 5.0f;
	SEASON3B::RenderImageF(IMAGE_BUTTON_CLOSE_NAVI, closeX, closeY, 16.0f, 16.0f, 0.0f, 0.0f, 15.0f, 15.0f);

	if (SEASON3B::CheckMouseIn(closeX, closeY, 16.0f, 16.0f) == TRUE && SEASON3B::IsRelease(VK_LBUTTON))
	{
		this->Close();
		return true;
	}

	this->DrawText(g_hFontBold, startX + 18.0f, startY + 48.0f, 0xFFA200B8, (int)(windowW - 36.0f), RT3_SORT_CENTER, "Tai khoan chi dung 0-9, a-z, A-Z");

	static const char* textStatus[] = { "Tai khoan:", "Mat khau:", "7 so bi mat:", "So dien thoai:" };
	static const int inputType[] = { UIOPTION_NOLOCALIZEDCHARACTERS, UIOPTION_NOLOCALIZEDCHARACTERS, UIOPTION_NUMBERONLY, UIOPTION_NUMBERONLY };
	static const int maxInput[] = { 10, 10, 7, 11 };

	const float inputW = 110.0f;
	float inputY = startY + 80.0f;

	for (int i = 0; i < TYPE_INPUT_DKTK::eMaxINPUT; i++)
	{
		this->DrawText(g_hFontBold, startX + 30.0f, inputY, 0xFFFFFFFF, 85, RT3_SORT_LEFT, textStatus[i]);

		if (this->RenderInput(startX + 120.0f, inputY, inputW, 14.0f, this->CInputData[i], (UIOPTIONS)inputType[i], maxInput[i], (i == Pass)))
		{
			if ((i + 1) < TYPE_INPUT_DKTK::eMaxINPUT && this->CInputData[i + 1])
			{
				this->CInputData[i]->SetTabTarget(this->CInputData[i + 1]);
			}
		}

		inputY += 20.0f;
	}

	inputY += 18.0f;
	this->DrawText(g_hFontBold, startX + 40.0f, inputY + 4.0f, 0x62FF00FF, 80, RT3_SORT_LEFT, "Captcha");
	this->DrawBar(startX + 105.0f, inputY, 122.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.90f);
	this->DrawBar(startX + 108.0f, inputY + 2.0f, 50.0f, 16.0f, 1.0f, 0.22f, 0.0f, 1.0f);
	this->DrawText(g_hFontBold, startX + 108.0f, inputY + 5.0f, 0xFFFFFFB8, 50, RT3_SORT_CENTER, "%s", this->Captcha.c_str());
	this->RenderInput(startX + 166.0f, inputY + 3.0f, 50.0f, 14.0f, this->CInputCaptCha, UIOPTION_NUMBERONLY, 4, false);

	bool submitByEnter = false;

	if ((GetKeyState(VK_RETURN) & 0x8000) != 0)
	{
		if (!this->EnterPressed)
		{
			this->EnterPressed = true;
			submitByEnter = true;
		}
	}
	else
	{
		this->EnterPressed = false;
	}

	if (this->DrawButton(startX + 86.0f, startY + 218.0f, 90.0f, 18.0f, "Xac nhan") || submitByEnter)
	{
		this->HandleConfirmSubmit();
	}

	if (SEASON3B::CheckMouseIn(startX, startY, windowW, windowH) == TRUE)
	{
		MouseLButton = false;
		MouseLButtonPush = false;
		MouseLButtonPop = false;
		MouseRButton = false;
		MouseRButtonPush = false;
		MouseRButtonPop = false;
	}

	return true;
}

bool CB_DangKyInGame::RequsetDKTK()
{
	char szID[MAX_ID_SIZE + 1] = { 0 };
	char szPass[MAX_PASSWORD_SIZE + 1] = { 0 };
	char szSno[8] = { 0 };
	char szSDT[12] = { 0 };

	this->CInputData[Account]->GetText(szID, sizeof(szID));
	this->CInputData[Pass]->GetText(szPass, sizeof(szPass));
	this->CInputData[Snonumber]->GetText(szSno, sizeof(szSno));
	this->CInputData[Phone]->GetText(szSDT, sizeof(szSDT));

	if (this->TimeSendRegTK > GetTickCount())
	{
		leaf::CBTMessageBox(gwinhandle->GethWnd(), "Thao tac cham lai.", "Dang ky tai khoan", MB_OK, true);
		return false;
	}

	if (strlen(szID) < 1)
	{
		leaf::CBTMessageBox(gwinhandle->GethWnd(), "Vui long nhap tai khoan.", "Dang ky tai khoan", MB_OK, true);
		return false;
	}

	if (strlen(szPass) < 1)
	{
		leaf::CBTMessageBox(gwinhandle->GethWnd(), "Vui long nhap mat khau.", "Dang ky tai khoan", MB_OK, true);
		return false;
	}

	if (strlen(szSno) < 7)
	{
		leaf::CBTMessageBox(gwinhandle->GethWnd(), "Vui long nhap 7 so bao mat.", "Dang ky tai khoan", MB_OK, true);
		return false;
	}

	if (strlen(szSDT) < 10)
	{
		leaf::CBTMessageBox(gwinhandle->GethWnd(), "Vui long nhap so dien thoai.", "Dang ky tai khoan", MB_OK, true);
		return false;
	}

	if (!CheckChuoiKyTuDacBiet(szID) || !CheckChuoiKyTuDacBiet(szPass))
	{
		leaf::CBTMessageBox(gwinhandle->GethWnd(), "Tai khoan hoac mat khau co ky tu khong hop le.", "Dang ky tai khoan", MB_OK, true);
		return false;
	}

	PMSG_REGISTER_MAIN_SEND pMsg = { 0 };
	pMsg.header.set(0xD3, 0x05, sizeof(pMsg));
	pMsg.TypeSend = 0x01;

	memcpy(pMsg.account, szID, sizeof(pMsg.account));
	memcpy(pMsg.password, szPass, sizeof(pMsg.password));
	memcpy(pMsg.numcode, szSno, sizeof(pMsg.numcode));
	memcpy(pMsg.sodienthoai, szSDT, sizeof(pMsg.sodienthoai));

	DataSend((uint8_t*)&pMsg, pMsg.header.Size);

	this->ResetCaptcha();
	this->TimeSendRegTK = GetTickCount() + 5000;
	return true;
}

void CB_DangKyInGame::RecvKQRegInGame(XULY_CGPACKET* lpMsg)
{
	if (lpMsg == NULL)
	{
		return;
	}

	char szID[MAX_ID_SIZE + 1] = { 0 };
	char szPass[MAX_PASSWORD_SIZE + 1] = { 0 };

	if (this->CInputData[Account])
	{
		this->CInputData[Account]->GetText(szID, sizeof(szID));
	}

	if (this->CInputData[Pass])
	{
		this->CInputData[Pass]->GetText(szPass, sizeof(szPass));
	}

	switch (lpMsg->ThaoTac)
	{
	case CB_DangKyInGame::eDangKyThanhCong:
	{
		char message[128] = { 0 };
		sprintf_s(message, "Dang ky thanh cong.\nID: %s", szID);
		leaf::CBTMessageBox(gwinhandle->GethWnd(), message, "Ket qua", MB_OK, true);

		CUIMng& rUIMng = CUIMng::Instance();
		rUIMng.m_LoginWin.GetIDInputBox()->SetText(szID);
		rUIMng.m_LoginWin.GetPassInputBox()->SetText(szPass);

#ifdef SAVE_ACCOUNT_SYSTEM
		if (gmProtect->remember_account)
		{
			rUIMng.m_LoginWin.SaveAccount(szID, szPass);
		}
#endif

		this->Close();
	}
	break;
	case CB_DangKyInGame::eTaiKhoanDaTonTai:
		leaf::CBTMessageBox(gwinhandle->GethWnd(), "Tai khoan da ton tai.", "Ket qua", MB_OK, true);
		break;
	case CB_DangKyInGame::eDuLieuNhapKhongDung:
		leaf::CBTMessageBox(gwinhandle->GethWnd(), "Thong tin nhap khong phu hop.", "Ket qua", MB_OK, true);
		break;
	default:
		leaf::CBTMessageBox(gwinhandle->GethWnd(), "Khong the dang ky tai khoan.", "Ket qua", MB_OK, true);
		break;
	}
}
