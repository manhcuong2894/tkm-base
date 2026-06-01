#pragma once

#include <string>

#include "WSclient.h"
#include "UIControls.h"

class CUITextInputBox;

struct XULY_CGPACKET
{
	PSBMSG_HEADER header;
	DWORD ThaoTac;
};

class CB_DangKyInGame
{
	enum TYPE_INPUT_DKTK
	{
		Account,
		Pass,
		Snonumber,
		Phone,
		eMaxINPUT,
	};

	struct PMSG_REGISTER_MAIN_SEND
	{
		PSBMSG_HEADER header;
		BYTE TypeSend;
		char account[11];
		char password[21];
		char numcode[8];
		char sodienthoai[14];
	};

	enum eRecvKetQua
	{
		eDangKyThanhCong = 1,
		eTaiKhoanDaTonTai = 2,
		eDuLieuNhapKhongDung = 3,
		eDatLaiMatKhauThanhCong = 11,
		eThongTinBaoMatKhongChinhXac = 12,
	};

public:
	CB_DangKyInGame();
	~CB_DangKyInGame();

	static CB_DangKyInGame* Instance();

	bool RenderWindow(int X, int Y);
	void OpenOnOff();
	bool RequsetDKTK();
	void RecvKQRegInGame(XULY_CGPACKET* lpMsg);
	void Clear();
	bool HandleConfirmSubmit();
	bool IsOpen() const;

private:
	void Close();
	void ResetCaptcha();
	void OpenMessageBox(bool closeRegister, const char* caption, const char* text, ...);
	bool DrawMessageBox();
	bool DrawButton(float x, float y, float w, float h, const char* text);
	void DrawText(HFONT font, float x, float y, DWORD color, int width, int align, const char* text, ...);
	void DrawBar(float x, float y, float w, float h, float r, float g, float b, float a);
	bool RenderInput(float x, float y, float w, float h, CUITextInputBox*& input, UIOPTIONS option, int maxChar, bool isPass);
	bool CheckCaptcha(const char* input) const;

	CUITextInputBox* CInputData[TYPE_INPUT_DKTK::eMaxINPUT];
	CUITextInputBox* CInputCaptCha;
	DWORD TimeSendRegTK;
	DWORD ButtonClickTime;
	bool OpenDKTK;
	bool EnterPressed;
	bool MsgBoxOpen;
	bool MsgCloseRegister;
	int MsgLine;
	std::string Captcha;
	std::string MsgBoxCaption;
	std::string MsgBoxText;
};

#define gCB_DangKyInGame (CB_DangKyInGame::Instance())
