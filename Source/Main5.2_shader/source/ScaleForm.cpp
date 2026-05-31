#include "stdafx.h"
#include "ScaleForm.h"
#include "UIControls.h"
#include "Utilities/Log/muConsoleDebug.h"

#ifdef SHUTDOWN_SCALEFORM_INFO
PLACEMENT_NEW placementObj_new = NULL;
PLACEMENT_NEW placementMov_new = NULL;

SCALEFORM::SCALEFORM()
{
	hModule = NULL;
}

SCALEFORM::~SCALEFORM()
{
	Release();
}

void SCALEFORM::Release()
{
	if (hModule != NULL)
	{
		hldList.clear();

		void (*ExitProc)() = (void(*)())GetProcAddress(hModule, "runtime_kill");

		if (ExitProc) ExitProc();

		FreeLibrary(hModule);
	}
}

SCALEFORM* SCALEFORM::Instance()
{
	static SCALEFORM s_Instance;
	return &s_Instance;
}

void SCALEFORM::Linker()
{
	hModule = LoadLibrary("ScaleForm.dll");

	if (hModule != NULL)
	{
		void (*EntryProc)() = (void(*)())GetProcAddress(hModule, "EntryProc");

		if (EntryProc != 0)
		{
			placementMov_new = ((PLACEMENT_NEW)GetProcAddress(hModule, "InitializeMovieLoader"));

			placementObj_new = ((PLACEMENT_NEW)GetProcAddress(hModule, "InitializeContainerType"));

			EntryProc();
		}

		if (placementMov_new == NULL || placementObj_new == NULL)
		{
			FreeLibrary(hModule);
		}
	}
}

void SCALEFORM::UnLinker()
{
	Release();
}

void commandInternal(void* pClass, const char* pName, const char* pCommand)
{
}

void commandExternal(void* pClass, const char* pName, const void* gfxValue, UInt numArgs)
{
}

void SCALEFORM::LoadMovie()
{
	this->LoadMovie(0, "hudNotice.gfx", commandInternal, commandExternal, GFxMovieView::Align_TopLeft);

#if jdk_dev_chat_fx
	this->LoadMovie(1, "Chat.gfx", commandInternal, commandExternal, GFxMovieView::Align_TopLeft);
	this->LoadMovie(2, "Chat.gfx", commandInternal, commandExternal, GFxMovieView::Align_TopLeft);

	this->Invoke(1, "configUI", "");
	this->Invoke(2, "configUI", "");
#endif // jdk_dev_chat_fx
}

void SCALEFORM::LoadMovie(UInt index, const char* pfilename, FUNC_INTERNAL pCommand, FUNC_EXTERNAL pExternal, UInt align)
{
	if (placementMov_new)
	{
		auto datafx = std::make_shared<CONTAINER_HANDLE>();

		if (datafx)
		{
			datafx->Live = false;
			datafx->index = index;
			datafx->shared.AddMovie(pCommand, pExternal, pfilename, align);
			datafx->shared.OnCreateDevice(WindowWidth, WindowHeight);
			hldList.push_back(datafx);
		}
	}
}

void SCALEFORM::CreateObject(UInt index, SCALEFORMOBJECT* gfxValue)
{
	SCALEFORMHANDLE* shared = GetSharedHandleByIndex(index);

	if (shared)
	{
		shared->CreateObject(gfxValue);
	}
}

void SCALEFORM::CreateArray(UInt index, SCALEFORMOBJECT* gfxValue, UInt Size)
{
	SCALEFORMHANDLE* shared = GetSharedHandleByIndex(index);

	if (shared)
	{
		shared->CreateArray(gfxValue, Size);
	}
}

void SCALEFORM::Render(UInt index)
{
	SCALEFORMHANDLE* shared = GetSharedHandleByIndex(index);

	if (shared)
	{
		EndBitmap();
		shared->Render();
		BeginBitmap();
	}
}

void SCALEFORM::Update(UInt index)
{
	SCALEFORMHANDLE* shared = GetSharedHandleByIndex(index);

	if (shared)
	{
		shared->Update();
	}
}

void SCALEFORM::Invoke(UInt index, const char* pmethodName, const char* pargFmt, ...)
{
	SCALEFORMHANDLE* shared = GetSharedHandleByIndex(index);

	if (shared)
	{
		va_list args;
		va_start(args, pargFmt);
		shared->Invoke(pmethodName, pargFmt, args);
		va_end(args);
	}
}

void SCALEFORM::Invoke(UInt index, const char* pmethodName, SCALEFORMOBJECT* pargFmt)
{
	SCALEFORMHANDLE* shared = GetSharedHandleByIndex(index);

	if (shared)
	{
		shared->Invoke(pmethodName, pargFmt);
	}
}

void SCALEFORM::Invoke(UInt index, const char* pmethodName, SCALEFORMOBJECT* pargFmt, int Size)
{
	SCALEFORMHANDLE* shared = GetSharedHandleByIndex(index);

	if (shared)
	{
		shared->Invoke(pmethodName, pargFmt);
	}
}

bool SCALEFORM::CallBack(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	for (size_t i = 0; i < hldList.size(); i++)
	{
		if (hldList[i]->Live)
		{
			hldList[i]->shared.runtime_event(uMsg, wParam, lParam);
		}
	}

	return false;
}

SCALEFORMHANDLE* SCALEFORM::GetSharedHandleByIndex(int searchIndex)
{
	SCALEFORMHANDLE* shared = NULL;

	auto it = std::find_if(hldList.begin(), hldList.end(), [searchIndex](const std::shared_ptr<CONTAINER_HANDLE>& handle) {
		return handle->index == searchIndex;
		});

	if (it != hldList.end())
	{
		shared = &(*it)->shared;
	}

	return shared;
}

SCALEFORMOBJECT::SCALEFORMOBJECT()
{
	memset(inheritance, 0, sizeof(inheritance));
	if(placementObj_new)
		placementObj_new(this, 0x00);
}

SCALEFORMOBJECT::SCALEFORMOBJECT(UInt Type)
{
	memset(inheritance, 0, sizeof(inheritance));
	if (placementObj_new)
		placementObj_new(this, Type);
}

bool SCALEFORMOBJECT::AddMember(const char* name, bool to_member)
{
	if (placementObj_new)
		return (*(bool(__thiscall**)(void*, const char*, int))(*(DWORD*)this + 4))(this, name, to_member);

	return false;
}

bool SCALEFORMOBJECT::AddMember(const char* name, int to_member)
{
	if (placementObj_new)
		return (*(bool(__thiscall**)(void*, const char*, double))(*(DWORD*)this + 8))(this, name, static_cast<double>(to_member));
	return false;
}

bool SCALEFORMOBJECT::AddMember(const char* name, double to_member)
{
	if (placementObj_new)
		return (*(bool(__thiscall**)(void*, const char*, double))(*(DWORD*)this + 8))(this, name, to_member);
	return false;
}

bool SCALEFORMOBJECT::AddMember(const char* name, const char* to_member)
{
	if (placementObj_new)
		return (*(bool(__thiscall**)(void*, const char*, const char*))(*(DWORD*)this + 12))(this, name, to_member);
	return false;
}

bool SCALEFORMOBJECT::AddElement(UInt idx, void* to_element)
{
	if (placementObj_new)
		return (*(bool(__thiscall**)(void*, UInt, void*))(*(DWORD*)this + 16))(this, idx, to_element);
	return false;
}

bool SCALEFORMOBJECT::SetArraySize(UInt Size)
{
	if (placementObj_new)
		return (*(bool(__thiscall**)(void*, UInt))(*(DWORD*)this + 20))(this, Size);
	return false;
}

SCALEFORMHANDLE::SCALEFORMHANDLE()
{
	memset(this, 0, sizeof(SCALEFORMHANDLE));
	if (placementMov_new)
		placementMov_new(this, 0x01);
}

bool SCALEFORMHANDLE::AddMovie(FUNC_INTERNAL pCommand, FUNC_EXTERNAL pExternalCommand, const char* pfilename, UInt AlignType)
{
	if (placementMov_new)
		return (*(bool(__thiscall**)(void*, FUNC_INTERNAL, FUNC_EXTERNAL, const char*, UInt))(*(DWORD*)this + 4))(this, pCommand, pExternalCommand, pfilename, AlignType);
	return false;
}

bool SCALEFORMHANDLE::Update()
{
	if (placementMov_new)
		return (*(bool(__thiscall**)(void*))(*(DWORD*)this + 8))(this);
	return false;
}

bool SCALEFORMHANDLE::Render()
{
	if (placementMov_new)
		return (*(bool(__thiscall**)(void*))(*(DWORD*)this + 12))(this);
	return false;
}

void SCALEFORMHANDLE::CreateObject(SCALEFORMOBJECT* gfxValue)
{
	if (placementMov_new)
		(*(bool(__thiscall**)(void*, void*))(*(DWORD*)this + 16))(this, gfxValue);
}

void SCALEFORMHANDLE::CreateArray(SCALEFORMOBJECT* gfxValue, UInt sz)
{
	if (placementMov_new)
		(*(bool(__thiscall**)(void*, void*, UInt))(*(DWORD*)this + 20))(this, gfxValue, sz);
}

void SCALEFORMHANDLE::Invoke(const char* methodName, SCALEFORMOBJECT* gfxValue)
{
	if (placementMov_new)
		(*(void(__thiscall**)(void*, const char*, void*))(*(DWORD*)this + 28))(this, methodName, gfxValue);
}

void SCALEFORMHANDLE::Invoke(const char* methodName, const char* pargFmt, va_list args)
{
	if (placementMov_new)
		(*(void(__thiscall**)(void*, const char*, const char*, va_list))(*(DWORD*)this + 24))(this, methodName, pargFmt, args);
}

void SCALEFORMHANDLE::OnResetDevice(SInt bufw, SInt bufh)
{
	if (placementMov_new)
		(*(void(__thiscall**)(void*, SInt, SInt))(*(DWORD*)this + 32))(this, bufw, bufh);
}

bool SCALEFORMHANDLE::OnCreateDevice(SInt bufw, SInt bufh)
{
	if (placementMov_new)
		return (*(bool(__thiscall**)(void*, SInt, SInt))(*(DWORD*)this + 36))(this, bufw, bufh);
	return false;
}

void SCALEFORMHANDLE::runtime_event(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (placementMov_new)
		(*(void(__thiscall**)(void*, UInt, WPARAM, LPARAM))(*(DWORD*)this + 40))(this, uMsg, wParam, lParam);
}


extern int FontHeight;

void gfxNotice::addNoticeText(char* Text, DWORD Style)
{
	DWORD HexColor = RGBA(0xff, 0xff, 0xff, 0xff);

	switch (Style)
	{
	case 0:
		HexColor = RGBA(0xff, 0xc8, 0x32, 0xff);
		break;
	case 1:
		HexColor = RGBA(0xff, 0xff, 0xff, 0xff);
		break;
	case 2:
		HexColor = RGBA(0x32, 0xe2, 0x86, 0xff);
		break;
	}

	const char* HTML = SEASON3B::FontHTML(Text, HexColor, (int)(FontHeight-1), true, false, 3);
	gfxinit->Invoke(0, "addText", "%s", HTML);
}

void gfxNotice::addEventMapText(char* Text)
{
	const char* HTML = SEASON3B::FontHTML(Text, RGBA(0xff, 0xcc, 0x19u, 0xffu), 0, false, true);
	gfxinit->Invoke(0, "addEventMapText", "%s", HTML);
}

void gfxNotice::addLongMovementText(const char* Text, int Size, int YPos, int Duration)
{
	const char* textHTML = SEASON3B::FontHTML(Text, RGBA(0xff, 0xcc, 0x19u, 0xffu), Size, false, true);

	SCALEFORMOBJECT gfxvalue(6);

	gfxinit->CreateObject(0, &gfxvalue);
	gfxvalue.AddMember("Duration", (double)Duration);
	gfxvalue.AddMember("YPos", (double)(YPos / g_fScreenRate_y));
	gfxvalue.AddMember("Text", textHTML);
	gfxinit->Invoke(0, "addLongMovementText", &gfxvalue);
}

#if jdk_dev_chat_fx
void gfxChat::SetPosition(int x, int y)
{
	x *= g_fScreenRate_x;
	y *= g_fScreenRate_y;
	gfxinit->Invoke(1, "setPosition", "%d %d", x, y);
}

void gfxChat::SetPositionView(int x, int y)
{
	x *= g_fScreenRate_x;
	y *= g_fScreenRate_y;
	gfxinit->Invoke(2, "setPosition", "%d %d", x, y);
}

void gfxChat::AddChatInfo(int type, const char* name, const char* text)
{
	if (MAIN_SCENE == SceneFlag)
	{
		int index = 1;
		if (type == SEASON3B::MESSAGE_TYPE::TYPE_SYSTEM_MESSAGE)
		{
			index = 2;
		}

		SCALEFORMOBJECT gfxvalue(6);

		gfxinit->CreateObject(index, &gfxvalue);
		gfxvalue.AddMember("type", type);

		if (type != SEASON3B::MESSAGE_TYPE::TYPE_ERROR_MESSAGE
			&& type != SEASON3B::MESSAGE_TYPE::TYPE_SYSTEM_MESSAGE)
		{
			gfxvalue.AddMember("name", name);
		}

		gfxvalue.AddMember("msg", text);
		gfxinit->Invoke(index, "AddChatInfo", &gfxvalue);
	}
}

void gfxChat::ClearAllChatHistory()
{
	gfxinit->Invoke(1, "ClearAllChatHistory", "");
	gfxinit->Invoke(2, "ClearAllChatHistory", "");
}
#endif // jdk_dev_chat_fx

#endif // SHUTDOWN_SCALEFORM_INFO
