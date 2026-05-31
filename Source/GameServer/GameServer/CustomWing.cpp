#include "stdafx.h"
#include "Util.h"
#include "MemScript.h"
#include "ItemOption.h"
#include "CustomWing.h"

CCustomWing gCustomWing;

CCustomWing::CCustomWing() // OK
{
	this->Init();
}

CCustomWing::~CCustomWing() // OK
{
}

void CCustomWing::Init() // OK
{
	m_nBackByItem.clear();
}

void CCustomWing::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->Init();

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

			CUSTOM_WING_INFO info;

			int nType = lpMemScript->GetNumber();

			int nIndex = lpMemScript->GetAsNumber();

			info.ItemIndex = nIndex + (nType << 9);

			info.DefenseConstA = lpMemScript->GetAsNumber();

			info.IncDamageConstA = lpMemScript->GetAsNumber();

			info.IncDamageConstB = lpMemScript->GetAsNumber();

			info.DecDamageConstA = lpMemScript->GetAsNumber();

			info.DecDamageConstB = lpMemScript->GetAsNumber();

			for (size_t i = 0; i < 3; i++)
			{
				info.OptionIndex[i] = lpMemScript->GetAsNumber();

				info.OptionValue[i] = lpMemScript->GetAsNumber();
			}

			for (size_t i = 0; i < 4; i++)
			{
				info.NewOptionIndex[i] = lpMemScript->GetAsNumber();

				info.NewOptionValue[i] = lpMemScript->GetAsNumber();
			}

			lpMemScript->GetAsNumber();

			lpMemScript->GetAsString();

			this->Mng__Register(info.ItemIndex, info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomWing::Mng__Register(int Index, CUSTOM_WING_INFO info) // OK
{
	m_nBackByItem.insert(std::pair<int, CUSTOM_WING_INFO>(info.ItemIndex, info));
}

CUSTOM_WING_INFO* CCustomWing::findWings007(int ItemIndex) // OK
{
	std::map<int, CUSTOM_WING_INFO>::iterator it = m_nBackByItem.find(ItemIndex);

	if (it != m_nBackByItem.end())
	{
		return (&it->second);
	}

	return NULL;
}

bool CCustomWing::CheckCustomWingByItem(int ItemIndex) // OK
{
	if (this->findWings007(ItemIndex) != 0)
	{
		return 1;
	}

	return 0;
}
