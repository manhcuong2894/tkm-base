#pragma once
#include "stdafx.h"
#include "wsclientinline.h"
#include "StreamPacketEngine.h"

PBMSG_ENCRYPTED bc;
PWMSG_ENCRYPTED wc;
uint8_t byBuffer[MAX_SPE_BUFFERSIZE_];

bool FindText2(char* Text, char* Token, bool First)
{
	int LengthToken = (int)strlen(Token);
	int Length = (int)strlen(Text) - LengthToken;
	if (First)
		Length = 0;
	if (Length < 0)
		return false;
	for (int i = 0; i <= Length; i++)
	{
		bool Success = true;
		for (int j = 0; j < LengthToken; j++)
		{
			if (Text[i + j] != Token[j])
			{
				Success = false;
				break;
			}
		}
		if (Success)
			return true;
	}
	return false;
}


int SendPacket(char* buf, int len, BOOL bEncrypt, BOOL bForceC4)
{
	if (!bEncrypt)
	{
		return (g_pSocketClient->sSend(buf, len));
	}

	memset(byBuffer, 0, sizeof(byBuffer));
	memcpy(byBuffer, buf, len);
	byBuffer[len] = rand() % 256;

	int iSkip = (byBuffer[0] == 0xC1) ? 2 : 3;

	byBuffer[iSkip - 1] = g_byPacketSerialSend++;
	--iSkip;


	int iSize = g_SimpleModulusCS.Encrypt(NULL, byBuffer + iSkip, len - iSkip);

	if (iSize < 256 && bForceC4 == FALSE)
	{
		int iLength = iSize + 2;

		bc.Code = 0xC3;
		bc.Size = iLength;

		g_SimpleModulusCS.Encrypt(bc.byBuffer, byBuffer + iSkip, len - iSkip);
		assert(iSize < 256);

		return (g_pSocketClient->sSend((char*)&bc, iLength));
	}
	else
	{
		int iLength = iSize + 3;

		wc.Code = 0xC4;
		wc.SizeL = iLength % 256;
		wc.SizeH = iLength / 256;

		g_SimpleModulusCS.Encrypt(wc.byBuffer, byBuffer + iSkip, len - iSkip);

		assert(iSize <= MAX_SPE_BUFFERSIZE_);
		return (g_pSocketClient->sSend((char*)&wc, iLength));
	}
}

void SendRequestServerHWID()
{
	char ComputerHardwareId[36] = { 0, };

	create_hwid_system(ComputerHardwareId);

	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF4);
	uint8_t byData = (uint8_t)0x04;
	spe.AddData(&byData, 1, FALSE);
	spe.AddData(ComputerHardwareId, sizeof(ComputerHardwareId), FALSE);
	spe.Send();
}

void SendRequestServerList()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF4);
	uint8_t byData = (uint8_t)0x06;
	spe.AddData(&byData, 1, FALSE);
	spe.Send();
}

void SendRequestServerUpdate(int version)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF4);
	uint8_t byData = (uint8_t)0x08;
	spe.AddData(&byData, 1, FALSE);
	uint16_t Index = (uint16_t)(version);
	spe.AddData(&Index, 2, FALSE);
	spe.Send();
}

void SendRequestServerAddress(int p_Index)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF4);
	uint8_t byData = (uint8_t)0x03;
	spe.AddData(&byData, 1, FALSE);
	uint16_t Index = (uint16_t)(p_Index);
	spe.AddData(&Index, 2, FALSE);
	spe.Send();

	g_pChatListBox->AddText("", GlobalText[470], SEASON3B::TYPE_SYSTEM_MESSAGE);
	g_pChatListBox->AddText("", GlobalText[471], SEASON3B::TYPE_SYSTEM_MESSAGE);
}

void SendRequestServerOff(uint8_t State)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x20);
	spe << (uint8_t)State;
	spe.Send();
}

void SendCheck(void)
{
	if (!g_bGameServerConnected)
	{
		return;
	}
	g_ConsoleDebug->Write(MCD_SEND, "SendCheck");

	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x0E);
	uint32_t dwTick = GetTickCount();
	spe.AddNullData(1);
	spe << dwTick;
	spe << (uint32_t)(CharacterAttribute->AttackSpeed) << (uint32_t)(CharacterAttribute->MagicSpeed);
	spe.Send(TRUE);

	if (!First)
	{
		First = true;
		FirstTime = dwTick;
	}
}

void SendCheckSum(uint32_t dwCheckSum)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x03);
	spe.AddNullData(1);
	spe << (uint32_t)(dwCheckSum);
	spe.Send(TRUE);
}

void SendHackingChecked(uint8_t byType, uint8_t byParam)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF1);
	spe << (uint8_t)0x03 << (uint8_t)(byType) << (uint8_t)(byParam);
	spe.Send(TRUE);
}

void SendRequestLogIn(const char* p_lpszID, const char* p_lpszPassword)
{
	char lpszID[MAX_ID_SIZE + 1];
	char lpszPass[MAX_PASSWORD_SIZE + 1];
	ZeroMemory(lpszID, MAX_ID_SIZE + 1);
	ZeroMemory(lpszPass, MAX_PASSWORD_SIZE + 1);

	strncpy(lpszID, p_lpszID, MAX_ID_SIZE);
	strncpy(lpszPass, p_lpszPassword, MAX_PASSWORD_SIZE);
	BuxConvert((uint8_t*)lpszID, MAX_ID_SIZE);
	BuxConvert((uint8_t*)lpszPass, MAX_PASSWORD_SIZE);

	char ComputerHardwareId[36] = { 0, };
	create_hwid_system(ComputerHardwareId);
	BuxConvert((uint8_t*)ComputerHardwareId, 36);

	LogIn = 1;
	strcpy(LogInID, (p_lpszID));
	CurrentProtocolState = REQUEST_LOG_IN;
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF1);
	spe << (uint8_t)0x01;
	spe.AddData(lpszID, MAX_ID_SIZE);
	spe.AddData(lpszPass, MAX_PASSWORD_SIZE);
	spe.AddData(ComputerHardwareId, sizeof(ComputerHardwareId));

	spe << GetTickCount();
	for (int i = 0; i < SIZE_PROTOCOLVERSION; i++)
		spe << (uint8_t)(Version[i] - (i + 1));
	for (int i = 0; i < SIZE_PROTOCOLSERIAL; i++)
		spe << Serial[i];
	spe.Send(TRUE);

	g_pChatListBox->AddText("", GlobalText[472], SEASON3B::TYPE_SYSTEM_MESSAGE);
	g_pChatListBox->AddText("", GlobalText[473], SEASON3B::TYPE_SYSTEM_MESSAGE);
}

void SendRequestLogOut(int Flag)
{
	LogOut = true;
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF1);
	spe << (uint8_t)0x02 << (uint8_t)Flag;
	spe.Send(TRUE);

	g_ConsoleDebug->Write(MCD_SEND, "0xF1 [SendRequestLogOut]");
}

void SendRequestCharactersList(uint8_t byLanguage)
{
	CurrentProtocolState = REQUEST_CHARACTERS_LIST;
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)0x00;
	spe << (uint8_t)byLanguage;
	spe.Send();
}

void SendRequestCreateCharacter(char* p_ID, uint8_t p_Class, uint8_t p_Skin)
{
	CurrentProtocolState = REQUEST_CREATE_CHARACTER;
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)0x01;

	spe.AddData((p_ID), strlen(p_ID));
	spe.AddNullData(MAX_ID_SIZE - strlen(p_ID));

	spe << (uint8_t)(((p_Class) << 4) + (p_Skin));
	spe.Send();
}

void SendRequestDeleteCharacter(char* p_ID, char* p_Resident)
{
	CurrentProtocolState = REQUEST_DELETE_CHARACTER;
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)0x02;
	spe.AddData((p_ID), strlen(p_ID));
	spe.AddNullData(MAX_ID_SIZE - strlen(p_ID));
	spe.AddData((p_Resident), 20);
	spe.Send();
}

void SendRequestJoinMapServer(char* p_ID)
{
	CurrentProtocolState = REQUEST_JOIN_MAP_SERVER;
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)0x03;
	spe.AddData((p_ID), strlen((p_ID)));
	spe.AddNullData(MAX_ID_SIZE - strlen((p_ID)));
	spe.Send();
}

void SendRequestFinishLoading()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)0x12;
	spe.Send();

	g_dwLatestZoneMoving = GetTickCount();
	g_bWhileMovingZone = FALSE;
}

void SendChat(const char* Text)
{
	if (ChatTime > (int32_t)standlimit(50))
	{
		return;
	}
	else if (ChatTime > 0)
	{
		if (strcmp(ChatText, Text) == NULL)
			return;
	}

	strcpy(ChatText, Text);
	ChatTime = (int32_t)standlimit(70);

	if (FindText2(Hero->ID, (char*)"webzen"))
	{
		BOOL bReturn = TRUE;
		char* lspzCommand[2] = { "/", "!" };

		for (int i = 0; i < sizeof(lspzCommand) / sizeof(char*); i++)
		{
			if (0 == strncmp(Text, lspzCommand[i], strlen(lspzCommand[i])))
			{
				bReturn = FALSE;
				break;
			}
		}

		if (bReturn)
		{
			return;
		}
	}

	if (Hero->Dead > 0)
	{
		if (strlen(GlobalText[260]) > 0 && !strncmp(Text, GlobalText[260], strlen(GlobalText[260])))
		{
			return;
		}
	}
	else if (Text[0] == '/')
	{
		if (strlen(GlobalText[264]) > 0 && !strncmp(Text, GlobalText[264], strlen(GlobalText[264])))
		{
			g_pChatInputBox->SetBlockWhisper(true);
			g_pChatListBox->AddText("", GlobalText[267], SEASON3B::TYPE_SYSTEM_MESSAGE);
			return;
		}
		else if (strlen(GlobalText[265]) && !strncmp(Text, GlobalText[265], strlen(GlobalText[265])))
		{
			g_pChatInputBox->SetBlockWhisper(false);
			g_pChatListBox->AddText("", GlobalText[268], SEASON3B::TYPE_SYSTEM_MESSAGE);
			return;
		}
	}

	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x00);
	spe.AddData(Hero->ID, MAX_ID_SIZE);
	spe.AddData((void*)Text, (uint16_t)min(strlen(Text) + 1, MAX_CHAT_SIZE));
	spe.Send();
}

void SendChatWhisper(char* p_TargetID, char* p_Text)
{
	if (!FindText2(Hero->ID, "webzen"))
	{
		CStreamPacketEngine spe;
		spe.Init(0xC1, 0x02);
		spe.AddData((p_TargetID), MAX_ID_SIZE);
		spe.AddData((p_Text), min(strlen(p_Text) + 1, MAX_CHAT_SIZE));
		spe.Send();

		memcpy(ChatWhisperID, (p_TargetID), MAX_ID_SIZE);
		ChatWhisperID[MAX_ID_SIZE] = NULL;
	}
}

void SendPosition(int p_x, int p_y)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, PACKET_POSITION);
	spe << (uint8_t)(p_x) << (uint8_t)(p_y);
	spe.Send();
}

void SendCharacterMove(uint16_t Key, float Angle, uint8_t PathNum, uint8_t* PathX, uint8_t* PathY, uint8_t TargetX, uint8_t TargetY)
{
	if (PathNum < 1)
		return;

	if (PathNum >= MAX_PATH_FIND)
	{
		PathNum = MAX_PATH_FIND - 1;
	}

	CStreamPacketEngine spe;
	spe.Init(0xC1, PACKET_MOVE);
	spe << PathX[0] << PathY[0];
	uint8_t Path[8];
	ZeroMemory(Path, 8);
	uint8_t Dir = 0;
	for (int i = 1; i < PathNum; i++)
	{
		Dir = 0;
		for (int j = 0; j < 8; j++)
		{
			if (DirTable[j * 2] == (PathX[i] - PathX[i - 1]) && DirTable[j * 2 + 1] == (PathY[i] - PathY[i - 1]))
			{
				Dir = j;
				break;
			}
		}

		if (i % 2 == 1)
		{
			Path[(i + 1) / 2] = Dir << 4;
		}
		else
		{
			Path[(i + 1) / 2] += Dir;
		}
	}
	if (PathNum == 1)
	{
		Path[0] = ((uint8_t)((Angle + 22.5f) / 360.f * 8.f + 1.f) % 8) << 4;
	}
	else
	{
		for (int j = 0; j < 8; j++)
		{
			if (DirTable[j * 2] == (TargetX - PathX[PathNum - 1]) && DirTable[j * 2 + 1] == (TargetY - PathY[PathNum - 1]))
			{
				Dir = j;
				break;
			}
		}
		Path[0] = Dir << 4;
	}
	Path[0] += (uint8_t)(PathNum - 1);
	spe.AddData(Path, 1 + (PathNum) / 2);
	spe.Send();
}

void SendRequestAction(uint8_t p_Action, uint8_t p_Angle)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x18);
	spe << (uint8_t)(p_Angle) << (uint8_t)(p_Action);
	spe.Send();
}

void SendRequestAttack(int p_Key, uint8_t p_Dir)
{
	if (!FindText2(Hero->ID, "webzen"))
	{
		CStreamPacketEngine spe;
		spe.Init(0xC1, PACKET_ATTACK);
		spe << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff) << (uint8_t)AT_ATTACK1 << (uint8_t)(p_Dir);
		spe.Send();
	}
}

void SendRequestMagic(int p_Type, int p_Key)
{
	if (!FindText2(Hero->ID, "webzen"))
	{
		if (gSkillManager.IsMultiTargetSkill(p_Type) == true || (abs((int)(GetTickCount() - g_dwLatestMagicTick)) > 200))
		{
			g_dwLatestMagicTick = GetTickCount();
			CStreamPacketEngine spe;
			uint16_t Type = (uint16_t)p_Type;
			spe.Init(0xC1, 0x19);
			spe << (uint8_t)(HIBYTE(Type)) << (uint8_t)(LOBYTE(Type)) << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff);
			spe.Send(TRUE);
		}
	}
}

void SendRequestCancelMagic(int p_Type, int p_Key)\
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x1B);
	uint16_t Type = (uint16_t)p_Type;
	spe << (uint8_t)(HIBYTE(Type)) << (uint8_t)(LOBYTE(Type)) << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff);
	spe.Send(TRUE);
}

void SendRequestMagicAttack(int p_Type, uint8_t p_x, uint8_t p_y, uint8_t p_Serial, int p_Count, int* p_Key, uint16_t p_SkillSerial)\
{
	if (!FindText2(Hero->ID, "webzen"))
	{
		CStreamPacketEngine spe;
		spe.Init(0xC1, PACKET_MAGIC_ATTACK);
		uint16_t Type = (uint16_t)p_Type;
		spe << (uint8_t)(HIBYTE(Type)) << (uint8_t)(LOBYTE(Type)) << (uint8_t)(p_x) << (uint8_t)(p_y) << (uint8_t)MakeSkillSerialNumber(&p_Serial) << (uint8_t)(p_Count);

		for (int i = 0; i < p_Count; i++)
		{
			spe << (uint8_t)(p_Key[i] >> 8) << (uint8_t)(p_Key[i] & 0xff);
			spe << (uint8_t)p_SkillSerial;
		}
		spe.Send(TRUE);
	}
}

uint8_t GetDestValue(int xPos, int yPos, int xDst, int yDst)
{
	int DestX = xDst - xPos;
	int DestY = yDst - yPos;
	if (DestX < -8) DestX = -8;
	if (DestX > 7) DestX = 7;
	if (DestY < -8) DestY = -8;
	if (DestY > 7) DestY = 7;
	assert(-8 <= DestX && DestX <= 7);
	assert(-8 <= DestY && DestY <= 7);
	uint8_t byValue1 = ((uint8_t)(DestX + 8)) << 4;
	uint8_t byValue2 = ((uint8_t)(DestY + 8)) & 0xf;
	return (byValue1 | byValue2);
}

void SendRequestMagicContinue(int p_Type, uint8_t p_x, uint8_t p_y, uint8_t p_Angle, uint8_t p_Dest, uint8_t p_Tpos, uint16_t p_TKey, uint8_t* p_SkillSerial)
{
	if (!FindText2(Hero->ID, "webzen"))
	{
		CurrentSkill = p_Type;
		CStreamPacketEngine spe;
		uint16_t Type = (uint16_t)p_Type;
		spe.Init(0xC1, 0x1E);
		spe << (uint8_t)(HIBYTE(Type)) << (uint8_t)(LOBYTE(Type)) << (uint8_t)(p_x) << (uint8_t)(p_y) << (uint8_t)(p_Angle) << (uint8_t)(p_Dest) << (uint8_t)(p_Tpos) << (uint8_t)((p_TKey) >> 8) << (uint8_t)((p_TKey) & 0xff);
		spe << MakeSkillSerialNumber(p_SkillSerial);
		spe.Send(TRUE);
	}
}

void SendRequestMagicTeleport(bool* p_pbResult, int p_Type, int p_x, int p_y)
{
	if (Teleport || g_bWhileMovingZone || (GetTickCount() - g_dwLatestZoneMoving < 3000))
	{
		*(p_pbResult) = false;
	}
	else
	{
		if ((p_Type) == 0)
		{
			Teleport = true;
		}
		CStreamPacketEngine spe;
		spe.Init(0xC1, 0x1C);
		spe.AddNullData(1);
		spe << (uint16_t)(p_Type) << (uint8_t)(p_x) << (uint8_t)(p_y);
		spe.Send(TRUE);
		*(p_pbResult) = true;
	}
}

void SendRequestMagicTeleportB(bool* p_pbResult, int p_Index, int p_x, int p_y)
{
	if (Teleport)
	{
		Teleport = false;
		*(p_pbResult) = false;
	}
	else
	{
		Teleport = true;
		CStreamPacketEngine spe;
		spe.Init(0xC1, 0xB0);
		spe << (uint16_t)(p_Index) << (uint8_t)(p_x) << (uint8_t)(p_y);
		spe.Send(TRUE);
		*(p_pbResult) = true;
	}
}

void SendRequestTalk(uint16_t p_Key)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x30);
	spe << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff);
	spe.Send(TRUE);
}

void SendExitInventory()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x31);
	spe.Send(FALSE);
}

void SendRequestSell(int32_t p_Index)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x33);
	spe << (uint8_t)(p_Index);
	spe.Send(TRUE);
	g_pNPCShop->SetSellingItem(true);
}

void SendRequestBuy(int32_t Index)
{
	if (BuyCost != 0)
		return;
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)0xED;
	spe << (uint8_t)Index;
	spe.Send();
}

void SendRequestBuy(int32_t Index, int32_t Cost)
{
	if (BuyCost == 0)
	{
		CStreamPacketEngine spe;
		spe.Init(0xC1, 0x32);
		spe << (uint8_t)Index;
		spe.Send(TRUE);
		BuyCost = Cost;
	}
}

void SendRequestBuyInventory(int32_t Index)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x35);
	spe << (uint8_t)Index;
	spe.Send();
}

void SendRequestUnBreak(int32_t p_Index, int32_t p_AddGold)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x27);
	spe << (uint8_t)(p_Index) << (uint8_t)(p_AddGold);
	spe.Send();
}

void SendRequestRepair(int32_t p_Index, int32_t p_AddGold)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x34);
	spe << (uint8_t)(p_Index) << (uint8_t)(p_AddGold);
	spe.Send(TRUE);
}

void SendRequestEventChip(uint8_t p_Type, uint8_t p_Index)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x95);
	spe << (uint8_t)(p_Type) << (uint8_t)(p_Index);
	spe.Send(FALSE);
}

void SendRequestMutoNumber()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x96);
	spe.Send(FALSE);
}

void SendRequestEventChipExit()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x97);
	spe.Send(FALSE);
}

void SendRequestLenaExchange(uint8_t p_byType)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x98);
	spe << (uint8_t)(p_byType);
	spe.Send(FALSE);
}

void SendRequestScratchSerial(void* p_strSerial1, void* p_strSerial2, void* p_strSerial3)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x9D);
	spe.AddData(p_strSerial1, 5);
	spe.AddData(p_strSerial2, 5);
	spe.AddData(p_strSerial3, 5);
	spe.Send(FALSE);
}

void SendRequestServerImmigration(void* p_ResidentNumber)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x99);
	spe.AddData((p_ResidentNumber), 10);
	spe.Send(TRUE);
}

void SendRequestQuestHistory()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xA0);
	spe.Send(TRUE);
}

void SendRequestQuestState(uint8_t p_Index, uint8_t p_State)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xA2);
	spe << (uint8_t)(p_Index) << (uint8_t)(p_State);
	spe.Send(TRUE);
}

void SendRequestAttribute(uint8_t p_Att)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x9B);
	spe << (uint8_t)(p_Att);
	spe.Send(FALSE);
}

void SendRequestQuestMonKillInfo(uint8_t byQuestIndex)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xA4);
	spe << (uint8_t)0x00;
	spe << (uint8_t)byQuestIndex;
	spe.Send();
}

void SendQuestSelection(uint32_t dwQuestIndex, uint8_t byResult)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF6);
	spe << (uint8_t)0x0A;
	spe << (uint32_t)(dwQuestIndex);
	spe << (uint8_t)(byResult);
	spe.Send();
}

void SendQuestSelAnswer(uint32_t dwQuestIndex, uint8_t bySelAnswer)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF6);
	spe << (uint8_t)0x0B;
	spe << (uint32_t)(dwQuestIndex);
	spe << (uint8_t)(bySelAnswer);
	spe.Send();
}

void SendRequestQuestComplete(uint32_t dwQuestIndex)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF6);
	spe << (uint8_t)0x0D;
	spe << (uint32_t)(dwQuestIndex);
	spe.Send();
}

void SendSatisfyQuestRequestFromClient(uint32_t dwQuestIndex)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF6);
	spe << (uint8_t)0x10;
	spe << (uint32_t)(dwQuestIndex);
	spe.Send();
}

void SendRequestProgressQuestList()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF6);
	spe << (uint8_t)0x1A;
	spe.Send();
}

void SendRequestProgressQuestRequestReward(uint32_t dwQuestIndex)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF6);
	spe << (uint8_t)0x1B;
	spe << (uint32_t)(dwQuestIndex);
	spe.Send();
}

void SendRequestQuestGiveUp(uint32_t dwQuestIndex)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF6);
	spe << (uint8_t)0x0F;
	spe << (uint32_t)(dwQuestIndex);
	spe.Send();
}

void SendRequestQuestByEtcEPList()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF6);
	spe << (uint8_t)0x21;
	spe.Send();
}

void SendRequestGensJoining(uint8_t byInfluence)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF8);
	spe << (uint8_t)0x01;
	spe << (uint8_t)(byInfluence);
	spe.Send();
}

void SendRequestGensSecession()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF8);
	spe << (uint8_t)0x03;
	spe.Send();
}

void SendRequestGensReward(uint8_t byInfluence)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF8);
	spe << (uint8_t)0x09;
	spe << (uint8_t)(byInfluence);
	spe.Send();
}

void SendRequestGensInfo_Open()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF8);
	spe << (uint8_t)0x0B;
	spe.Send();
}

void SendRequestQuestByNPCEPList()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF6);
	spe << (uint8_t)0x30;
	spe.Send();
}

void SendRequestAPDPUp()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF6);
	spe << (uint8_t)0x31;
	spe.Send();
}

bool SendRequestEquipmentItem(uint8_t iSrcType, uint8_t iSrcIndex, ITEM* pItem, uint8_t iDstType, uint8_t iDstIndex)
{
	uint8_t splitType;
	uint8_t spareBits;
	uint8_t socketBits[MAX_SOCKETS];

	if (!EquipmentItem && pItem)
	{
		EquipmentItem = true;

		if (pItem->option_380 == true)
			splitType = ((pItem->Type >> 5) & 0xF0) | 0x8;
		else
			splitType = ((pItem->Type >> 5) & 0xF0);

		if (pItem->bPeriodItem == true)
			splitType |= 0x2u;

		if (pItem->bExpiredPeriod == true)
			splitType |= 0x4u;

		if (g_SocketItemMgr.IsSocketItem(pItem))
			spareBits = pItem->SocketSeedSetOption;
		else
			spareBits = (((uint8_t)pItem->Jewel_Of_Harmony_Option) << 4) + ((uint8_t)pItem->Jewel_Of_Harmony_OptionLevel);

		memcpy(socketBits, pItem->bySocketOption, MAX_SOCKETS);


		CStreamPacketEngine spe;
		spe.Init(0xC1, 0x24);

		spe << (uint8_t)iSrcType;
		spe << (uint8_t)iSrcIndex;
		spe << (uint8_t)(pItem->Type & 0xff);
		spe << (uint8_t)(pItem->Level & 0xff);
		spe << (uint8_t)(pItem->Durability);
		spe << (uint8_t)(pItem->Option1);
		spe << (uint8_t)(pItem->ExtOption);
		spe << (uint8_t)splitType;
		spe << (uint8_t)spareBits;
		spe << (uint8_t)socketBits[0];
		spe << (uint8_t)socketBits[1];
		spe << (uint8_t)socketBits[2];
		spe << (uint8_t)socketBits[3];
		spe << (uint8_t)socketBits[4];
		spe << (uint8_t)iDstType;
		spe << (uint8_t)iDstIndex;

		spe.Send(TRUE);
	}
	return true;
}

void SendRequestEquipmentItem(int32_t SrcFlag, int32_t SrcIndex, int32_t DstFlag, int32_t DstIndex)
{
	if (EquipmentItem) return;
	EquipmentItem = true;

	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x24);

	uint8_t splitType;

	if (PickItem.option_380)
	{
		splitType = ((uint8_t)(PickItem.Type >> 5) & 240) | 0x08;
	}
	else
		splitType = ((uint8_t)(PickItem.Type >> 5) & 240);

#ifdef KJH_ADD_PERIOD_ITEM_SYSTEM
	if (PickItem.bPeriodItem == true)
	{
		splitType |= 0x02;
	}

	if (PickItem.bExpiredPeriod == true)
	{
		splitType |= 0x04;
	}
#endif // #ifdef KJH_ADD_PERIOD_ITEM_SYSTEM

	uint8_t spareBits;
	if (g_SocketItemMgr.IsSocketItem(&PickItem))
	{
		spareBits = PickItem.SocketSeedSetOption;
	}
	else
	{
		spareBits = (((uint8_t)PickItem.Jewel_Of_Harmony_Option) << 4) + ((uint8_t)PickItem.Jewel_Of_Harmony_OptionLevel);
	}

	uint8_t socketBits[5] = { PickItem.bySocketOption[0], PickItem.bySocketOption[1], PickItem.bySocketOption[2], PickItem.bySocketOption[3], PickItem.bySocketOption[4] };

	spe << (uint8_t)SrcFlag << (uint8_t)SrcIndex << (uint8_t)PickItem.Type << (uint8_t)(PickItem.Level)
		<< (uint8_t)(PickItem.Durability) << (uint8_t)(PickItem.Option1) << (uint8_t)(PickItem.ExtOption) << splitType << spareBits
		<< socketBits[0] << socketBits[1] << socketBits[2] << socketBits[3] << socketBits[4]
		<< (uint8_t)DstFlag << (uint8_t)DstIndex;
	spe.Send(TRUE);

#ifdef CONSOLE_DEBUG
	g_ConsoleDebug->Write(MCD_SEND, "0x24 [SendRequestEquipmentItem(%d %d %d %d %d %d %d)]", SrcIndex, DstIndex, SrcFlag, DstFlag, (PickItem.Type & 0x1FFF), (uint8_t)(PickItem.Level), (uint8_t)(PickItem.Durability));
#endif // CONSOLE_DEBUG
}

void SendRequestUse(int32_t p_Index, int32_t p_Target)
{
	if (IsCanUseItem())
	{
		if (EnableUse <= 0)
		{
			EnableUse = 10;
			CStreamPacketEngine spe;
			spe.Init(0xC1, 0x26);
			spe << (uint8_t)((p_Index)+MAX_EQUIPMENT) << (uint8_t)(p_Target);
			spe << (uint8_t)g_byItemUseType;
			spe.Send(TRUE);
			ITEM* pItem = g_pMyInventory->FindItem(p_Index);
			if (pItem)
			{
				if (pItem->Type == ITEM_POTION)
					PlayBuffer(SOUND_EAT_APPLE01);
				else if (pItem->Type >= ITEM_POTION + 1 && pItem->Type <= ITEM_POTION + 9)
					PlayBuffer(SOUND_DRINK01);
			}
		}
	}
	else
	{
		g_pChatListBox->AddText("", GlobalText[474], SEASON3B::TYPE_ERROR_MESSAGE);
	}
}

void SendRequestGetItem(int32_t p_Key)
{
	if (SendGetItem == -1)
	{
		SendGetItem = p_Key;

		CStreamPacketEngine spe;
		spe.Init(0xC1, 0x22);
		spe << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff);
		spe.Send(TRUE);
	}
}

void SendRequestDropItem(uint8_t p_InventoryIndex, uint8_t p_x, uint8_t p_y)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x23);
	spe << (uint8_t)(p_x) << (uint8_t)(p_y) << (uint8_t)(p_InventoryIndex);
	spe.Send(TRUE);
	SendDropItem = (p_InventoryIndex);
}

#ifdef _PVP_ADD_MOVE_SCROLL
#define SendRequestCharacterEffect( p_key, p_type)\
{\
	CStreamPacketEngine spe;\
	spe.Init( 0xC1, 0x48);\
	spe << ( uint8_t)( p_key >> 8) << ( uint8_t)( p_key & 0x00FF) << ( uint8_t)( p_type);\
	spe.Send( FALSE);\
}
#endif	// _PVP_ADD_MOVE_SCROLL

void SendRequestPostChat(int32_t p_Index)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x25);
	spe << (uint8_t)p_Index;
	spe.Send();
}

void SendRequestAddPoint(uint8_t p_Type)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)0x06 << (uint8_t)(p_Type);
	spe.Send();
	PlayBuffer(SOUND_CLICK01);
}

void SendRequestTrade(uint16_t p_Key)
{
	if (IsCanTrade() && EnableMainRender)
	{
		CStreamPacketEngine spe;
		spe.Init(0xC1, 0x36);
		spe << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff);
		spe.Send(TRUE);
	}
}

void SendRequestTradeAnswer(uint8_t p_Result)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x37);
	spe << (uint8_t)(p_Result);
	spe.Send();
}

void SendRequestTradeGold(uint32_t p_Gold, uint32_t coin1, uint32_t coin2, uint32_t coin3)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x3A);
	spe.AddNullData(1);
	spe << (uint32_t)(p_Gold);
	spe << (uint32_t)(coin1);
	spe << (uint32_t)(coin2);
	spe << (uint32_t)(coin3);
	spe.Send();
}

void SendRequestTradeResult(uint8_t p_Result)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x3C);
	spe << (uint8_t)(p_Result);
	spe.Send(TRUE);
}

void SendRequestTradeExit()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x3D);
	spe.Send(TRUE);
}

void SendPing(int p_Key)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x71);
	spe.AddNullData(1);
	spe << (int)(p_Key);
	spe.Send();
}

void SendRequestMoveMap(uint32_t dwBlockKey, uint16_t wMapIndex)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x8E);
	spe << (uint8_t)0x02; \
		spe << (uint32_t)dwBlockKey << (uint16_t)wMapIndex;
	spe.Send();

#ifdef CONSOLE_DEBUG
	g_ConsoleDebug->Write(MCD_SEND, "0x8E [SendRequestMoveMap(%d %d)]", dwBlockKey, wMapIndex);
#endif // CONSOLE_DEBUG
}

void SendRequestStorageGold(int Flag, int Gold)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x81);
	spe << (uint8_t)Flag << (uint32_t)Gold;
	spe.Send();

#ifdef CONSOLE_DEBUG
	g_ConsoleDebug->Write(MCD_SEND, "0x81 Send [SendRequestStorageGold(%d %d)]", Flag, Gold);
#endif // CONSOLE_DEBUG
}

bool SendRequestStorageExit()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x82);
	spe.Send();

#ifdef CONSOLE_DEBUG
	g_ConsoleDebug->Write(MCD_SEND, "0x82 Send [SendRequestStorageExit]");
#endif // CONSOLE_DEBUG

	return true;
}

void SendStoragePassword(uint8_t p_byType, uint16_t p_wPassword, void* p_ResidentNumber)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x83);
	spe << (uint8_t)(p_byType) << (uint16_t)(p_wPassword);
	spe.AddData((p_ResidentNumber), 20);
	spe.Send(TRUE);
}

void SendRequestParty(uint16_t p_Key)
{
	if (EnableMainRender)
	{
		PartyKey = p_Key;
		CStreamPacketEngine spe;
		spe.Init(0xC1, 0x40);
		spe << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff);
		spe.Send(TRUE);
	}
}

void SendRequestPartyAnswer(uint8_t p_Result)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x41);
	spe << (uint8_t)(p_Result) << (uint8_t)(PartyKey >> 8) << (uint8_t)(PartyKey & 0xff);
	spe.Send(TRUE);
}

void SendRequestPartyList()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x42);
	spe.Send();
}

void SendRequestPartyLeave(uint8_t p_Index)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x43);
	spe << (uint8_t)(p_Index);
	spe.Send();
}

void SendRequestGuildMaster(uint8_t p_Value)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x54);
	spe << (uint8_t)(p_Value);
	spe.Send();
}

void SendRequestCreateGuild(uint8_t GuildType, void* pGuildName, void* pGuildMark)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x55);
	spe << (uint8_t)(GuildType);
	spe.AddData((pGuildName), 8);
	spe.AddData((pGuildMark), 32);
	spe.Send();
}

void SendRequestEditGuildType(uint8_t GuildType)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xE2);
	spe << (uint8_t)(GuildType);
	spe.Send();
}

void SendRequestGuildRelationShip(uint8_t RelationType, uint8_t RequestType, uint8_t TargetUserIndexH, uint8_t TargetUserIndexL)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xE5);
	spe << (uint8_t)(RelationType);
	spe << (uint8_t)(RequestType);
	spe << (uint8_t)(TargetUserIndexH);
	spe << (uint8_t)(TargetUserIndexL);
	spe.Send(); \
}

void SendRequestGuildRelationShipResult(uint8_t Type, uint8_t RequestType, uint8_t Result, uint8_t TargetUserIndexH, uint8_t TargetUserIndexL)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xE6);
	spe << (uint8_t)(Type);
	spe << (uint8_t)(RequestType);
	spe << (uint8_t)(Result);
	spe << (uint8_t)(TargetUserIndexH);
	spe << (uint8_t)(TargetUserIndexL);
	spe.Send();
}

void SendRequestBanUnionGuild(void* GuildName)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xEB);
	spe << (uint8_t)0x01;
	spe.AddData((GuildName), 8);
	spe.Send();
}

void SendRequestUnionList()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xE9);
	spe.Send();
}

void SendRequestEditGuildMark(void* p_Name, void* p_Mark)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x55); \
		spe.AddData((p_Name), 8);
	spe.AddData((p_Mark), 32);
	spe.Send();
}

void SendRequestGuild(uint16_t p_Key)
{
	if (EnableMainRender)
	{
		GuildPlayerKey = (p_Key);
		CStreamPacketEngine spe;
		spe.Init(0xC1, 0x50);
		spe << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff);
		spe.Send();
	}
}

void SendRequestGuildAnswer(uint8_t p_Result)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x51);
	spe << (uint8_t)(p_Result) << (uint8_t)(GuildPlayerKey >> 8) << (uint8_t)(GuildPlayerKey & 0xff);
	spe.Send();
}

void SendRequestCreateGuildCancel()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x57);
	spe.Send();
}

void SendRequestGuildList()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x52);
	spe.Send();
}

void SendRequestGuildLeave(char* p_ID, char* p_ResidentNumber)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x53);
	spe.AddData((p_ID), MAX_ID_SIZE);
	spe.AddData((p_ResidentNumber), 20);
	spe.Send();
}

void SendRequestDeclareWar(char* p_Name)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x60);
	spe.AddData((p_Name), 8);
	spe.Send();
}

void SendRequestGuildWarAnswer(uint8_t p_Result)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x61);
	spe << (uint8_t)(p_Result);
	spe.Send();
	if (!p_Result) InitGuildWar();
}

void SendRequestGuildInfo(int32_t p_GuildKey)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x66);
	spe.AddNullData(1);
	spe << (int32_t)(p_GuildKey);
	spe.Send();
}

void SendRequestGuildAssign(uint8_t Type, uint8_t GuildStatus, char* Name)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xE1);
	spe << (uint8_t)(Type);
	spe << (uint8_t)(GuildStatus);
	spe.AddData((Name), MAX_ID_SIZE);
	spe.Send();
}

void SendRequestMix(uint16_t p_Type, uint16_t p_SubType)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x86);
	spe.AddNullData(1);
	spe << (uint16_t)(p_Type);
	spe << (uint16_t)(p_SubType);
	spe.Send();
}

bool SendRequestMixExit()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x87);
	spe.Send();

#ifdef CONSOLE_DEBUG
	g_ConsoleDebug->Write(MCD_SEND, "0x87 [SendRequestMixExit]");
#endif // CONSOLE_DEBUG
	return true;
}

void SendRequestGemMix(uint8_t iType, uint8_t iLevel)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBC);
	spe << (uint8_t)0x00;
	spe << (uint8_t)(iType) << (uint8_t)(iLevel);
	spe.Send();
}

void SendRequestGemUnMix(uint8_t iType, uint8_t iLevel, uint8_t iPos)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBC);
	spe << (uint8_t)0x01;
	spe << (uint8_t)(iType) << (uint8_t)(iLevel) << (uint8_t)(iPos);
	spe.Send();
}

void SendRequestMoveToDevilSquare(uint8_t bySquareNumber, uint8_t iItemIndex)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x90);
	spe << (uint8_t)(bySquareNumber) << (uint8_t)((iItemIndex)+12);
	spe.Send();
}

void SendRequestDevilSquareOpenTime()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x91);
	spe.Send();
}

void SendRequestEventCount(uint8_t wType)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x9F);
	spe << (uint8_t)(wType);
	spe.Send();
}

void SendRequestMoveToEventMatch(uint8_t byCastleNumber, uint8_t iItemIndex)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x9A);
	spe << (uint8_t)(byCastleNumber) << (uint8_t)(iItemIndex);
	spe.Send();
}

void SendRequestEventZoneOpenTime(uint8_t byZoneType, uint8_t iItemLevel)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x91);
	spe << (uint8_t)(byZoneType) << (uint8_t)(iItemLevel);
	spe.Send();
}

extern void SendRequestMoveToEventMatch2(uint8_t byCastleNumber, uint8_t iItemIndex)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xAF);
	spe << (uint8_t)0x01;
	spe << (uint8_t)(byCastleNumber) << (uint8_t)(iItemIndex);
	spe.Send();
}


extern void SendRequestCheckPosition(uint8_t byPositionX, uint8_t byPositionY)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xAF);
	spe << (uint8_t)0x02;
	spe << (uint8_t)(byPositionX) << (uint8_t)(byPositionY);
	spe.Send();
}

extern void SendRequestHotKey(void* option)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)0x30;
	spe.AddData(option, 30);
	spe.Send();
}

void SendRequestDuelStart(uint16_t index, char* name)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xAA);
	spe << (uint8_t)0x01;
	spe << (uint8_t)((int16_t)index >> 8) << (uint8_t)((int16_t)index & 0xFF);
	spe.AddData(name, 10);
	spe.Send(TRUE);
}

void SendRequestDuelOk(uint8_t ok, int32_t index, char* name)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xAA);
	spe << (uint8_t)0x02;
	spe << (uint8_t)(ok) << (uint8_t)((int16_t)index >> 8) << (uint8_t)((int16_t)index & 0xFF);
	spe.AddData(name, 10);
	spe.Send(TRUE);
}

void SendRequestDuelEnd()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xAA);
	spe << (uint8_t)0x03;
	spe.Send(TRUE);
}

void SendRequestJoinChannel(uint8_t channelid)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xAA);
	spe << (uint8_t)0x07;
	spe << (uint8_t)(channelid);
	spe.Send(TRUE);
}

void SendRequestQuitChannel(uint8_t channelid)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xAA);
	spe << (uint8_t)0x09;
	spe << (uint8_t)(channelid);
	spe.Send(TRUE);
}

void SendRequestSetSalePrice(uint8_t offset, int32_t money, uint32_t JewelBless, uint32_t JewelSoul, uint32_t JewelChaos, uint8_t ShopType)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x3F);
	spe << (uint8_t)0x01;
	spe << (uint8_t)offset;
	spe << (int32_t)money;
	spe << (uint8_t)(JewelBless & 0xff);
	spe << (uint8_t)(JewelBless >> 8);
	spe << (uint8_t)(JewelSoul & 0xff);
	spe << (uint8_t)(JewelSoul >> 8);
	spe << (uint8_t)(JewelChaos & 0xff);
	spe << (uint8_t)(JewelChaos >> 8);
	spe << (uint8_t)ShopType;
	spe.Send(TRUE);
}

void SendRequestCreatePersonalShop(char* name)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x3F);
	spe << (uint8_t)0x02;
	spe.AddData(name, MAX_SHOPTITLE);
	spe.Send(TRUE);
}

void SendRequestDestoryPersonalShop()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x3F);
	spe << (uint8_t)0x03;
	spe.Send(TRUE);
}

void SendRequestOpenPersonalShop(uint16_t index, char* Id)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x3F);
	spe << (uint8_t)0x05;
	spe << (uint8_t)((int16_t)index >> 8) << (uint8_t)((int16_t)index & 0xFF);
	spe.AddData(Id, MAX_ID_SIZE);
	spe.Send(TRUE);
}

void SendRequestPurchase(int32_t index, char* Id, uint8_t offset)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x3F);
	spe << (uint8_t)0x06;
	spe << (uint8_t)((int16_t)index >> 8) << (uint8_t)((int16_t)index & 0xFF);
	spe.AddData(Id, MAX_ID_SIZE);
	spe << (uint8_t)offset;
	spe.Send(TRUE);
}

void SendRequestClosePersonalShop(int32_t index, char* Id)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x3F);
	spe << (uint8_t)0x07;
	spe << (uint8_t)((int16_t)index >> 8) << (uint8_t)((int16_t)index & 0xFF);
	spe.AddData(Id, MAX_ID_SIZE);
	spe.Send(TRUE);
}

void SendRequestInventory()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)0x10;
	spe.Send(TRUE);
}

void SendRequestFriendList()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xC0);
	spe.Send();
}

void SendRequestAddFriend(char* name)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xC1);
	spe.AddData(name, 10);
	spe.Send();
}

void SendAcceptAddFriend(uint8_t result, char* name)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xC2);
	spe << (uint8_t)(result);
	spe.AddData(name, MAX_ID_SIZE);
	spe.Send();
}

void SendRequestDeleteFriend(char* name)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xC3);
	spe.AddData(name, MAX_ID_SIZE);
	spe.Send();
}

void SendRequestChangeState(uint8_t ChatState)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xC4);
	spe << (uint8_t)(ChatState);
	spe.Send();
}

void SendRequestSendLetter(uint32_t windowuiid, char* name, char* subject, uint8_t photodir, uint8_t photoaction, uint32_t memosize, void* memo)
{
	CStreamPacketEngine spe;
	spe.Init(0xC2, 0xC5);
	spe << (uint32_t)(windowuiid);
	spe.AddData(name, MAX_ID_SIZE);
	spe.AddData(subject, MAX_LETTER_TITLE_LENGTH);
	spe << (uint8_t)(photodir);
	spe << (uint8_t)(photoaction);
	spe << (uint16_t)(memosize);
	spe.AddData(memo, memosize);
	spe.Send(TRUE, TRUE);
}

void SendRequestLetterText(uint16_t index)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xC7);
	spe.AddNullData(1);
	spe << (uint16_t)(index);
	spe.Send();
}

void SendRequestDeleteLetter(uint16_t index)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xC8);
	spe.AddNullData(1);
	spe << (uint16_t)(index);
	spe.Send();
}

void SendRequestDeliverLetter()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xC9);
	spe.Send();
}

void SendRequestCreateChatRoom(char* Name)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xCA);
	spe.AddData(Name, MAX_ID_SIZE);
	spe.Send();
}

void SendRequestCRInviteFriend(char* Name, uint16_t RoomNumber, uint32_t WindowUIID)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xCB);
	spe.AddData(Name, MAX_ID_SIZE);
	spe.AddNullData(1);
	spe << (uint16_t)(RoomNumber);
	spe << (uint32_t)(WindowUIID);
	spe.Send();
}

void SendRequestCRConnectRoom(CWsctlc* pSocketClient, uint16_t RoomNumber, void* Ticket)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x00);
	spe.AddNullData(1);
	spe << (uint16_t)(RoomNumber);
	spe.AddData(Ticket, 10);
	g_pSocketClient = pSocketClient;
	spe.Send();
	g_pSocketClient = &SocketClient;
}

void SendRequestCRDisconnectRoom(CWsctlc* pSocketClient)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x01);
	g_pSocketClient = pSocketClient;
	spe.Send();
	g_pSocketClient = &SocketClient;
}

void SendRequestCRUserList(CWsctlc* pSocketClient)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x02);
	g_pSocketClient = pSocketClient;
	spe.Send();
	g_pSocketClient = &SocketClient;
}

void SendRequestCRChatText(CWsctlc* pSocketClient, uint8_t Index, uint8_t MsgSize, void* Msg)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x04);
	spe << (uint8_t)(Index);
	spe << (uint8_t)(MsgSize);
	BuxConvert((uint8_t*)Msg, MsgSize);
	spe.AddData(Msg, MsgSize);
	g_pSocketClient = pSocketClient;
	spe.Send();
	g_pSocketClient = &SocketClient;
}

void SendRequestCRConnectCheck(CWsctlc* pSocketClient)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x05);
	g_pSocketClient = pSocketClient;
	spe.Send();
	g_pSocketClient = &SocketClient;
}


void  SendRequestPetCommand(uint8_t PetType, uint8_t Command, uint16_t Key)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xA7);
	spe << (uint8_t)(PetType);
	spe << (uint8_t)(Command);
	spe << (uint8_t)((Key) >> 8) << (uint8_t)((Key) & 0xff);
	spe.Send();
}

void SendRequestPetInfo(uint8_t PetType, uint8_t InvType, uint8_t nPos)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xA9);
	spe << (uint8_t)(PetType);
	spe << (uint8_t)(InvType);
	spe << (uint8_t)(nPos);
	spe.Send();
}

void SendChangeMServer(char* lpID, char* lpChr, int32_t iAuth1, int32_t iAuth2, int32_t iAuth3, int32_t iAuth4)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB1);
	spe << (uint8_t)0x01;
	char lpszID[MAX_ID_SIZE + 2];
	char lpszCHR[MAX_ID_SIZE + 2];
	ZeroMemory(lpszID, MAX_ID_SIZE + 2);
	ZeroMemory(lpszCHR, MAX_ID_SIZE + 2);
	strcpy(lpszID, lpID);
	strcpy(lpszCHR, lpChr);
	BuxConvert((uint8_t*)lpszID, MAX_ID_SIZE + 2);
	spe.AddData(lpszID, MAX_ID_SIZE + 2);
	spe.AddData(lpszCHR, MAX_ID_SIZE + 2);
	spe << (int32_t)(iAuth1);
	spe << (int32_t)(iAuth2);
	spe << (int32_t)(iAuth3);
	spe << (int32_t)(iAuth4);
	spe << GetTickCount();

	for (int i = 0; i < SIZE_PROTOCOLVERSION; i++)
		spe << (uint8_t)(Version[i] - (i + 1));
	for (int i = 0; i < SIZE_PROTOCOLSERIAL; i++)
		spe << Serial[i];
	spe.Send(TRUE);
}

void SendRequestBCStatus()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x00;
	spe.Send();
}

void SendRequestBCReg()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x01;
	spe.Send();
}

void SendRequestBCGiveUp(uint8_t GiveUp)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x02;
	spe << (uint8_t)GiveUp;
	spe.Send();
}

void SendRequestBCRegInfo()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x03;
	spe.Send();
}

void SendRequestBCRegMark(int32_t ItemPos)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x04;
	spe << (uint8_t)ItemPos;
	spe.Send();
}

void SendRequestBCNPCBuy(int32_t NPCNumber, int32_t NPCIndex)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x05;
	spe << (int32_t)NPCNumber;
	spe << (int32_t)NPCIndex;
	spe.Send();
}

void SendRequestBCNPCRepair(int32_t NPCNumber, int32_t NPCIndex)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x06;
	spe << (int32_t)NPCNumber;
	spe << (int32_t)NPCIndex;
	spe.Send();
}

void SendRequestBCNPCUpgrade(int32_t NPCNumber, int32_t NPCIndex, int32_t NPCUpType, int32_t NPCUpValue)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x07;
	spe << (int32_t)NPCNumber;
	spe << (int32_t)NPCIndex;
	spe << (int32_t)NPCUpType;
	spe << (int32_t)NPCUpValue;
	spe.Send();
}

void SendRequestBCGetTaxInfo()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x08;
	spe.Send();
}

void SendRequestBCChangeTaxRate(uint8_t TaxType, uint8_t TaxRate1, uint8_t TaxRate2, uint8_t TaxRate3, uint8_t TaxRate4)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x09;
	spe << (uint8_t)TaxType;
	spe << (uint8_t)TaxRate1 << (uint8_t)TaxRate2 << (uint8_t)TaxRate3 << (uint8_t)TaxRate4;
	spe.Send();
}

void SendRequestBCWithdraw(uint8_t Money1, uint8_t Money2, uint8_t Money3, uint8_t Money4)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x10;
	spe << (uint8_t)Money1;
	spe << (uint8_t)Money2;
	spe << (uint8_t)Money3;
	spe << (uint8_t)Money4;
	spe.Send();
}

void SendRequestBCNPCList(uint8_t MonsterCode)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB3);
	spe << (uint8_t)MonsterCode;
	spe.Send();
}

void SendRequestBCDeclareGuildList()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB4);
	spe.Send();
}

void SendRequestBCGuildList()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB5);
	spe.Send();
}

void SendRequestHuntZoneEnter(uint8_t p_HuntZoneEnter)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x1F;
	spe << (uint8_t)p_HuntZoneEnter;
	spe.Send();
}

void SendGetCastleGuildMark()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB9);
	spe << (uint8_t)0x02;
	spe.Send();
}

void SendCastleHuntZoneEnter(int32_t p_Money)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB9);
	spe << (uint8_t)0x05;
	spe << (int32_t)p_Money;
	spe.Send();
}

void SendGateOparator(uint8_t p_Operator, int32_t p_Key)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x12;
	spe << (uint8_t)p_Operator << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff);
	spe.Send();
}

void SendCatapultFire(uint8_t p_Key, uint8_t p_Index)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB7);
	spe << (uint8_t)0x01;
	spe << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff) << (uint8_t)(p_Index);
	spe.Send();
}

void SendWeaponExplosion(uint8_t p_KeyH, uint8_t p_KeyL)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB7);
	spe << (uint8_t)0x04;
	spe << (uint8_t)p_KeyH << (uint8_t)p_KeyL;
	spe.Send();
}

void SendGuildCommand(uint8_t p_Team, uint8_t p_posX, uint8_t p_posY, uint8_t p_Cmd)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xB2);
	spe << (uint8_t)0x1D;
	spe << (uint8_t)p_Team << (uint8_t)p_posX << (uint8_t)p_posY << (uint8_t)p_Cmd;
	spe.Send();
}

void SendRequestCrywolfInfo()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBD);
	spe << (uint8_t)0x00;
	spe.Send();
}

void SendRequestCrywolfAltarContract(uint16_t p_Key)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBD);
	spe << (uint8_t)0x03;
	spe << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff);
	spe.Send(); \
}

void SendRequestCrywolfBenefitPlusChaosRate()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBD);
	spe << (uint8_t)0x09;
	spe.Send();
}

void SendRequestKanturuInfo()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD1);
	spe << (uint8_t)0x00;
	spe.Send();
}

void SendRequestKanturuEnter()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD1);
	spe << (uint8_t)0x01;
	spe.Send();
}

void SendRequestWhiteAngelItem()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD0);
	spe << (uint8_t)0x03;
	spe.Send();
}

void SendRequestEnterOnWerwolf()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD0);
	spe << (uint8_t)0x07;
	spe.Send();
}

void SendRequestEnterOnGateKeeper()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD0);
	spe << (uint8_t)0x08;
	spe.Send();
}

void SendRequest3ColorHarvestItem()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD0);
	spe << (uint8_t)0x09;
	spe.Send();
}

void SendRequestSantaItem()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD0);
	spe << (uint8_t)0x10;
	spe.Send();
}

void SendRequestMoveDevias()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD0);
	spe << (uint8_t)0x0A;
	spe.Send();
}

void SendRequestRaklionStateInfo()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD1);
	spe << (uint8_t)0x10;
	spe.Send();
}

void SendRequestCursedEnter(uint8_t mapnumber, uint8_t itempos)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBF);
	spe << (uint8_t)0x00;
	spe << (uint8_t)mapnumber << (uint8_t)itempos;
	spe.Send();
}

void SendRequestCursedTempleMagic(uint16_t MagicNumber, int32_t TargetObjIndex, uint8_t Dis)
{
	CStreamPacketEngine spe;
	uint16_t Magic = (uint16_t)MagicNumber;
	spe.Init(0xC1, 0xBF);
	spe << (uint8_t)0x02;
	spe << (uint8_t)(HIBYTE(Magic)) << (uint8_t)(LOBYTE(Magic)) << (uint8_t)((TargetObjIndex) >> 8) << (uint8_t)((TargetObjIndex) & 0xff) << (uint8_t)Dis;
	spe.Send();
}

void SendRequestCursedTempleRequital()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBF);
	spe << (uint8_t)0x05;
	spe.Send();
}

void SendRequestMasterLevelSkill(int32_t SkillNum)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)0x52;
	spe << (int32_t)SkillNum;
	spe.Send();
}

void SendRequestResetCharacterPoint()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF2);
	spe << (uint8_t)0x00;
	spe.Send();
}

void SendRequestRegistedLuckyCoin()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBF);
	spe << (uint8_t)0x0B;
	spe.Send();
}

void SendRequestRegistLuckyCoin()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBF);
	spe << (uint8_t)0x0C;
	spe.Send();
}

void SendRequestExChangeLuckyCoin(int32_t nCoinCnt)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBF);
	spe << (uint8_t)0x0D;
	spe << (int32_t)nCoinCnt;
	spe.Send();
}

void SendRequestEnterDoppelGangerEvent(uint8_t btItemPos)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBF);
	spe << (uint8_t)0x0E;
	spe << (uint8_t)btItemPos;
	spe.Send();
}

void SendRequestEnterEmpireGuardianEvent()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF7);
	spe << (uint8_t)0x01;
	spe << (uint8_t)0x01;
	spe.Send();
}

void SendRequestEnterUnitedMarketPlaceEvent()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBF);
	spe << (uint8_t)0x17;
	spe.Send();
}

void SendRequestIGS_CashPointInfo()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD2);
	spe << (uint8_t)0x01;
	spe.Send();
}

void SendRequestIGS_CashShopOpen(uint8_t byShopOpenType)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD2);
	spe << (uint8_t)0x02;
	spe << (uint8_t)byShopOpenType;
	spe.Send();
}

void SendRequestIGS_BuyItem(int32_t lBuyItemPackageSeq, int32_t lBuyItemDisplaySeq, int32_t lBuyItemPriceSeq, uint16_t wItemCode, int32_t iCashType)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD2);
	spe << (uint8_t)0x03;
	spe << (int32_t)lBuyItemPackageSeq;
	spe << (int32_t)lBuyItemDisplaySeq;
	spe << (int32_t)lBuyItemPriceSeq;
	spe << (uint16_t)wItemCode;
	spe << (int)iCashType;
	spe.Send();
}

void SendRequestIGS_SendItemGift(int32_t lGiftItemPackageSeq, int32_t lGiftItemPriceSeq, int32_t lGiftItemDisplaySeq, int32_t lSaleZone, uint16_t wItemCode, int32_t iCashType, char* pstrReceiveUserID, char* pstrGiftMessage)
{
	char strReceiveUserID[MAX_ID_SIZE + 1];
	char strGiftMessage[MAX_GIFT_MESSAGE_SIZE];
	ZeroMemory(strReceiveUserID, MAX_ID_SIZE + 1);
	ZeroMemory(strGiftMessage, MAX_GIFT_MESSAGE_SIZE);
	memcpy(strReceiveUserID, pstrReceiveUserID, MAX_ID_SIZE + 1);
	memcpy(strGiftMessage, pstrGiftMessage, MAX_GIFT_MESSAGE_SIZE);

	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD2);
	spe << (uint8_t)0x04;
	spe << (int32_t)lGiftItemPackageSeq;
	spe << (int32_t)lGiftItemPriceSeq;
	spe << (int32_t)lGiftItemDisplaySeq;
	spe << (int32_t)lSaleZone;
	spe << (uint16_t)wItemCode;
	spe << (int)iCashType;
	spe.AddData(strReceiveUserID, MAX_ID_SIZE + 1);
	spe.AddData(strGiftMessage, MAX_GIFT_MESSAGE_SIZE);
	spe.Send();
}

void SendRequestIGS_ItemStorageList(int32_t iPageIndex, char* szStorageType)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD2);
	spe << (uint8_t)0x05;
	spe << (int)iPageIndex;
	spe.AddData(szStorageType, sizeof(char) + 1);
	spe.Send();
}

/*#define SendRequestIGS_SendCashGift(dCashValue, pstrReceiveUserID, pstrGiftMessage) \
{\
	char strReceiveUserID[MAX_ID_SIZE+2]; \
	char strGiftMessage[MAX_GIFT_MESSAGE_SIZE+1]; \
	ZeroMemory( lpszID, MAX_ID_SIZE+2); \
	ZeroMemory( lpszPass, MAX_GIFT_MESSAGE_SIZE+1); \
	memcpy( strReceiveUserID, pstrReceiveUserID, MAX_ID_SIZE+1 ); \
	memcpy( strGiftMessage, pstrGiftMessage, MAX_GIFT_MESSAGE_SIZE); \
	CStreamPacketEngine spe; \
	spe.Init( 0xC1, 0xD2); \
	spe << (uint8_t)0x07; \
	spe << (double)dCashValue; \
	spe.AddData( strReceiveUserID, MAX_ID_SIZE+1); \
	spe.AddData( strGiftMessage, MAX_GIFT_MESSAGE_SIZE); \
	spe.Send(); \
}*/

void SendRequestIGS_PossibleBuy()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD2);
	spe << (uint8_t)0x08;
	spe.Send();
}

void SendRequestIGS_LeftCountItem(int32_t lPackageSeq)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD2);
	spe << (uint8_t)0x09;
	spe << (int32_t)lPackageSeq;
	spe.Send();
}

void SendRequestIGS_DeleteStorageItem(int32_t lStorageSeq, int32_t lStorageItemSeq, char* pstrStorageItemType)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD2);
	spe << (uint8_t)0x0A;
	spe << (int32_t)lStorageSeq;
	spe << (int32_t)lStorageItemSeq;
	spe.AddData(pstrStorageItemType, sizeof(char));
	spe.Send();
}

void SendRequestIGS_UseStorageItem(int32_t lStorageSeq, int32_t lStorageItemSeq, uint16_t wItemCode, char* pstrStorageItemType)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD2);
	spe << (uint8_t)0x0B;
	spe << (int32_t)lStorageSeq;
	spe << (int32_t)lStorageItemSeq;
	spe << (uint16_t)wItemCode;
	spe.AddData(pstrStorageItemType, sizeof(char));
	spe.Send();
}

void SendRequestIGS_EventItemList(int32_t lEventCategorySeq)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xD2);
	spe << (uint8_t)0x13;
	spe << (int32_t)lEventCategorySeq;
	spe.Send();
}

bool SendRequestEquippingInventoryItem(uint8_t iItemPos, uint8_t iValue)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBF);
	spe << (uint8_t)0x20;
	spe << (uint8_t)iItemPos;
	spe << (uint8_t)iValue;
	spe.Send();

	return true;
}

void SendRequestStartHelper(uint8_t status)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xBF);
	spe << (uint8_t)0x51;
	spe << (uint8_t)status;
	spe.Send();
}

void SendRequestSaveHelperSetting(uint8_t* buffer, int32_t Size)
{
	CStreamPacketEngine spe;
	spe.Init(0xC2, 0xAE);
	spe << (uint8_t)0;
	spe.AddData(buffer, Size);
	spe.Send();
}

void SendRequestRageAtt(int32_t p_Type, int32_t p_Key)
{
	if (!FindText2(Hero->ID, "webzen"))
	{
		CStreamPacketEngine spe;
		uint16_t Type = (uint16_t)p_Type;
		spe.Init(0xC1, 0x4A);
		spe << (uint8_t)(HIBYTE(Type)) << (uint8_t)(LOBYTE(Type)) << (uint8_t)(0) << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff);
		spe.Send(TRUE);
	}
}

void SendRequestDarkside(uint16_t nSkill, int32_t p_Key)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x4B);
	spe << (uint8_t)(HIBYTE(nSkill)) << (uint8_t)(LOBYTE(nSkill)) << (uint8_t)((p_Key) >> 8) << (uint8_t)((p_Key) & 0xff);
	spe.Send();
}

void SendRequestRemoveJewelOfInventory(uint8_t btIndex, uint8_t btType, uint32_t btValue)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x4F);
	spe << (uint8_t)0x08;
	spe << (uint8_t)btIndex;
	spe << (uint8_t)btType;
	spe.AddNullData(2);
	spe << (uint32_t)btValue;
	spe.Send();
}

void SendRequestAddJewelOfInventory(uint8_t btIndex, uint8_t btType, uint32_t btValue)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0x4F);
	spe << (uint8_t)0x09;
	spe << (uint8_t)btIndex;
	spe << (uint8_t)btType;
	spe.AddNullData(2);
	spe << (uint32_t)btValue;
	spe.Send();
}

void SendRequestMaxRanking()
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)(0xE7);
	spe.Send();
}

void SendRequestRankingInfo(uint8_t p_Type)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)(0xE6);
	spe << (uint8_t)(p_Type);
	spe.Send();
}

void SendRequestRankingPlayer(int32_t characterIndex, const char* p_ID)
{
	char characterName[MAX_ID_SIZE + 1] = { 0 };

	strcpy_s(characterName, p_ID);

	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)0xE5;
	spe << (uint32_t)(characterIndex);
	spe.AddData(characterName, MAX_ID_SIZE);
	spe.Send();
}

void SendRequestPurcharseMembership(uint32_t p_Type)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)(0xF0);
	spe << (uint32_t)(p_Type);
	spe.Send();
}

void CGSendOffTrade(int32_t p_Type)
{
	CStreamPacketEngine spe;
	spe.Init(0xC1, 0xF3);
	spe << (uint8_t)(0xEB);
	spe << (int32_t)(p_Type);
	spe.Send();
}


void SendRequestMixIndex(uint8_t p_Index)
{
	CStreamPacketEngine spe;
	spe.Init(0xC2, 0x4F);
	spe << (uint8_t)(0x03);
	spe << (uint8_t)(p_Index);
	spe.Send();
}

void SendRequestMixState(uint8_t p_Index)
{
	CStreamPacketEngine spe;
	spe.Init(0xC2, 0x4F);
	spe << (uint8_t)(0x06);
	spe << (uint8_t)(p_Index);
	spe.Send();
}


void DataSend(uint8_t* byData, uint32_t size) // OK
{
	CStreamPacketEngine spe;

	spe.Init(byData[0], byData[1]);

	if (byData[0] == 0xC1 || byData[0] == 0xC3)
		spe.AddData(&byData[3], (size - 3), FALSE);
	else
		spe.AddData(&byData[4], (size - 4), FALSE);

	spe.Send((byData[0] == 0xC3 || byData[0] == 0xC4));
}