#pragma once
#define TRAY_ID_ICON			(WM_USER + 100)
#define TRAY_ID_MESSAGE			(WM_USER + 101)

//#define 
#include "Resolutions.h"

class CChatRoomSocketList;

class CWINHANDLE
{
public:
	CWINHANDLE();
	virtual~CWINHANDLE();
	void Release();
	HWND Create(HINSTANCE hCurrentInst, uint32_t RenderSizeX, uint32_t RenderSizeY);
	void Destroyer();
	void InitSize(uint32_t RenderSizeX, uint32_t RenderSizeY);
	void SetFontSize(uint32_t FontSize);
	void Resize(uint32_t RenderSizeX, uint32_t RenderSizeY);
	MSG winLoop();

	//-- control general de interfaz en escala
	void SetDisplayIndex(uint8_t index, bool Fontload = true);
	uint8_t GetDisplayIndex();
	uint8_t GetDisplayIndex(const std::string text_name);
	float GetScreenX();
	float GetScreenY();
	ResolutionConfig* LoadCurrentConfig();
	//--

	HWND GethWnd();
	void SetInstance(HINSTANCE hInst);
	HINSTANCE GetInstance();
	void SetWndActive(bool bActive);
	bool CheckWndActive();
	void UpdateWndActive();
	void SetWndMode(bool bActive);
	bool CheckWndMode();
	void Check_State();
	void Change_State(bool bActive);
	bool CheckWndIconic();
	bool CheckPerformance();

	//-- funciones globales
	void SendWindowMessage(const char* format, bool destroyAfter, ...);
	void SendNowMessage(UINT Msg, WPARAM wParam, LPARAM lParam);
	void SendPostMessage(UINT Msg, WPARAM wParam, LPARAM lParam);
	static CWINHANDLE* Instance();
	static LONG FAR PASCAL WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	HWND hWnd;
	HINSTANCE hInstance;
	NOTIFYICONDATA nid;
	uint8_t wndIndex;
	bool WndMode;
	bool WndActive;
	bool WndIconic;
	float iWinWidth;
	float iWinHight;
	CSettings res_setting;
};

extern int g_iInactiveWarning;
extern BOOL g_bMinimizedEnabled;
extern float g_iMousePopPosition_x;
extern float g_iMousePopPosition_y;
extern CChatRoomSocketList* g_pChatRoomSocketList;

#ifdef IMPLEMENT_IMGUI_WIN32
extern void DestroyImGuiWindow();
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // IMPLEMENT_IMGUI_WIN32

#define gwinhandle			(CWINHANDLE::Instance())
