#pragma once

#include "dsplaysound.h"
#include "zzzscene.h"
#include "zzzinterface.h"
#include "zzzinventory.h"
#include "zzzMixInventory.h"
#include "SimpleModulus.h"
#include "WSclient.h"
#include "SocketSystem.h"
#include "ItemAddOptioninfo.h"
#include "SkillManager.h"

#ifdef _DEBUG
#define SAVE_PACKET
#define PACKET_SAVE_FILE	"PacketList.txt"
#include "./ExternalObject/leaf/stdleaf.h"
#endif
#include "./Utilities/Log/muConsoleDebug.h"
#include "NewUISystem.h" 
#include "ProtocolSend.h" 
#include "Utilities\Log\DebugAngel.h"

#define PACKET_MOVE					0xD4
#define PACKET_POSITION				0x15
#define PACKET_MAGIC_ATTACK			0xDB
#define PACKET_ATTACK				0x11

extern int HeroKey;
extern CWsctlc SocketClient;
extern int CurrentProtocolState;
extern CWsctlc* g_pSocketClient;
extern uint8_t g_byPacketSerialSend;
extern CSimpleModulus g_SimpleModulusCS;
extern CSimpleModulus g_SimpleModulusSC;
extern uint8_t Version[SIZE_PROTOCOLVERSION];
extern uint8_t Serial[SIZE_PROTOCOLSERIAL + 1];
extern ItemAddOptioninfo* g_pItemAddOptioninfo;


extern int SendGetItem;
extern int SendDropItem;
extern int  LogIn;
extern char LogInID[MAX_ID_SIZE + 1];
extern bool LogOut;
extern bool First;
extern int FirstTime;
extern BOOL g_bGameServerConnected;
extern int ChatTime;
extern char ChatText[256];
extern BOOL g_bWhileMovingZone;
extern DWORD g_dwLatestZoneMoving;
extern int DirTable[16];
extern char Password[MAX_ID_SIZE + 1];
extern char QuestionID[MAX_ID_SIZE + 1];
extern char Question[31];
extern char ChatWhisperID[MAX_ID_SIZE + 1];
extern int MoveCount;
extern int CurrentSkill;
extern bool Teleport;
extern int BuyCost;
extern int  EnableUse;
extern uint32_t g_dwLatestMagicTick;

extern uint8_t MakeSkillSerialNumber(uint8_t* pSerialNumber);
extern bool FindText2(char* Text, char* Token, bool First = false);
extern int SendPacket(char* buf, int len, BOOL bEncrypt = FALSE, BOOL bForceC4 = FALSE);

#include "StreamPacketEngine.h"

extern void SendRequestServerHWID();
extern void SendRequestServerList();
extern void SendRequestServerUpdate(int version);
extern void SendRequestServerAddress(int p_Index);
extern void SendRequestServerOff(uint8_t State);
extern void SendCheck(void);
extern void SendCheckSum(uint32_t dwCheckSum);
extern void SendHackingChecked(uint8_t byType, uint8_t byParam);
extern void SendRequestLogIn(const char* p_lpszID, const char* p_lpszPassword);
extern void SendRequestLogOut(int Flag);
extern void SendRequestCharactersList(uint8_t byLanguage);
extern void SendRequestCreateCharacter(char* p_ID, uint8_t p_Class, uint8_t p_Skin);
extern void SendRequestDeleteCharacter(char* p_ID, char* p_Resident);
extern void SendRequestJoinMapServer(char* p_ID);
extern void SendRequestFinishLoading();
extern void SendChat(const char* Text);
extern void SendChatWhisper(char* p_TargetID, char* p_Text);
extern void SendPosition(int p_x, int p_y);
extern void SendCharacterMove(uint16_t Key, float Angle, uint8_t PathNum, uint8_t* PathX, uint8_t* PathY, uint8_t TargetX, uint8_t TargetY);
extern void SendRequestAction(uint8_t p_Action, uint8_t p_Angle);
extern void SendRequestAttack(int p_Key, uint8_t p_Dir);
extern void SendRequestMagic(int p_Type, int p_Key);
extern void SendRequestCancelMagic(int p_Type, int p_Key);
extern void SendRequestMagicAttack(int p_Type, uint8_t p_x, uint8_t p_y, uint8_t p_Serial, int p_Count, int* p_Key, uint16_t p_SkillSerial);
extern uint8_t GetDestValue(int xPos, int yPos, int xDst, int yDst);
extern void SendRequestMagicContinue(int p_Type, uint8_t p_x, uint8_t p_y, uint8_t p_Angle, uint8_t p_Dest, uint8_t p_Tpos, uint16_t p_TKey, uint8_t* p_SkillSerial);
extern void SendRequestMagicTeleport(bool* p_pbResult, int p_Type, int p_x, int p_y);
extern void SendRequestMagicTeleportB(bool* p_pbResult, int p_Index, int p_x, int p_y);
extern void SendRequestTalk(uint16_t p_Key);
extern void SendExitInventory();
extern void SendRequestSell(int32_t p_Index);
extern void SendRequestBuy(int32_t Index);
extern void SendRequestBuy(int32_t Index, int32_t Cost);
extern void SendRequestBuyInventory(int32_t Index);
extern void SendRequestUnBreak(int32_t p_Index, int32_t p_AddGold);
extern void SendRequestRepair(int32_t p_Index, int32_t p_AddGold);
extern void SendRequestEventChip(uint8_t p_Type, uint8_t p_Index);
extern void SendRequestMutoNumber();
extern void SendRequestEventChipExit();
extern void SendRequestLenaExchange(uint8_t p_byType);
extern void SendRequestScratchSerial(void* p_strSerial1, void* p_strSerial2, void* p_strSerial3);
extern void SendRequestServerImmigration(void* p_ResidentNumber);
extern void SendRequestQuestHistory();
extern void SendRequestQuestState(uint8_t p_Index, uint8_t p_State);
extern void SendRequestAttribute(uint8_t p_Att);
extern void SendRequestQuestMonKillInfo(uint8_t byQuestIndex);
extern void SendQuestSelection(uint32_t dwQuestIndex, uint8_t byResult);
extern void SendQuestSelAnswer(uint32_t dwQuestIndex, uint8_t bySelAnswer);
extern void SendRequestQuestComplete(uint32_t dwQuestIndex);
extern void SendSatisfyQuestRequestFromClient(uint32_t dwQuestIndex);
extern void SendRequestProgressQuestList();
extern void SendRequestProgressQuestRequestReward(uint32_t dwQuestIndex);
extern void SendRequestQuestGiveUp(uint32_t dwQuestIndex);
extern void SendRequestQuestByEtcEPList();
extern void SendRequestGensJoining(uint8_t byInfluence);
extern void SendRequestGensSecession();
extern void SendRequestGensReward(uint8_t byInfluence);
extern void SendRequestGensInfo_Open();
extern void SendRequestQuestByNPCEPList();
extern void SendRequestAPDPUp();
extern bool SendRequestEquipmentItem(uint8_t iSrcType, uint8_t iSrcIndex, ITEM* pItem, uint8_t iDstType, uint8_t iDstIndex);
extern void SendRequestEquipmentItem(int32_t SrcFlag, int32_t SrcIndex, int32_t DstFlag, int32_t DstIndex);
extern void SendRequestUse(int32_t p_Index, int32_t p_Target);
extern void SendRequestGetItem(int32_t p_Key);
extern void SendRequestDropItem(uint8_t p_InventoryIndex, uint8_t p_x, uint8_t p_y);

#ifdef _PVP_ADD_MOVE_SCROLL
#define SendRequestCharacterEffect( p_key, p_type)\
{\
	CStreamPacketEngine spe;\
	spe.Init( 0xC1, 0x48);\
	spe << ( uint8_t)( p_key >> 8) << ( uint8_t)( p_key & 0x00FF) << ( uint8_t)( p_type);\
	spe.Send( FALSE);\
}
#endif	// _PVP_ADD_MOVE_SCROLL

extern void SendRequestPostChat(int32_t p_Index);
extern void SendRequestAddPoint(uint8_t p_Type);
extern void SendRequestTrade(uint16_t p_Key);
extern void SendRequestTradeAnswer(uint8_t p_Result);
extern void SendRequestTradeGold(uint32_t p_Gold, uint32_t coin1, uint32_t coin2, uint32_t coin3);
extern void SendRequestTradeResult(uint8_t p_Result);
extern void SendRequestTradeExit();
extern void SendPing(int p_Key);
extern void SendRequestMoveMap(uint32_t dwBlockKey, uint16_t wMapIndex);
extern void SendRequestStorageGold(int Flag, int Gold);
extern bool SendRequestStorageExit();
extern void SendStoragePassword(uint8_t p_byType, uint16_t p_wPassword, void* p_ResidentNumber);
extern void SendRequestParty(uint16_t p_Key);
extern void SendRequestPartyAnswer(uint8_t p_Result);
extern void SendRequestPartyList();
extern void SendRequestPartyLeave(uint8_t p_Index);
extern void SendRequestGuildMaster(uint8_t p_Value);
extern void SendRequestCreateGuild(uint8_t GuildType, void* pGuildName, void* pGuildMark);
extern void SendRequestEditGuildType(uint8_t GuildType);
extern void SendRequestGuildRelationShip(uint8_t RelationType, uint8_t RequestType, uint8_t TargetUserIndexH, uint8_t TargetUserIndexL);
extern void SendRequestGuildRelationShipResult(uint8_t Type, uint8_t RequestType, uint8_t Result, uint8_t TargetUserIndexH, uint8_t TargetUserIndexL);
extern void SendRequestBanUnionGuild(void* GuildName);
extern void SendRequestUnionList();
extern void SendRequestEditGuildMark(void* p_Name, void* p_Mark);
extern void SendRequestGuild(uint16_t p_Key);
extern void SendRequestGuildAnswer(uint8_t p_Result);
extern void SendRequestCreateGuildCancel();
extern void SendRequestGuildList();
extern void SendRequestGuildLeave(char* p_ID, char* p_ResidentNumber);
extern void SendRequestDeclareWar(char* p_Name);
extern void InitGuildWar();
extern void SendRequestGuildWarAnswer(uint8_t p_Result);
extern void SendRequestGuildInfo(int32_t p_GuildKey);
extern void SendRequestGuildAssign(uint8_t Type, uint8_t GuildStatus, char* Name);
extern void SendRequestMix(uint16_t p_Type, uint16_t p_SubType);
extern bool SendRequestMixExit();
extern void SendRequestGemMix(uint8_t iType, uint8_t iLevel);
extern void SendRequestGemUnMix(uint8_t iType, uint8_t iLevel, uint8_t iPos);
extern void SendRequestMoveToDevilSquare(uint8_t bySquareNumber, uint8_t iItemIndex);
extern void SendRequestDevilSquareOpenTime();
extern void SendRequestEventCount(uint8_t wType);
extern void SendRequestMoveToEventMatch(uint8_t byCastleNumber, uint8_t iItemIndex);
extern void SendRequestEventZoneOpenTime(uint8_t byZoneType, uint8_t iItemLevel);
extern void SendRequestMoveToEventMatch2(uint8_t byCastleNumber, uint8_t iItemIndex);
extern void SendRequestCheckPosition(uint8_t byPositionX, uint8_t byPositionY);
extern void SendRequestHotKey(void* option);
extern void SendRequestDuelStart(uint16_t index, char* name);
extern void SendRequestDuelOk(uint8_t ok, int32_t index, char* name);
extern void SendRequestDuelEnd();
extern void SendRequestJoinChannel(uint8_t channelid);
extern void SendRequestQuitChannel(uint8_t channelid);
extern void SendRequestSetSalePrice(uint8_t offset, int32_t money, uint32_t JewelBless, uint32_t JewelSoul, uint32_t JewelChaos, uint8_t ShopType);
extern void SendRequestCreatePersonalShop(char* name);
extern void SendRequestDestoryPersonalShop();
extern void SendRequestOpenPersonalShop(uint16_t index, char* Id);
extern void SendRequestPurchase(int32_t index, char* Id, uint8_t offset);
extern void SendRequestClosePersonalShop(int32_t index, char* Id);
extern void SendRequestInventory();
extern void SendRequestFriendList();
extern void SendRequestAddFriend(char* name);
extern void SendAcceptAddFriend(uint8_t result, char* name);
extern void SendRequestDeleteFriend(char* name);
extern void SendRequestChangeState(uint8_t ChatState);
extern void SendRequestSendLetter(uint32_t windowuiid, char* name, char* subject, uint8_t photodir, uint8_t photoaction, uint32_t memosize, void* memo);
extern void SendRequestLetterText(uint16_t index);
extern void SendRequestDeleteLetter(uint16_t index);
extern void SendRequestDeliverLetter();
extern void SendRequestCreateChatRoom(char* Name);
extern void SendRequestCRInviteFriend(char* Name, uint16_t RoomNumber, uint32_t WindowUIID);
extern void SendRequestCRConnectRoom(CWsctlc* pSocketClient, uint16_t RoomNumber, void* Ticket);
extern void SendRequestCRDisconnectRoom(CWsctlc* pSocketClient);
extern void SendRequestCRUserList(CWsctlc* pSocketClient);
extern void SendRequestCRChatText(CWsctlc* pSocketClient, uint8_t Index, uint8_t MsgSize, void* Msg);
extern void SendRequestCRConnectCheck(CWsctlc* pSocketClient);
extern void SendRequestPetCommand(uint8_t PetType, uint8_t Command, uint16_t Key);
extern void SendRequestPetInfo(uint8_t PetType, uint8_t InvType, uint8_t nPos);
extern void SendChangeMServer(char* lpID, char* lpChr, int32_t iAuth1, int32_t iAuth2, int32_t iAuth3, int32_t iAuth4);
extern void SendRequestBCStatus();
extern void SendRequestBCReg();
extern void SendRequestBCGiveUp(uint8_t GiveUp);
extern void SendRequestBCRegInfo();
extern void SendRequestBCRegMark(int32_t ItemPos);
extern void SendRequestBCNPCBuy(int32_t NPCNumber, int32_t NPCIndex);
extern void SendRequestBCNPCRepair(int32_t NPCNumber, int32_t NPCIndex);
extern void SendRequestBCNPCUpgrade(int32_t NPCNumber, int32_t NPCIndex, int32_t NPCUpType, int32_t NPCUpValue);
extern void SendRequestBCGetTaxInfo();
extern void SendRequestBCChangeTaxRate(uint8_t TaxType, uint8_t TaxRate1, uint8_t TaxRate2, uint8_t TaxRate3, uint8_t TaxRate4);
extern void SendRequestBCWithdraw(uint8_t Money1, uint8_t Money2, uint8_t Money3, uint8_t Money4);
extern void SendRequestBCNPCList(uint8_t MonsterCode);
extern void SendRequestBCDeclareGuildList();
extern void SendRequestBCGuildList();
extern void SendRequestHuntZoneEnter(uint8_t p_HuntZoneEnter);
extern void SendGetCastleGuildMark();
extern void SendCastleHuntZoneEnter(int32_t p_Money);
extern void SendGateOparator(uint8_t p_Operator, int32_t p_Key);
extern void SendCatapultFire(uint8_t p_Key, uint8_t p_Index);
extern void SendWeaponExplosion(uint8_t p_KeyH, uint8_t p_KeyL);
extern void SendGuildCommand(uint8_t p_Team, uint8_t p_posX, uint8_t p_posY, uint8_t p_Cmd);
extern void SendRequestCrywolfInfo();
extern void SendRequestCrywolfAltarContract(uint16_t p_Key);
extern void SendRequestCrywolfBenefitPlusChaosRate();
extern void SendRequestKanturuInfo();
extern void SendRequestKanturuEnter();
extern void SendRequestWhiteAngelItem();
extern void SendRequestEnterOnWerwolf();
extern void SendRequestEnterOnGateKeeper();
extern void SendRequest3ColorHarvestItem();
extern void SendRequestSantaItem();
extern void SendRequestMoveDevias();
extern void SendRequestRaklionStateInfo();
extern void SendRequestCursedEnter(uint8_t mapnumber, uint8_t itempos);
extern void SendRequestCursedTempleMagic(uint16_t MagicNumber, int32_t TargetObjIndex, uint8_t Dis);
extern void SendRequestCursedTempleRequital();
extern void SendRequestMasterLevelSkill(int32_t SkillNum);
extern void SendRequestResetCharacterPoint();
extern void SendRequestRegistedLuckyCoin();
extern void SendRequestRegistLuckyCoin();
extern void SendRequestExChangeLuckyCoin(int32_t nCoinCnt);
extern void SendRequestEnterDoppelGangerEvent(uint8_t btItemPos);
extern void SendRequestEnterEmpireGuardianEvent();
extern void SendRequestEnterUnitedMarketPlaceEvent();
extern void SendRequestIGS_CashPointInfo();
extern void SendRequestIGS_CashShopOpen(uint8_t byShopOpenType);
extern void SendRequestIGS_BuyItem(int32_t lBuyItemPackageSeq, int32_t lBuyItemDisplaySeq, int32_t lBuyItemPriceSeq, uint16_t wItemCode, int32_t iCashType);
extern void SendRequestIGS_SendItemGift(int32_t lGiftItemPackageSeq, int32_t lGiftItemPriceSeq, int32_t lGiftItemDisplaySeq, int32_t lSaleZone, uint16_t wItemCode, int32_t iCashType, char* pstrReceiveUserID, char* pstrGiftMessage);
extern void SendRequestIGS_ItemStorageList(int32_t iPageIndex, char* szStorageType);
/*extern void SendRequestIGS_SendCashGift(dCashValue, pstrReceiveUserID, pstrGiftMessage)
{
	char strReceiveUserID[MAX_ID_SIZE+2];
	char strGiftMessage[MAX_GIFT_MESSAGE_SIZE+1];
	ZeroMemory( lpszID, MAX_ID_SIZE+2);
	ZeroMemory( lpszPass, MAX_GIFT_MESSAGE_SIZE+1);
	memcpy( strReceiveUserID, pstrReceiveUserID, MAX_ID_SIZE+1 );
	memcpy( strGiftMessage, pstrGiftMessage, MAX_GIFT_MESSAGE_SIZE);
	CStreamPacketEngine spe;
	spe.Init( 0xC1, 0xD2);
	spe << (uint8_t)0x07;
	spe << (double)dCashValue;
	spe.AddData( strReceiveUserID, MAX_ID_SIZE+1);
	spe.AddData( strGiftMessage, MAX_GIFT_MESSAGE_SIZE);
	spe.Send();
}*/
extern void SendRequestIGS_PossibleBuy();
extern void SendRequestIGS_LeftCountItem(int32_t lPackageSeq);
extern void SendRequestIGS_DeleteStorageItem(int32_t lStorageSeq, int32_t lStorageItemSeq, char* pstrStorageItemType);
extern void SendRequestIGS_UseStorageItem(int32_t lStorageSeq, int32_t lStorageItemSeq, uint16_t wItemCode, char* pstrStorageItemType);
extern void SendRequestIGS_EventItemList(int32_t lEventCategorySeq);
extern bool SendRequestEquippingInventoryItem(uint8_t iItemPos, uint8_t iValue);
extern void SendRequestStartHelper(uint8_t status);
extern void SendRequestSaveHelperSetting(uint8_t* buffer, int32_t Size);
extern void SendRequestRageAtt(int32_t p_Type, int32_t p_Key);
extern void SendRequestDarkside(uint16_t nSkill, int32_t p_Key);
extern void SendRequestRemoveJewelOfInventory(uint8_t btIndex, uint8_t btType, uint32_t btValue);
extern void SendRequestAddJewelOfInventory(uint8_t btIndex, uint8_t btType, uint32_t btValue);
extern void SendRequestMaxRanking();
extern void SendRequestRankingInfo(uint8_t p_Type);
extern void SendRequestRankingPlayer(int32_t characterIndex, const char* p_ID);
extern void SendRequestPurcharseMembership(uint32_t p_Type);
extern void CGSendOffTrade(int32_t p_Type);
extern void SendRequestMixIndex(uint8_t p_Index);
extern void SendRequestMixState(uint8_t p_Index);
extern void DataSend(uint8_t* byData, uint32_t size); // OK
