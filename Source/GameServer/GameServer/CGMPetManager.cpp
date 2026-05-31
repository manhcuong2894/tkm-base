#include "StdAfx.h"
#include "User.h"
#include "Util.h"
#include "MemScript.h"
#include "ItemOption.h"
#include "ItemManager.h"
#include "CGMPetManager.h"

CGMPetManager::CGMPetManager()
{
}

CGMPetManager::~CGMPetManager()
{
	this->m_ItemOption.clear();
}

CGMPetManager* CGMPetManager::Instance()
{
	static CGMPetManager s_Instance;
	return &s_Instance;
}

bool CGMPetManager::LoadFiles(const std::string& filename)
{
	bool is_open = true;
	bool Success = false;

	this->m_ItemOption.clear();

	Success = this->LoadFileXml(filename + ".xml", is_open);

	if (!Success && !is_open)
	{
		Success = this->LoadFileTxt(filename + ".txt");
	}

	return Success;
}

bool CGMPetManager::LoadFileTxt(const std::string& filename)
{
	CMemScript* lpMemScript = new CMemScript;

	const char* lpszFileName = filename.c_str();

	if (lpMemScript == 0)
	{
		return false;
	}

	if (lpMemScript->SetBuffer(lpszFileName) == 0)
	{
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

			CUSTOM_PET_INFO info;

			int x = lpMemScript->GetNumber();

			int y = lpMemScript->GetAsNumber();

			info.ItemIndex = SafeGetItem(GET_ITEM(x, y));

			info.IncDamageRate = lpMemScript->GetAsNumber();

			info.DecDamageRate = lpMemScript->GetAsNumber();

			for (size_t i = 0; i < MAX_ADD_OPTION_INFO; i++)
			{
				info.Opt[i] = lpMemScript->GetAsNumber();

				info.Val[i] = lpMemScript->GetAsNumber();
			}

			this->m_ItemOption.insert(std::pair<int, CUSTOM_PET_INFO>(info.ItemIndex, info));
		}
	}
	catch (...)
	{
		printf(lpMemScript->GetLastError());
	}

	delete lpMemScript;

	return true;
}

bool CGMPetManager::LoadFileXml(const std::string& filename, bool& is_open)
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

	pugi::xml_node root = file.child("ItemOption");

	if (!root)
	{
		ErrorMessageBox("[ItemOption] ItemOption::LoadData() - Data Fail");
		return false;
	}

	for (pugi::xml_node child = root.child("Item"); child; child = child.next_sibling())
	{
		CUSTOM_PET_INFO info;

		int x = child.attribute("Type").as_int();

		int y = child.attribute("Index").as_int();

		info.ItemIndex = SafeGetItem(GET_ITEM(x, y));

		info.IncDamageRate = child.attribute("IncDamageRate").as_int();

		info.DecDamageRate = child.attribute("DecDamageRate").as_int();

		char attribute_name[100];

		for (size_t i = 0; i < MAX_ADD_OPTION_INFO; i++)
		{
			sprintf_s(attribute_name, "Opt%d", i + 1);
			info.Opt[i] = child.attribute(attribute_name).as_int();

			sprintf_s(attribute_name, "Val%d", i + 1);
			info.Val[i] = child.attribute(attribute_name).as_int();
		}

		this->m_ItemOption.insert(std::pair<int, CUSTOM_PET_INFO>(info.ItemIndex, info));
	}

	return true;
}

bool CGMPetManager::CheckPetByItem(int ItemIndex)
{
	std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_ItemOption.find(ItemIndex);

	return (it != this->m_ItemOption.end());
}

bool CGMPetManager::PetIsInmortal(int ItemIndex)
{
	std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_ItemOption.find(ItemIndex);

	if (it != this->m_ItemOption.end())
	{
		return (it->second.Inmortal != 0);
	}

	return false;
}

int CGMPetManager::GetItemPetIncDamageRate(int ItemIndex)
{
	std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_ItemOption.find(ItemIndex);

	if (it != this->m_ItemOption.end())
	{
		return it->second.IncDamageRate;
	}

	return 0;
}

int CGMPetManager::GetItemPetDecDamageRate(int ItemIndex)
{
	std::map<int, CUSTOM_PET_INFO>::iterator it = this->m_ItemOption.find(ItemIndex);

	if (it != this->m_ItemOption.end())
	{
		return it->second.DecDamageRate;
	}

	return 0;
}

void CGMPetManager::CalcItemCommonOption(LPOBJ lpObj, bool flag)
{
	int _inv[] = { EQUIPMENT_HELPER, EQUIPMENT_EAGLE, EQUIPMENT_MUUN, -1 };

	for (int i = 0; _inv[i] != -1; i++)
	{
		int n = _inv[i];

		CItem* pItem = &lpObj->Inventory[n];

		if (pItem->IsItem() != 0 && pItem->m_IsValidItem != 0 && pItem->m_Durability != 0)
		{
			this->InsertOption(lpObj, pItem, flag);
		}
	}
}

void CGMPetManager::InsertOption(LPOBJ lpObj, CItem* lpItem, bool flag)
{
	this->UpdateInfo(lpItem->m_Index);

	if (this->CurrentInfo != this->m_ItemOption.end())
	{
		for (size_t i = 0; i < MAX_ADD_OPTION_INFO; i++)
		{
			int index = this->GetIndex(i);

			int value = this->GetValue(i);

			if (index == 0 || value == 0)
				continue;

			gItemOption.AddOption(lpObj, index, value, lpItem, flag);
		}
	}
}

void CGMPetManager::UpdateInfo(int Index)
{
	this->CurrentInfo = this->m_ItemOption.find(Index);
}

int CGMPetManager::GetIndex(int index)
{
	if (this->CurrentInfo != this->m_ItemOption.end())
	{
		return this->CurrentInfo->second.Opt[index];
	}
	return 0;
}

int CGMPetManager::GetValue(int index)
{
	if (this->CurrentInfo != this->m_ItemOption.end())
	{
		return this->CurrentInfo->second.Val[index];
	}
	return 0;
}


