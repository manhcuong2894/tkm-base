//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PersonalShopTitleImp.h"
#include "CSPetSystem.h"
#include "UIManager.h"
#include "zzzlodterrain.h"
#include "zzzAi.h"
#include "ZzzOpenglUtil.h"
#include "zzzInfomation.h"
#include "zzzBmd.h"
#include "zzzObject.h"
#include "zzztexture.h"
#include "zzzCharacter.h"
#include "zzzscene.h"
#include "zzzinfomation.h"
#include "zzzInterface.h"
#include "zzzinventory.h"
#include "wsclientinline.h"
#include "CSChaosCastle.h"
#include "CSQuest.h"
#include "GIPetManager.h"
#include "npcBreeder.h"
#include "MapManager.h"
#include "ZzzEffect.h"
#include "CharacterManager.h"

extern  int CheckX;
extern  int CheckY;
extern  int CheckSkill;
extern  bool SkillEnable;
static BYTE g_byTabBar = 0;
static DWORD g_iRenderItemIndexBackup = 0;
static DWORD g_iRenderEquippingBackup = 0;
static ITEM g_RenderItemInfoBackup;
static const BYTE g_byCommandNum = 4;

void giPetManager::InitPetManager(void)
{
	g_byTabBar = 0;
	gs_PetInfo.m_dwPetType = PET_TYPE_NONE;
}

void giPetManager::CreatePetDarkSpirit(CHARACTER* c)
{
	DeletePet(c);

	if (gMapManager->InChaosCastle() == true)
		return;

	CSPetSystem* pPet = new CSPetDarkSpirit(c);
	c->m_pPet = (void*)pPet;
}

void giPetManager::CreatePetDarkSpirit_Now(CHARACTER* c)
{
	if (c->Weapon[1].Type == MODEL_HELPER + 5)
	{
		DeletePet(c);
		CSPetSystem* pPet = new CSPetDarkSpirit(c);
		c->m_pPet = (void*)pPet;
	}
}

void giPetManager::MovePet(CHARACTER* c)
{
	if (c->m_pPet != NULL)
	{
		CSPetSystem* pPet = (CSPetSystem*)c->m_pPet;
		pPet->MovePet();
	}
}

void giPetManager::RenderPet(CHARACTER* c)
{
	OBJECT* o = &c->Object;
	if (c->m_pPet != NULL)
	{
		CSPetSystem* pPet = (CSPetSystem*)c->m_pPet;

		if (g_isCharacterBuff(o, eBuff_Cloaking))
		{
			pPet->RenderPet(10);
		}
		else
		{
			ITEM* pEquipmentItemSlot = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT];
			PET_INFO* pPetInfo = giPetManager::GetPetInfo(pEquipmentItemSlot);
			pPet->SetPetInfo(pPetInfo);
			pPet->RenderPet();
		}
	}
}

bool giPetManager::SelectPetCommand(void)
{
	if (gCharacterManager.GetBaseClass(Hero->Class) != CLASS_DARK_LORD)
		return false;
	if (HIBYTE(GetAsyncKeyState(VK_SHIFT)) == 128)
	{
		for (int i = 0; i < (AT_PET_COMMAND_END - AT_PET_COMMAND_DEFAULT); i++)
		{
			if (HIBYTE(GetAsyncKeyState('1' + i)))
			{
				Hero->CurrentSkill = AT_PET_COMMAND_DEFAULT + i;
				return true;
			}
		}
	}
	return false;
}

void giPetManager::MovePetCommand(CHARACTER* c)
{
	if (c->m_pPet != NULL)
	{
		int SkillCount = 0;
		int Width, Height, x, y;
		for (int i = AT_PET_COMMAND_DEFAULT; i < AT_PET_COMMAND_END; i++)
		{
			Width = 32; Height = 36; x = 320 - (AT_PET_COMMAND_END - AT_PET_COMMAND_DEFAULT) * Width / 2 + SkillCount * Width; y = 330;
			SkillCount++;

			if (MouseX >= x && MouseX < x + Width && MouseY >= y && MouseY < y + Height)
			{
				CheckSkill = i;
				CheckX = x + Width / 2;
				CheckY = y;
				MouseOnWindow = true;
				if (MouseLButtonPush)
				{
					MouseLButtonPush = false;
					Hero->CurrentSkill = i;
					SkillEnable = false;
					PlayBuffer(SOUND_CLICK01);
					MouseUpdateTime = 0;
					MouseUpdateTimeMax = 6;
				}
			}
		}
	}
}

bool giPetManager::SendPetCommand(CHARACTER* c, int Index)
{
	if (c->m_pPet != NULL)
	{
		if (MouseRButtonPush || MouseRButton)
		{
			if (Index >= AT_PET_COMMAND_DEFAULT && Index < AT_PET_COMMAND_END)
			{
				CSPetSystem* pPet = (CSPetSystem*)c->m_pPet;

				if (Index == AT_PET_COMMAND_TARGET)
				{
					if (CheckAttack())
					{
						CHARACTER* tc = gmCharacters->GetCharacter(SelectedCharacter);

						if (SelectedCharacter != -1 && (tc->Object.Kind == KIND_MONSTER || tc->Object.Kind == KIND_PLAYER))
						{
							SendRequestPetCommand(pPet->GetPetType(), Index - AT_PET_COMMAND_DEFAULT, tc->Key);
						}
					}
				}
				else
				{
					SendRequestPetCommand(pPet->GetPetType(), Index - AT_PET_COMMAND_DEFAULT, 0xffff);
				}
				MouseRButtonPop = false;
				MouseRButtonPush = false;
				MouseRButton = false;

				MouseRButtonPress = 0;
				return true;
			}
		}
	}
	return false;
}

void giPetManager::SetPetCommand(CHARACTER* c, int Key, BYTE Cmd)
{
	if (c->m_pPet != NULL)
	{
		CSPetSystem* pPet = (CSPetSystem*)c->m_pPet;
		pPet->SetCommand(Key, Cmd);
	}
}

void giPetManager::SetAttack(CHARACTER* c, int Key, int attackType)
{
	if (c->m_pPet != NULL)
	{
		CSPetSystem* pPet = (CSPetSystem*)c->m_pPet;
		pPet->SetAttack(Key, attackType);
	}
}

bool giPetManager::RenderPetCmdInfo(int sx, int sy, int Type)
{
	if (Type < AT_PET_COMMAND_DEFAULT || Type >= AT_PET_COMMAND_END) return false;

	int  TextNum = 0;
	int  SkipNum = 0;

	if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK_LORD)
	{
		int cmdType = Type - AT_PET_COMMAND_DEFAULT;

		TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = true;
		sprintf(TextList[TextNum], GlobalText[1219 + cmdType]); TextNum++; SkipNum++;

		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		sprintf(TextList[TextNum], "\n"); TextNum++; SkipNum++;
		sprintf(TextList[TextNum], "\n"); TextNum++; SkipNum++;
		switch (cmdType)
		{
		case PET_CMD_DEFAULT:
			sprintf(TextList[TextNum], GlobalText[1223]);
			TextNum++; SkipNum++; break;
		case PET_CMD_RANDOM:
			sprintf(TextList[TextNum], GlobalText[1224]);
			TextNum++; SkipNum++; break;
		case PET_CMD_OWNER:
			sprintf(TextList[TextNum], GlobalText[1225]);
			TextNum++; SkipNum++; break;
		case PET_CMD_TARGET:
			sprintf(TextList[TextNum], GlobalText[1226]);
			TextNum++; SkipNum++; break;
		}

		SIZE TextSize = { 0, 0 };
		g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), TextList[0], 1, &TextSize);
		int Height = (int)(((TextNum - SkipNum) * TextSize.cy + SkipNum * TextSize.cy / 2) / g_fScreenRate_y);
		sy -= Height;

		RenderTipTextList(sx, sy, TextNum, 0);
		return true;
	}
	return false;
}

void giPetManager::DeletePet(CHARACTER* c)
{
	if (c->m_pPet != NULL)
	{
		if (c->m_pPet != NULL)
		{
			int iObjectType = ((CSPetSystem*)c->m_pPet)->GetObjectType();
			TerminateOwnerEffectObject(iObjectType);
		}

		delete ((CSPetSystem*)c->m_pPet);
		c->m_pPet = NULL;
	}
}

void giPetManager::InitItemBackup(void)
{
	ZeroMemory(&g_RenderItemInfoBackup, sizeof(ITEM));
	gs_PetInfo.m_dwPetType = PET_TYPE_NONE;
}

bool giPetManager::RequestPetInfo(unsigned __int16 sx, unsigned __int16 sy, ITEM* pItem, DWORD RenderEquipping)
{
	DWORD itemIndex = MAKELONG(sx, sy);

	if (gs_PetInfo.m_dwPetType == PET_TYPE_NONE
		|| g_iRenderItemIndexBackup != itemIndex
		|| g_iRenderEquippingBackup != RenderEquipping
		|| g_RenderItemInfoBackup.Type != pItem->Type
		|| g_RenderItemInfoBackup.Level != pItem->Level)
	{
		g_iRenderItemIndexBackup = itemIndex;
		g_RenderItemInfoBackup.Type = pItem->Type;
		g_RenderItemInfoBackup.Level = pItem->Level;
		g_iRenderEquippingBackup = RenderEquipping;

		BYTE PetType = PET_TYPE_DARK_SPIRIT;

		if (pItem->Type == ITEM_HELPER + 4 || pItem->Kind3 == ItemKind3::PET_GAIN_EXPERIENCE)
		{
			PetType = PET_TYPE_DARK_HORSE;
		}

		int iInvenType = 0;
		int iSlotIndex = -1;

		if (RenderEquipping == 0)
		{
			iSlotIndex = g_pMyInventory->GetPointedItemIndex();

			if (iSlotIndex == -1)
			{
				iSlotIndex = g_pMyShopInventory->GetPointedItemIndex();

				if (iSlotIndex == -1)
				{
					iSlotIndex = g_pMixInventory->GetPointedItemIndex();

					if (iSlotIndex == -1)
					{
						iSlotIndex = g_pNPCShop->GetPointedItemIndex();

						if (iSlotIndex == -1)
						{
							iSlotIndex = g_pStorageInventory->GetPointedItemIndex();

							if (iSlotIndex == -1)
							{
								iSlotIndex = g_pTrade->GetPointedItemIndexMyInven();

								if (iSlotIndex == -1)
								{
									iSlotIndex = g_pPurchaseShopInventory->GetPointedItemIndex();

									if (iSlotIndex == -1)
									{
										iSlotIndex = g_pTrade->GetPointedItemIndexYourInven();

										if (iSlotIndex != -1)
										{
											iInvenType = 3;
										}
									}
									else
									{
										iInvenType = 5;
									}
								}
								else
								{
									iInvenType = 2;
								}
							}
							else
							{
								iInvenType = 1;
							}
						}
						else
						{
							iInvenType = 6;
						}
					}
					else
					{
						iInvenType = 4;
					}
				}
				else
				{
					iInvenType = 0;
				}
			}
			else
			{
				iInvenType = 0;
			}
		}
		else
		{
			iSlotIndex = pItem->lineal_pos;
		}
		SendRequestPetInfo(PetType, iInvenType, iSlotIndex);
	}

	return false;
}

void giPetManager::SetPetInfo(BYTE InvType, BYTE InvPos, PET_INFO* pPetinfo)
{
	CalcPetInfo(pPetinfo);

	if ((!InvType || InvType == 254 || InvType == 255) && (InvPos == EQUIPMENT_HELPER || InvPos == EQUIPMENT_WEAPON_LEFT))
	{
		PET_INFO* pHeroPetInfo = Hero->GetEquipedPetInfo(pPetinfo->m_dwPetType);
		memcpy(pHeroPetInfo, pPetinfo, sizeof(PET_INFO));

		if (pPetinfo->m_dwPetType == PET_TYPE_DARK_HORSE)
		{
			if (InvType == 254 || InvType == 255)
				Hero->Object.ExtState = InvType + 3;

			SetPetItemConvert(&CharacterMachine->Equipment[EQUIPMENT_HELPER], pHeroPetInfo);
		}
		else
		{
			if (!Hero->m_pPet)
				return;

			((CSPetSystem*)Hero->m_pPet)->SetPetInfo(pHeroPetInfo);

			if (InvType == 254)
			{
				((CSPetSystem*)Hero->m_pPet)->Eff_LevelUp();
			}
			else if (InvType == 255)
			{
				((CSPetSystem*)Hero->m_pPet)->Eff_LevelDown();
			}
		}
		CharacterMachine->CalculateAll();
	}

	memcpy(&gs_PetInfo, pPetinfo, sizeof(PET_INFO));
}

PET_INFO* giPetManager::GetPetInfo(ITEM* pItem)
{
	if (pItem == &CharacterMachine->Equipment[EQUIPMENT_HELPER])
	{
		return Hero->GetEquipedPetInfo(PET_TYPE_DARK_HORSE);
	}
	else if (pItem == &CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT])
	{
		return Hero->GetEquipedPetInfo(PET_TYPE_DARK_SPIRIT);
	}
	return &gs_PetInfo;
}

void giPetManager::CalcPetInfo(PET_INFO* pPetInfo)
{
	int Charisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;
	int Strength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;

	int Level = pPetInfo->m_wLevel + 1;

	switch (pPetInfo->m_dwPetType)
	{
	case PET_TYPE_DARK_SPIRIT:
		pPetInfo->m_dwExp2 = ((10 + Level) * Level * Level * Level * 100);
		pPetInfo->m_wDamageMin = (180 + (pPetInfo->m_wLevel * 15) + (Charisma / 8));
		pPetInfo->m_wDamageMax = (200 + (pPetInfo->m_wLevel * 15) + (Charisma / 4));
		pPetInfo->m_wAttackSpeed = (20 + (pPetInfo->m_wLevel * 4 / 5) + (Charisma / 50));
		pPetInfo->m_wAttackSuccess = (1000 + pPetInfo->m_wLevel) + (pPetInfo->m_wLevel * 15);
		break;
	case PET_TYPE_DARK_HORSE:
		pPetInfo->m_dwExp2 = ((10 + Level) * Level * Level * Level * 100);
		pPetInfo->m_wDamageMin = (Strength / 10) + (Charisma / 10) + (pPetInfo->m_wLevel * 5);
		pPetInfo->m_wDamageMax = pPetInfo->m_wDamageMin + (pPetInfo->m_wDamageMin / 2);
		pPetInfo->m_wAttackSpeed = (20 + (pPetInfo->m_wLevel * 4 / 5) + (Charisma / 50));
		pPetInfo->m_wAttackSuccess = (1000 + pPetInfo->m_wLevel) + (pPetInfo->m_wLevel * 15);
		break;
	}
}

void giPetManager::SetPetItemConvert(ITEM* pItem, PET_INFO* pPetInfo)
{
	if (pItem->Type == ITEM_HELPER + 4)
	{
		int Index = -1;

		for (int i = 0; i < pItem->SpecialNum; ++i)
		{
			if (pItem->Special[i] == AT_SET_OPTION_IMPROVE_DEFENCE)
			{
				Index = i;
				break;
			}
		}

		if (Index == -1)
		{
			if (pItem->SpecialNum < MAX_ITEM_SPECIAL)
			{
				pItem->SpecialValue[pItem->SpecialNum] = CharacterAttribute->Dexterity / 20 + 2 * pPetInfo->m_wLevel + 5;
				pItem->Special[pItem->SpecialNum++] = AT_SET_OPTION_IMPROVE_DEFENCE;
			}
		}
		else
		{
			pItem->SpecialValue[Index] = CharacterAttribute->Dexterity / 20 + 2 * pPetInfo->m_wLevel + 5;
			pItem->Special[Index] = AT_SET_OPTION_IMPROVE_DEFENCE;
		}
	}
}

DWORD giPetManager::GetPetItemValue(PET_INFO* pPetInfo)
{
	DWORD dwGold = 0;

	if (pPetInfo->m_dwPetType == -1)
		return dwGold;

	switch (pPetInfo->m_dwPetType)
	{
	case PET_TYPE_DARK_HORSE:
		dwGold = pPetInfo->m_wLevel * 2000000;
		break;

	case PET_TYPE_DARK_SPIRIT:
		dwGold = pPetInfo->m_wLevel * 1000000;
		break;
	}

	return dwGold;
}

bool giPetManager::RenderPetItemInfo(int sx, int sy, ITEM* pItem, int iInvenType)
{
	PET_INFO* pPetInfo = GetPetInfo(pItem);

	if (pPetInfo->m_dwPetType == PET_TYPE_NONE)
		return false;

	int TextNum = 0;
	int SkipNum = 0;
	int RequireLevel = 0;
	int RequireCharisma = 0;

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_NPCSHOP))
	{
		char  Text[100];
		DWORD Gold = (GetPetItemValue(&giPetManager::gs_PetInfo) / 3);
		Gold = Gold / 100 * 100;

		ConvertGold(Gold, Text);
		sprintf(TextList[TextNum], GlobalText[63], Text);

		TextListColor[TextNum] = TEXT_COLOR_WHITE;
		TextBold[TextNum] = true;
		TextNum++;
		sprintf(TextList[TextNum], "\n"); TextNum++; SkipNum++;
	}
	else if ((iInvenType == SEASON3B::TOOLTIP_TYPE_MY_SHOP || iInvenType == SEASON3B::TOOLTIP_TYPE_PURCHASE_SHOP) && !IsPersonalShopBan(pItem))
	{
		PERSONAL_ITEM_PRICE_TABLE price_table;

		int price = 0;
		char Text[100] = { 0, };
		int indexInv = pItem->x + (COL_PERSONALSHOP_INVEN * pItem->y) + MAX_MY_INVENTORY_INDEX;

		if (GetPersonalItemPrice(indexInv, price_table, g_IsPurchaseShop))
		{
			DWORD gold = 0;
			price = price_table.Price;

			if (price > 0)
			{
				if (price_table.PersonalType == 1)
				{
					ConvertGold(price, Text);
					sprintf(TextList[TextNum], TextGlobal[55], Text);
					gold = CharacterMachine->Coin1;
				}
				else if (price_table.PersonalType == 2)
				{
					ConvertGold(price, Text);
					sprintf(TextList[TextNum], TextGlobal[56], Text);
					gold = CharacterMachine->Coin2;
				}
				else if (price_table.PersonalType == 3)
				{
					ConvertGold(price, Text);
					sprintf(TextList[TextNum], TextGlobal[57], Text);
					gold = CharacterMachine->Coin3;
				}
				else
				{
					ConvertGold(price, Text);
					sprintf(TextList[TextNum], TextGlobal[49], Text);
					gold = CharacterMachine->Gold;
				}

				if (price >= 10000000)
					TextListColor[TextNum] = TEXT_COLOR_RED;
				else if (price >= 1000000)
					TextListColor[TextNum] = TEXT_COLOR_YELLOW;
				else if (price >= 100000)
					TextListColor[TextNum] = TEXT_COLOR_GREEN;
				else
					TextListColor[TextNum] = TEXT_COLOR_WHITE;
				TextBold[TextNum++] = true;

				if ((int)gold < price && g_IsPurchaseShop == PSHOPWNDTYPE_PURCHASE)
				{
					TextListColor[TextNum] = TEXT_COLOR_RED;
					TextBold[TextNum] = true;
					sprintf(TextList[TextNum++], GlobalText[423]);
					sprintf(TextList[TextNum++], "\n");
					SkipNum++;
				}
			}

			price = price_table.JewelOfBless;

			if (price > 0)
			{
				ConvertGold(price, Text);
				sprintf(TextList[TextNum], TextGlobal[61], Text);
				TextListColor[TextNum] = 18;
				TextBold[TextNum++] = true;
			}

			price = price_table.JewelOfSoul;

			if (price > 0)
			{
				ConvertGold(price, Text);
				sprintf(TextList[TextNum], TextGlobal[62], Text);
				TextListColor[TextNum] = 18;
				TextBold[TextNum++] = true;
			}

			price = price_table.JewelOfChaos;

			if (price > 0)
			{
				ConvertGold(price, Text);
				sprintf(TextList[TextNum], TextGlobal[63], Text);
				TextListColor[TextNum] = 18;
				TextBold[TextNum++] = true;
			}
			sprintf(TextList[TextNum++], "\n");
			SkipNum++;
		}
		else if (g_IsPurchaseShop == PSHOPWNDTYPE_SALE)
		{
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = true;
			sprintf(TextList[TextNum++], GlobalText[1101]);
			sprintf(TextList[TextNum++], "\n");
			SkipNum++;
		}
	}

	TextListColor[TextNum] = TEXT_COLOR_BLUE;
	TextBold[TextNum] = true;

	if (pItem->Type == ITEM_HELPER + 4)
	{
		RequireLevel = (218 + (pPetInfo->m_wLevel * 2));
		RequireCharisma = 0;
		sprintf(TextList[TextNum++], GlobalText[1187]);
		SkipNum++;
	}
	else if (pItem->Type == ITEM_HELPER + 5)
	{
		RequireCharisma = (185 + (pPetInfo->m_wLevel * 15));
		sprintf(TextList[TextNum++], GlobalText[1214]);
		SkipNum++;
	}

	TextListColor[TextNum] = TEXT_COLOR_WHITE;
	sprintf(TextList[TextNum++], "\n");
	SkipNum++;
	sprintf(TextList[TextNum++], "\n");
	SkipNum++;
	sprintf(TextList[TextNum++], GlobalText[201], pPetInfo->m_dwExp1, pPetInfo->m_dwExp2);
	SkipNum++;
	sprintf(TextList[TextNum++], "%s : %d", GlobalText[368], pPetInfo->m_wLevel);
	SkipNum++;

	if (pItem->Type == ITEM_HELPER + 5)
	{
#ifdef SHUTDOWN_CHARACTER_INFO
		DWORD _DamageMin = pPetInfo->m_wDamageMin;
		DWORD _DamageMax = pPetInfo->m_wDamageMax;
		CharacterAttribute->GetDarkSpiritDamage(&_DamageMin, &_DamageMax);
		sprintf(TextList[TextNum++], GlobalText[203], _DamageMin, _DamageMax, CharacterAttribute->Work_DarkSpiritAttackSuccessRate);
#else
		sprintf(TextList[TextNum++], GlobalText[203], pPetInfo->m_wDamageMin, pPetInfo->m_wDamageMax, pPetInfo->m_wAttackSuccess);
#endif // SHUTDOWN_CHARACTER_INFO

#ifdef SHUTDOWN_CHARACTER_INFO
		sprintf(TextList[TextNum++], GlobalText[64], CharacterAttribute->Work_DarkSpiritAttackSpeed);
#else
		sprintf(TextList[TextNum++], GlobalText[64], pPetInfo->m_wAttackSpeed);
#endif // SHUTDOWN_CHARACTER_INFO
		;
		SkipNum++;
	}
	sprintf(TextList[TextNum], GlobalText[70], pPetInfo->m_wLife); TextNum++; SkipNum++;

	if (pItem->Type == ITEM_HELPER + 4)
	{
		sprintf(TextList[TextNum], GlobalText[76], RequireLevel);

		if (CharacterAttribute->Level < RequireLevel)
		{
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			TextNum++;
			sprintf(TextList[TextNum], GlobalText[74], RequireLevel - CharacterAttribute->Level);
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			TextNum++;
		}
		else
		{
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			TextBold[TextNum] = false;
			TextNum++;
		}
	}
	else if (pItem->Type == ITEM_HELPER + 5)
	{
		sprintf(TextList[TextNum], GlobalText[698], RequireCharisma);

		WORD Charisma;
		Charisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;

		if (Charisma < RequireCharisma)
		{
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			TextNum++;
			sprintf(TextList[TextNum], GlobalText[74], RequireCharisma - Charisma);
			TextListColor[TextNum] = TEXT_COLOR_RED;
			TextBold[TextNum] = false;
			TextNum++;
		}
		else
		{
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			TextBold[TextNum] = false;
			TextNum++;
		}
	}

	sprintf(TextList[TextNum++], "\n");
	SkipNum++;

	if (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK_LORD)
		TextListColor[TextNum] = TEXT_COLOR_WHITE;
	else
		TextListColor[TextNum] = TEXT_COLOR_DARKRED;

	sprintf(TextList[TextNum], GlobalText[61], GlobalText[24]);
	TextNum++; SkipNum++;

	SetPetItemConvert(pItem, &gs_PetInfo);

	for (int i = 0; i < pItem->SpecialNum; ++i)
	{
		GetSpecialOptionText(pItem->Type, TextList[TextNum], pItem->Special[i], pItem->SpecialValue[i], 0);
		TextListColor[TextNum] = TEXT_COLOR_BLUE;
		TextBold[TextNum] = false; TextNum++; SkipNum++;
	}

	if (pItem->Type == ITEM_HELPER + 4)
	{
		sprintf(TextList[TextNum], GlobalText[744], (30 + pPetInfo->m_wLevel) / 2);
		TextListColor[TextNum] = TEXT_COLOR_BLUE; TextNum++; SkipNum++;

		sprintf(TextList[TextNum], GlobalText[1188], 2);
		TextListColor[TextNum] = TEXT_COLOR_BLUE; TextBold[TextNum] = false; TextNum++;
	}

	SIZE TextSize = { 0, 0 };
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), TextList[0], 1, &TextSize);
	int Height = (int)(((TextNum - SkipNum) * TextSize.cy + SkipNum * TextSize.cy / 2) / g_fScreenRate_y);

	if (sy - Height >= 0)
		sy -= Height;

	bool isrendertooltip = true;

	if (isrendertooltip)
	{
		RenderTipTextList(sx, sy, TextNum, 0);
	}
	return true;
}
