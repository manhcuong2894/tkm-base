#include "StdAfx.h"
#include "Util.h"
#include "MemScript.h"
#include "ItemManager.h"
#include "JSProtocol.h"
#include "CustomItemUseVip.h"

CCustomItemTicketVip::CCustomItemTicketVip()
{
}

CCustomItemTicketVip::~CCustomItemTicketVip()
{
	ticketList.clear();
}

CCustomItemTicketVip* CCustomItemTicketVip::Instance()
{
	static CCustomItemTicketVip s_Instance;
	return &s_Instance;
}

bool CCustomItemTicketVip::LoadFiles(const std::string& filename)
{
	bool is_open = true;
	bool Success = false;

	ticketList.clear();

	// Intentar cargar el archivo XML
	Success = this->LoadFileXml(filename + ".xml", is_open);

	// Si no se pudo cargar el XML, intenta cargar el TXT
	if (!Success && !is_open)
	{
		Success = this->LoadFileTxt(filename + ".txt");
	}

	return Success;
}

bool CCustomItemTicketVip::LoadFileTxt(const std::string& filename)
{
	CMemScript* lpMemScript = new CMemScript;

	const char* lpszFileName = filename.c_str();

	if (lpMemScript == 0)
	{
		//ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, lpszFileName);
		return false;
	}

	if (lpMemScript->SetBuffer(lpszFileName) == 0)
	{
		//ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return false;
	}

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if (strcmp("end", lpMemScript->GetString()) == 0)
			{
				break;
			}

			CUSTOM_ITEM_TICKET_VIP ticketvip;

			int section = lpMemScript->GetNumber();

			int index = lpMemScript->GetAsNumber();

			(&ticketvip)->Index = GET_ITEM(section, index);

			(&ticketvip)->Level = lpMemScript->GetAsNumber();

			(&ticketvip)->VipLevel = lpMemScript->GetAsNumber();

			(&ticketvip)->Days = lpMemScript->GetAsNumber();

			(&ticketvip)->textName = lpMemScript->GetAsString();

			ticketList.push_back(ticketvip);
		}
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}

	delete lpMemScript;

	return true;
}

bool CCustomItemTicketVip::LoadFileXml(const std::string& filename, bool& is_open)
{
	const char* lpszFileName = filename.c_str();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if (res.status != pugi::status_ok)
	{
		is_open = false;
		return false;
	}

	is_open = true;

	pugi::xml_node root = file.child("itemticketvip");

	if (!root)
	{
		ErrorMessageBox("[itemticketvip] itemticketvip::LoadData() - Data Fail");
		return false;
	}

	for (pugi::xml_node child = root.child("item"); child; child = child.next_sibling())
	{
		CUSTOM_ITEM_TICKET_VIP ticketvip;

		int section = child.attribute("section").as_int();

		int index = child.attribute("Index").as_int();

		(&ticketvip)->Index = GET_ITEM(section, index);

		(&ticketvip)->Level = child.attribute("Level").as_int();

		(&ticketvip)->VipLevel = child.attribute("VipLevel").as_int();

		(&ticketvip)->Days = child.attribute("Days").as_int();

		(&ticketvip)->textName = child.attribute("VipName").as_string();

		ticketList.push_back(ticketvip);
	}

	return true;
}

CUSTOM_ITEM_TICKET_VIP* CCustomItemTicketVip::GetInfo(int index, int level)
{
	auto it = std::find_if(ticketList.begin(), ticketList.end(),
		[index, level](const CUSTOM_ITEM_TICKET_VIP& res)
		{
			return res.Index == index && (res.Level == -1 || (res.Level != -1 && res.Level == level));
		});

	return (it != ticketList.end()) ? &(*it) : NULL;
}

bool CCustomItemTicketVip::ChangeTicket(LPOBJ lpObj, int ItemIndex, int level)
{
	CUSTOM_ITEM_TICKET_VIP* ticket = this->GetInfo(ItemIndex, level);

	if (ticket)
	{
		GJAccountLevelSaveSend(lpObj->Index, ticket->VipLevel, ticket->Days * 86400);

		GJAccountLevelSend(lpObj->Index);

		return true;
	}

	return false;
}
