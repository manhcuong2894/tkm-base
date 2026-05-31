#pragma once
#include "wsctlc.h"
#include "SimpleModulus.h"
#include "CSMapServer.h"

#ifndef __SOCKETCLIENT_H__
#define __SOCKETCLIENT_H__
#define WM_ASYNCSELECTMSG							(WM_USER+0)
#define MAX_CHAT_SIZE								180
#define MAX_GUILDNAME								8
#define MAX_DEBUG_MAX								10

#define SIZE_PROTOCOLVERSION						( 5)
#define SIZE_PROTOCOLSERIAL							( 16)

#define REQUEST_CHARACTER_OFF_ATTK					0
#define REQUEST_CHARACTER_OFF_STOR					1

#define	REQUEST_JOIN_SERVER							0
#define RECEIVE_JOIN_SERVER_WAITING					1
#define	RECEIVE_JOIN_SERVER_SUCCESS					2
#define	RECEIVE_JOIN_SERVER_FAIL_VERSION			3
#define	REQUEST_CREATE_ACCOUNT						10
#define	RECEIVE_CREATE_ACCOUNT_SUCCESS				11
#define	RECEIVE_CREATE_ACCOUNT_FAIL_ID				12
#define	RECEIVE_CREATE_ACCOUNT_FAIL_RESIDENT		13
#define	REQUEST_LOG_IN								19
#define	RECEIVE_LOG_IN_SUCCESS						20
#define	RECEIVE_LOG_IN_FAIL_PASSWORD				21
#define	RECEIVE_LOG_IN_FAIL_ID						22
#define RECEIVE_LOG_IN_FAIL_ID_CONNECTED			23
#define RECEIVE_LOG_IN_FAIL_SERVER_BUSY				24
#define RECEIVE_LOG_IN_FAIL_ID_BLOCK				25
#define RECEIVE_LOG_IN_FAIL_VERSION					26
#define RECEIVE_LOG_IN_FAIL_CONNECT					27
#define RECEIVE_LOG_IN_FAIL_ERROR					28
#define RECEIVE_LOG_IN_FAIL_USER_TIME1				29
#define RECEIVE_LOG_IN_FAIL_USER_TIME2				30
#define RECEIVE_LOG_IN_FAIL_PC_TIME1				31
#define RECEIVE_LOG_IN_FAIL_PC_TIME2				32
#define RECEIVE_LOG_IN_FAIL_DATE					33
#define RECEIVE_LOG_IN_FAIL_POINT_DATE				34
#define RECEIVE_LOG_IN_FAIL_POINT_HOUR				35
#define RECEIVE_LOG_IN_FAIL_INVALID_IP				36
#define RECEIVE_LOG_IN_FAIL_NO_PAYMENT_INFO			37
#define RECEIVE_LOG_IN_FAIL_ONLY_OVER_15			38
#define RECEIVE_LOG_IN_FAIL_CHARGED_CHANNEL			39
#define	REQUEST_CHARACTERS_LIST						50
#define	RECEIVE_CHARACTERS_LIST						51
#define	REQUEST_CREATE_CHARACTER					52
#define	RECEIVE_CREATE_CHARACTER_SUCCESS			53
#define	RECEIVE_CREATE_CHARACTER_FAIL				54
#define	RECEIVE_CREATE_CHARACTER_FAIL2				55
#define	REQUEST_DELETE_CHARACTER					56
#define	RECEIVE_DELETE_CHARACTER_SUCCESS			57
#define REQUEST_JOIN_MAP_SERVER						60
#define RECEIVE_JOIN_MAP_SERVER						61
#define	RECEIVE_CONFIRM_PASSWORD_SUCCESS			62
#define	RECEIVE_CONFIRM_PASSWORD_FAIL_ID			63
#define	RECEIVE_CONFIRM_PASSWORD2_SUCCESS			64
#define	RECEIVE_CONFIRM_PASSWORD2_FAIL_ID			65
#define	RECEIVE_CONFIRM_PASSWORD2_FAIL_ANSWER		66
#define	RECEIVE_CONFIRM_PASSWORD2_FAIL_RESIDENT		67
#define RECEIVE_CHANGE_PASSWORD_SUCCESS				68
#define RECEIVE_CHANGE_PASSWORD_FAIL_ID				69
#define RECEIVE_CHANGE_PASSWORD_FAIL_RESIDENT		70
#define RECEIVE_CHANGE_PASSWORD_FAIL_PASSWORD		71
#define FIRST_CROWN_SWITCH_NUMBER					322
#define CLASS_SUMMONER_CARD							0x01
#define CLASS_DARK_LORD_CARD						0x02
#define CLASS_DARK_CARD								0x04
#define CLASS_RAGE_CARD								0x08
#define CLASS_CHARACTERCARD_TOTALCNT				4
#define PACKET_ITEM_LENGTH							12

#ifdef PROTO_EXTRA
#define EQUIPMENT_LENGTH							13
#else
#define EQUIPMENT_LENGTH							17
#endif
#define MAX_SPE_BUFFERSIZE_							( 16384)

extern int CurrentProtocolState;

typedef struct
{
	void set(uint8_t head, uint8_t size) // OK
	{
		this->Code = 0xC1;
		this->Size = size;
		this->HeadCode = head;
	}

	void setE(uint8_t head, uint8_t size) // OK
	{
		this->Code = 0xC3;
		this->Size = size;
		this->HeadCode = head;
	}

	void setSize(uint8_t size)
	{
		this->Size = (size);
	}

	uint8_t GetSize()
	{
		return this->Size;
	}

	uint8_t Code;
	uint8_t Size;
	uint8_t HeadCode;
} PBMSG_HEADER, * LPPBMSG_HEADER;

typedef struct
{
	void set(uint8_t head, uint8_t subh, uint8_t size) // OK
	{
		this->Code = 0xC1;
		this->Size = size;
		this->HeadCode = head;
		this->SubHeadCode = subh;
	}

	void setE(uint8_t head, uint8_t subh, uint8_t size) // OK
	{
		this->Code = 0xC3;
		this->Size = size;
		this->HeadCode = head;
		this->SubHeadCode = subh;
	}

	void setSize(uint8_t size)
	{
		this->Size = (size);
	}

	uint8_t GetSize()
	{
		return this->Size;
	}

	uint8_t Code;
	uint8_t Size;
	uint8_t HeadCode;
	uint8_t SubHeadCode;
} PSBMSG_HEADER, * LPPSBMSG_HEADER;

typedef struct
{
	void set(uint8_t head, uint16_t size) // OK
	{
		this->Code = 0xC2;
		this->SizeH = HIBYTE(size);
		this->SizeL = LOBYTE(size);
		this->HeadCode = head;
	}

	void setE(uint8_t head, uint16_t size) // OK
	{
		this->Code = 0xC4;
		this->SizeH = HIBYTE(size);
		this->SizeL = LOBYTE(size);
		this->HeadCode = head;
	}

	void setSize(uint16_t size)
	{
		this->SizeH = HIBYTE(size);
		this->SizeL = LOBYTE(size);
	}

	uint16_t GetSize()
	{
		return ((uint16_t)(this->SizeH) << 8) + this->SizeL;
	}

	uint8_t Code;
	uint8_t SizeH;
	uint8_t SizeL;
	uint8_t HeadCode;
} PWMSG_HEADER, * LPPWMSG_HEADER;

typedef struct
{
	void set(uint8_t head, uint8_t subh, uint16_t size) // OK
	{
		this->Code = 0xC2;
		this->SizeH = HIBYTE(size);
		this->SizeL = LOBYTE(size);
		this->HeadCode = head;
		this->SubHeadCode = subh;
	}

	void setE(uint8_t head, uint8_t subh, uint16_t size) // OK
	{
		this->Code = 0xC4;
		this->SizeH = HIBYTE(size);
		this->SizeL = LOBYTE(size);
		this->HeadCode = head;
		this->SubHeadCode = subh;
	}

	void setSize(uint16_t size)
	{
		this->SizeH = HIBYTE(size);
		this->SizeL = LOBYTE(size);
	}

	uint16_t GetSize()
	{
		return ((uint16_t)(this->SizeH) << 8) + this->SizeL;
	}

	uint8_t Code;
	uint8_t SizeH;
	uint8_t SizeL;
	uint8_t HeadCode;
	uint8_t SubHeadCode;
} PSWMSG_HEADER, * LPSWMSG_HEADER;



typedef struct
{
	uint8_t Code;
	uint8_t Size;
	uint8_t byBuffer[255];
} PBMSG_ENCRYPTED, * LPPBMSG_ENCRYPTED;

typedef struct
{
	uint8_t Code;
	uint8_t SizeH;
	uint8_t SizeL;
	uint8_t byBuffer[MAX_SPE_BUFFERSIZE_];
} PWMSG_ENCRYPTED, * LPWBMSG_ENCRYPTED;

//request default SubCode
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
} PREQUEST_DEFAULT_SUBCODE, * LPPREQUEST_DEFAULT_SUBCODE;

//receive default
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         Value;
} PHEADER_DEFAULT, * LPPHEADER_DEFAULT;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         result;
	uint16_t         btStatValue;
	uint8_t         btFruitType;
#ifdef PROTO_EXTRA
	DWORD        btValue;
	DWORD        btLevelUpPoint;
	DWORD        btStrength;
	DWORD        btDexterity;
	DWORD        btVitality;
	DWORD        btEnergy;
	DWORD        btCharisma;
#endif // PROTO_EXTRA

} PMSG_USE_STAT_FRUIT, * LPPMSG_USE_STAT_FRUIT;

//receive default subcode
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         Value;
} PHEADER_DEFAULT_SUBCODE, * LPPHEADER_DEFAULT_SUBCODE;

//receive Character List 
typedef struct
{
	PSWMSG_HEADER Header;
	uint8_t         ClassCode;
	uint8_t         MoveCount;
	uint8_t         MaxClass;
	uint8_t         ExtWarehouse;
} PHEADER_DEFAULT_CHARACTER_LIST, * LPPHEADER_DEFAULT_CHARACTER_LIST;

typedef struct
{
	PSWMSG_HEADER Header;
	int           index;
	char          rankname[20];
	char          col1[20];
	char          col2[20];
	int           count;
} PHEADER_RANKING_LIST, * LPPHEADER_RANKING_LIST;

typedef struct
{
	char          Name[20];
	uint8_t          PlayerClass;
	uint8_t          LevelVip;
	int           TotalScore;
}PCREATE_RANKING_INFO, * LPPCREATE_RANKING_INFO;

typedef struct
{
	PSBMSG_HEADER Header; // C1:BF:51
	int           characterIndex;
	uint32_t      Equipment[EQUIPMENT_LENGTH];
}PHEADER_RANKING_CHARACTER, * LPPHEADER_RANKING_CHARACTER;


typedef struct
{
	PBMSG_HEADER	header;
	uint8_t			Flag;
	uint8_t			CharacterCard;
} PHEADER_CHARACTERCARD, * LPPHEADER_CHARACTERCARD;

typedef struct
{
	bool bCharacterEnable[CLASS_CHARACTERCARD_TOTALCNT];
}CHARACTER_ENABLE, * LPCHARACTER_ENABLE;

extern CHARACTER_ENABLE g_CharCardEnable;

//receive default key
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         KeyH;
	uint8_t         KeyL;
} PHEADER_DEFAULT_KEY, * LPPHEADER_DEFAULT_KEY;

//receive default key
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         Value;
	uint8_t         KeyH;
	uint8_t         KeyL;
} PHEADER_DEFAULT_VALUE_KEY, * LPPHEADER_DEFAULT_VALUE_KEY;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         Value;
	uint8_t         KeyH;
	uint8_t         KeyM;
	uint8_t         KeyL;
} PHEADER_MATCH_OPEN_VALUE, * LPPHEADER_MATCH_OPEN_VALUE;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t		State;
	uint8_t		KeyH;
	uint8_t		KeyL;
	uint8_t		BuffIndex;
} PMSG_VIEWSKILLSTATE, * LPPMSG_VIEWSKILLSTATE;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint16_t         time;
	int          money;
	uint8_t         result;
} PMSG_ANS_HELPER_START, * LPPMSG_ANS_HELPER_START;

typedef struct PMSG_CHARACTER_BASE_INFO
{
	PSBMSG_HEADER Header;
	DWORD AttackSpeed;
	DWORD MagicSpeed;
} *LPPMSG_CHARACTER_BASE_INFO;

typedef struct
{
	PSBMSG_HEADER Header;
	uint8_t          KeyH;
	uint8_t          KeyL;
	DWORD         MaxHp;
	DWORD         CurHp;
}PMSG_CHARACTER_LIFE_INFO,*LPPMSG_CHARACTER_LIFE_INFO;

typedef struct
{
	PWMSG_HEADER Header;
	uint8_t         result;
	uint8_t         data[256];
}PMSG_HELPER_SETTING, * LPPMSG_HELPER_SETTING;

typedef struct
{
	PSBMSG_HEADER header; // C3:22
	uint8_t ExtInventory;
	DWORD PriceInventoryExt;
}PMSG_CREATE_INVENTARIO_SEND, * LPPMSG_CREATE_INVENTARIO_SEND;

typedef struct
{
	int MonsterIndex;
	int Monster_Kill;
	int MonsterCount;
} INVASION_DEFAULT_MONSTER, * LP_INVASION_DEFAULT_MONSTER;

typedef struct
{
	PSWMSG_HEADER header; // C3:22
	int Index;
	char Name[64];
	int RemainTime;
	int count;
}PMSG_INVASION_DEFAULT_INFO, * LPPMSG_INVASION_DEFAULT_INFO;

typedef struct
{
	PSWMSG_HEADER header; // C3:22
	int Index;
	int MonsterIndex;
	int Monster_Kill;
	int MonsterCount;
}PMSG_INVASION_KILL_INFO, * LPPMSG_INVASION_KILL_INFO;

typedef struct
{
	PSWMSG_HEADER header; // C3:22
	int CurReset;
	int MaxReset;
	int MinLevel;
	DWORD ReqMoney;
	int RewardItem;
	int RewardPoint;
	int ItemIndex[10];
	int ItemCount[10];
}PMSG_RESET_TABLE_INFO, * LPPMSG_RESET_TABLE_INFO;

typedef struct
{
	PSWMSG_HEADER header; // C3:22
	int CurMasterReset;
	int MaxMasterReset;
	int MinLevel;
	int MinReset;
	DWORD ReqMoney;
	int RewardItem;
	int RewardPoint;
	int ItemIndex[10];
	int ItemCount[10];
}PMSG_MASTER_RESET_TABLE_INFO, * LPPMSG_MASTER_RESET_TABLE_INFO;

typedef struct
{
	PSWMSG_HEADER header; // C3:22
	int CurReset;
}PMSG_RESET_TABLE_STATE, * LPPMSG_RESET_TABLE_STATE;

//receive default(word)
typedef struct
{
	PWMSG_HEADER Header;
	uint8_t         Value;
} PWHEADER_DEFAULT_WORD, * LPPWHEADER_DEFAULT_WORD;

//receive default(word)
typedef struct
{
	PWMSG_HEADER Header;
	INT          Value;
} PWHEADER_DEFAULT_WORD2, * LPPWHEADER_DEFAULT_WORD2;

//receive default subcode(word)
typedef struct
{
	PWMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         Value;
} PHEADER_DEFAULT_SUBCODE_WORD, * LPPHEADER_DEFAULT_SUBCODE_WORD;


typedef struct
{
	PSWMSG_HEADER Header;
	DWORD         Size;
	DWORD         TotalSize;
	DWORD         Offset;
	uint16_t          FileVersion;
} PHEADER_SERVER_VERSION, * LPPHEADER_SERVER_VERSION;

typedef struct
{
	PSWMSG_HEADER Header;
	uint16_t          state;
	uint16_t          Version;
} PHEADER_SERVER_UPDATE_VERSION, * LPPHEADER_SERVER_UPDATE_VERSION;

typedef struct
{
	PBMSG_HEADER Header;
	uint16_t        Value;
} PHEADER_DEFAULT_WORD, * LPPHEADER_DEFAULT_WORD;

typedef struct
{
	PBMSG_HEADER Header;
	DWORD        Value;
} PHEADER_DEFAULT_DWORD, * LPPHEADER_DEFAULT_DWORD;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t        byBuffer[1024];
} PHEADER_DEFAULT_CUSTOM, * LPPHEADER_DEFAULT_CUSTOM;

typedef struct
{
	PBMSG_HEADER Header;
	DWORD        nGold;
	DWORD        nCoin1;
	DWORD        nCoin2;
	DWORD        nCoin3;
} PHEADER_TRADE_MONEY, * LPPHEADER_TRADE_MONEY;

typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          Index;
	uint8_t          Item[PACKET_ITEM_LENGTH];
} PHEADER_DEFAULT_ITEM, * LPPHEADER_DEFAULT_ITEM;

typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          SubCode;
	uint8_t          Index;
	uint8_t          Item[PACKET_ITEM_LENGTH];
} PHEADER_DEFAULT_SUBCODE_ITEM, * LPPHEADER_DEFAULT_SUBCODE_ITEM;

typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          SubCode;
	uint8_t          ID[MAX_ID_SIZE];
	uint8_t          Item[PACKET_ITEM_LENGTH];
} PHEADER_DEFAULT_SUBCODE_ITEM_ID, * LPPHEADER_DEFAULT_SUBCODE_ITEM_ID;

//send trade
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          SrcFlag;
	uint8_t          SrcIndex;
	uint8_t          Item[PACKET_ITEM_LENGTH];
	uint8_t          DstFlag;
	uint8_t          DstIndex;
} PSEND_TRADE, * LPSEND_TRADE;

///////////////////////////////////////////////////////////////////////////////
// log in
///////////////////////////////////////////////////////////////////////////////

typedef struct
{
	uint16_t         Index;
	uint8_t         Percent;
	uint8_t         Type;
	char         ServerName[32];
} PRECEIVE_SERVER_LIST, * LPPRECEIVE_SERVER_LIST;

//receive join server
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         Result;
	uint8_t         NumberH;
	uint8_t         NumberL;
	uint8_t         Version[SIZE_PROTOCOLVERSION];
} PRECEIVE_JOIN_SERVER, * LPPRECEIVE_JOIN_SERVER;

//request create acoount
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         ID[MAX_ID_SIZE];
	uint8_t         Password[MAX_ID_SIZE];
	uint8_t         Name[10];
	uint8_t         ResidentNumber[13];
	uint8_t         Quiz[30];
	uint8_t         Answer[30];
	uint8_t         PhoneNumber[20];
	uint8_t         Email[50];
} PREQUEST_CREATE_ACCOUNT, * LPPREQUEST_CREATE_ACCOUNT;

//request log in
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         ID[MAX_ID_SIZE];
	uint8_t         Password[MAX_ID_SIZE];
	DWORD        Time;
	uint8_t         Version[SIZE_PROTOCOLVERSION];
	uint8_t         Serial[SIZE_PROTOCOLSERIAL];
} PREQUEST_LOG_IN, * LPPREQUEST_LOG_IN;

//request confirm password
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         ID[MAX_ID_SIZE];
} PREQUEST_CONFIRM_PASSWORD, * LPPREQUEST_CONFIRM_PASSWORD;

//receive confirm password
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         Result;
	uint8_t         Question[30];
} PRECEIVE_CONFIRM_PASSWORD, * LPPRECEIVE_CONFIRM_PASSWORD;

//request confirm password
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         ID[MAX_ID_SIZE];
	uint8_t         ResidentNumber[13];
	uint8_t         Answer[30];
} PREQUEST_CONFIRM_PASSWORD2, * LPPREQUEST_CONFIRM_PASSWORD2;

//receive confirm password
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         Result;
	uint8_t         Password[MAX_ID_SIZE];
} PRECEIVE_CONFIRM_PASSWORD2, * LPPRECEIVE_CONFIRM_PASSWORD2;

//request change password
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         ID[MAX_ID_SIZE];
	uint8_t         OldPassword[MAX_ID_SIZE];
	uint8_t         NewPassword[MAX_ID_SIZE];
	uint8_t         ResidentNumber[13];
} PREQUEST_CHANGE_PASSWORD, * LPPREQUEST_CHANGE_PASSWORD;

//receive characters list
typedef struct
{
	uint8_t  Index;
	char  ID[MAX_ID_SIZE];
	uint16_t  Level;
	uint8_t  CtlCode;
	uint8_t  Class;
	uint32_t Equipment[EQUIPMENT_LENGTH];
	uint8_t  byGuildStatus;
#ifdef PROTO_EXTRA
	uint32_t Reset;
	uint32_t MasterReset;
#endif // PROTO_EXTRA
} PRECEIVE_CHARACTER_LIST, * LPPRECEIVE_CHARACTER_LIST;

//request create character
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         ID[MAX_ID_SIZE];
	uint8_t         Class;
} PREQUEST_CREATE_CHARACTER, * LPREQUEST_CREATE_CHARACTER;

//receive create character
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         Result;
	char         ID[MAX_ID_SIZE];
	uint8_t         Index;
	uint16_t         Level;
	uint8_t		 Class;
	//uint8_t         Equipment[24];
} PRECEIVE_CREATE_CHARACTER, * LPPRECEIVE_CREATE_CHARACTER;

//request delete character
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         ID[MAX_ID_SIZE];
	uint8_t         Resident[10];
} PREQUEST_DELETE_CHARACTER, * LPREQUEST_DELETE_CHARACTER;

//request join map server
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t		 SubCode;
	uint8_t         ID[MAX_ID_SIZE];
} PREQUEST_JOIN_MAP_SERVER, * LPPREQUEST_JOIN_MAP_SERVER;

//receive join map server
typedef struct
{
	PSBMSG_HEADER Header;
	uint8_t PositionX;
	uint8_t PositionY;
	uint8_t Map;
	uint8_t Angle;
	uint8_t btMExp[8];
	uint8_t btMBaseExp[8];
	uint8_t btMNextExp[8];

	uint32_t LevelUpPoint;
	uint32_t Strength;
	uint32_t Dexterity;
	uint32_t Vitality;
	uint32_t Energy;
	uint32_t Charisma;

	uint32_t Life;
	uint32_t LifeMax;
	uint32_t Mana;
	uint32_t ManaMax;
	uint32_t Shield;
	uint32_t ShieldMax;
	uint32_t SkillMana;
	uint32_t SkillManaMax;

	uint32_t Gold;
	uint8_t PK;
	uint8_t CtlCode;
	uint16_t AddPoint;
	uint16_t MaxAddPoint;
	uint16_t wMinusPoint;
	uint16_t wMaxMinusPoint;
	uint8_t ExtInventory;
	uint8_t AccountLevel;
#ifdef PROTO_EXTRA
	uint32_t btReset;
	uint32_t MaxCharacterLevel;
	uint32_t PriceInventoryExt;
#endif // PROTO_EXTRA
	uint32_t PenaltyTime;
} PRECEIVE_JOIN_MAP_SERVER, * LPPRECEIVE_JOIN_MAP_SERVER;

#ifdef PROTO_EXTRA
typedef struct
{
	PSBMSG_HEADER Header;
	uint16_t Level;
	uint32_t Reset;
	uint32_t LevelPoint;
	uint32_t Strength;
	uint32_t Dexterity;
	uint32_t Vitality;
	uint32_t Energy;
	uint32_t Charisma;

	uint32_t Life;
	uint32_t LifeMax;
	uint32_t Mana;
	uint32_t ManaMax;
	uint32_t SkillMana;
	uint32_t SkillManaMax;
	uint32_t Shield;
	uint32_t ShieldMax;

	uint32_t MinusPoint;
	uint32_t MaxMinusPoint;
	uint32_t wMinusPoint;
	uint32_t wMaxMinusPoint;

	uint8_t btMExp[8];
	uint8_t btMBaseExp[8];
	uint8_t btMNextExp[8];
} PRECEIVE_NEW_JOIN_MAP_SERVER, * LPPRECEIVE_NEW_JOIN_MAP_SERVER;

typedef struct
{
	PSBMSG_HEADER Header;
	DWORD         Life;
	DWORD         LifeMax;
	DWORD         Mana;
	DWORD         ManaMax;
	DWORD         SkillMana;
	DWORD         SkillManaMax;
	DWORD         Shield;
	DWORD         ShieldMax;
	DWORD         AddStrength;
	DWORD         AddDexterity;
	DWORD         AddVitality;
	DWORD         AddEnergy;
	DWORD         AddCharisma;
	//--
	DWORD         btPhysiDamageMin;
	DWORD         btPhysiDamageMax;
	DWORD         btMagicDamageMin;
	DWORD         btMagicDamageMax;
	DWORD         btCurseDamageMin;
	DWORD         btCurseDamageMax;
	DWORD         btMulPhysiDamage;
	DWORD         btDivPhysiDamage;
	DWORD         btMulMagicDamage;
	DWORD         btDivMagicDamage;
	DWORD         btMulCurseDamage;
	DWORD         btDivCurseDamage;
	DWORD         btMagicDamageRate;
	DWORD         btCurseDamageRate;
	DWORD         btPhysiSpeed;
	DWORD         btMagicSpeed;
	DWORD         btAttackSuccessRate;
	DWORD         btAttackSuccessRatePvP;
	DWORD         btDefense;
	DWORD         btDefenseSuccessRate;
	DWORD         btDefenseSuccessRatePvP;
	DWORD         btDamageMultiplier;
	DWORD         btRFDamageMultiplierA;
	DWORD         btRFDamageMultiplierB;
	DWORD         btRFDamageMultiplierC;
	DWORD         btDarkSpiritAttackDamageMin;
	DWORD         btDarkSpiritAttackDamageMax;
	DWORD         btDarkSpiritAttackSpeed;
	DWORD         btDarkSpiritAttackSuccessRate;
	DWORD         mtGearPower;
} PRECEIVE_DEFAULT_CHARACTER, * LPPRECEIVE_DEFAULT_CHARACTER;
#endif // PROTO_EXTRA

//receive revival
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         PositionX;
	uint8_t         PositionY;
	uint8_t         Map;
	uint8_t         Angle;
	DWORD        Life;
	DWORD        Mana;
	DWORD        Shield;
	DWORD        SkillMana;
	uint8_t         btMExp[8];
	DWORD        Gold;
} PRECEIVE_REVIVAL, * LPPRECEIVE_REVIVAL;

//inventory
typedef struct
{
	uint8_t          Index;
	uint8_t          Item[PACKET_ITEM_LENGTH];
} PRECEIVE_INVENTORY, * LPPRECEIVE_INVENTORY;

//inventory
typedef struct
{
	PSBMSG_HEADER Header;
	uint8_t          btSlot;
} PRECEIVE_NPCSHOP_ITEM_BUY, * LPPRECEIVE_NPCSHOP_ITEM_BUY;

typedef struct
{
	PSBMSG_HEADER Header;
	DWORD         Coin[3];
} PRECEIVE_CHARACTER_COIN, * LPPRECEIVE_CHARACTER_COIN;

//inventory
typedef struct
{
	PSBMSG_HEADER Header;
	uint16_t      Lock;
	uint16_t      AccountLevel;
	char          AccountExpireDate[20];
} PRECEIVE_CHARACTER_SERVER_INFO, *LPPRECEIVE_CHARACTER_SERVER_INFO;

typedef struct
{
	uint8_t Inventory[12];
}PRECEIVE_INVENTORY_HOLY;
///////////////////////////////////////////////////////////////////////////////
// trade
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t      SubCode;
	uint8_t      ID[MAX_ID_SIZE];
	uint16_t     Level;
	uint32_t     GuildKey;
} PTRADE, * LPPTRADE;


///////////////////////////////////////////////////////////////////////////////
// game
///////////////////////////////////////////////////////////////////////////////

//request chat
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t      Name[MAX_ID_SIZE];
	uint8_t      ChatText[MAX_CHAT_SIZE];
} PCHATING, * LPPCHATING;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t KeyH;
	uint8_t KeyL;
	uint8_t ChatText[MAX_CHAT_SIZE];
} PCHATING_KEY, * LPPCHATING_KEY;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t  Result;
	uint8_t  Count;
	uint16_t Delay;
	uint32_t Color;
	uint8_t  Speed;
	uint8_t  Notice[256];
} PRECEIVE_NOTICE, * LPPRECEIVE_NOTICE;

//receive equipment
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t  SubCode;
	uint8_t  KeyH;
	uint8_t  KeyL;
	uint8_t  Class;
	uint32_t Equipment[EQUIPMENT_LENGTH];
} PRECEIVE_EQUIPMENT, * LPPRECEIVE_EQUIPMENT;

//receive other map character
typedef struct
{
	uint8_t  KeyH;
	uint8_t  KeyL;
	uint8_t  PositionX;
	uint8_t  PositionY;
	uint8_t  ServerClass;
	uint8_t  ActionPose;
	uint8_t  Name[MAX_ID_SIZE];
	uint32_t Equipment[EQUIPMENT_LENGTH];
	uint32_t MaxHP;
	uint32_t CurHP;
	uint8_t  TX;
	uint8_t  TY;
	uint8_t  dir_and_pk_level;
	uint8_t  s_BuffCount;
	uint8_t  s_BuffEffectState[MAX_BUFF_SLOT_INDEX];
} PCREATE_CHARACTER, * LPPCREATE_CHARACTER;

//receive other map character
typedef struct
{
	uint8_t  KeyH;
	uint8_t  KeyL;
	uint8_t  PositionX;
	uint8_t  PositionY;
	uint8_t  TypeH;
	uint8_t  TypeL;
	uint8_t  Name[MAX_ID_SIZE];
	uint8_t  ServerClass;
	uint8_t  ActionPose;
	uint32_t Equipment[EQUIPMENT_LENGTH];
	uint32_t MaxHP;
	uint32_t CurHP;
	uint8_t  TX;
	uint8_t  TY;
	uint8_t  dir_and_pk_level;
	uint8_t  s_BuffCount;
	uint8_t  s_BuffEffectState[MAX_BUFF_SLOT_INDEX];
} PCREATE_TRANSFORM, * LPPCREATE_TRANSFORM;

//receive other map character
typedef struct
{
	uint8_t KeyH;
	uint8_t KeyL;
	uint8_t TypeH;
	uint8_t TypeL;
	uint8_t PositionX;
	uint8_t PositionY;
	uint8_t TargetX;
	uint8_t TargetY;
	uint8_t Path;
	uint8_t ID[MAX_ID_SIZE];
	uint8_t s_BuffCount;
	uint8_t s_BuffEffectState[MAX_BUFF_SLOT_INDEX];
} PCREATE_SUMMON, * LPPCREATE_SUMMON;

//receive other map character
typedef struct
{
	uint8_t  KeyH;
	uint8_t  KeyL;
	uint8_t  TypeH;
	uint8_t  TypeL;
	uint8_t  PositionX;
	uint8_t  PositionY;
	uint8_t  TargetX;
	uint8_t  TargetY;
	uint8_t  Path;
	uint32_t MaxHP;
	uint32_t CurHP;
	uint8_t  s_BuffCount;
	uint8_t  s_BuffEffectState[MAX_BUFF_SLOT_INDEX];
} PCREATE_MONSTER, * LPPCREATE_MONSTER;

//send move my character
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t      PositionX;
	uint8_t      PositionY;
	uint8_t      Path[8];
} PMOVE_MY_CHARACTER, * LPPMOVE_MY_CHARACTER;

//receive move character
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          KeyH;
	uint8_t          KeyL;
	uint8_t          PositionX;
	uint8_t          PositionY;
	uint8_t          Path[1];
} PMOVE_CHARACTER, * LPPMOVE_CHARACTER;

//delete character and item
typedef struct
{
	uint8_t         KeyH;
	uint8_t         KeyL;
} PDELETE_CHARACTER, * LPPDELETE_CHARACTER;

//create item
typedef struct
{
	uint8_t          KeyH;
	uint8_t          KeyL;
	uint8_t          PositionX;
	uint8_t          PositionY;
	uint8_t          Item[PACKET_ITEM_LENGTH];
} PCREATE_ITEM, * LPPCREATE_ITEM;

//request drop item
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          PositionX;
	uint8_t          PositionY;
	uint8_t          InventoryIndex;
} PREQUEST_DROP_ITEM, * LPPREQUEST_DROP_ITEM;

//request drop item
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          SrcIndex;
	uint8_t          DstIndex;
} PREQUEST_EQUIPMENT_ITEM, * LPPREQUEST_EQUIPMENT_ITEM;

//change character
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          KeyH;
	uint8_t          KeyL;
	uint8_t          Item[PACKET_ITEM_LENGTH];
	uint8_t          iSrcIndex;

} PCHANGE_CHARACTER, * LPPCHANGE_CHARACTER;

typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          KeyH;
	uint8_t          KeyL;
	uint8_t          Action;
	uint8_t          Dir;
} PREQUEST_ATTACK, * LPPREQUEST_ATTACK;

//receive get item
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          Result;
	uint8_t          Item[PACKET_ITEM_LENGTH];
} PRECEIVE_GET_ITEM, * LPPRECEIVE_GET_ITEM;

//receive attack
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          KeyH;
	uint8_t          KeyL;
	uint8_t          DamageH;
	uint8_t          DamageL;
	uint8_t		  DamageType;
	uint8_t		  ShieldDamageH;
	uint8_t		  ShieldDamageL;
#ifdef PROTO_EXTRA
	DWORD         btLife;
	DWORD         btShield;
	DWORD         Damage;
	DWORD         ShieldDamage;
#endif
} PRECEIVE_ATTACK, * LPPRECEIVE_ATTACK;

//receive die
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          KeyH;
	uint8_t          KeyL;
	uint8_t          ExpH;
	uint8_t          ExpL;
	uint8_t          DamageH;
	uint8_t          DamageL;
#ifdef PROTO_EXTRA
	DWORD         Damage;
#endif
} PRECEIVE_DIE, * LPPRECEIVE_DIE;
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          KeyH;
	uint8_t          KeyL;
	uint16_t          ExpH;
	uint16_t          ExpL;
	uint8_t          DamageH;
	uint8_t          DamageL;
#ifdef PROTO_EXTRA
	DWORD         Damage;
#endif
} PRECEIVE_DIE2, * LPPRECEIVE_DIE2;

//receive default key
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         KeyH;
	uint8_t         KeyL;
	uint8_t         MagicH;
	uint8_t         MagicL;
	uint8_t         TKeyH;
	uint8_t         TKeyL;
} PHEADER_DEFAULT_DIE, * LPPHEADER_DEFAULT_DIE;

//request action
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          Angle;
	uint8_t          Action;
	uint8_t          TargetKeyH;
	uint8_t          TargetKeyL;
} PREQUEST_ACTION, * LPPREQUEST_ACTION;

//receive action
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          KeyH;
	uint8_t          KeyL;
	uint8_t          Angle;
	uint8_t          Action;
	uint8_t		  TargetKeyH;
	uint8_t		  TargetKeyL;
} PRECEIVE_ACTION, * LPPRECEIVE_ACTION;

//send magic
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          MagicH;
	uint8_t          MagicL;
	uint8_t          KeyH;
	uint8_t          KeyL;
} PSEND_MAGIC, * LPPSEND_MAGIC;

//send magic continue
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          MagicH;
	uint8_t          MagicL;
	uint8_t          PositionX;
	uint8_t          PositionY;
	uint8_t          Angle;
} PSEND_MAGIC_CONTINUE, * LPPSEND_MAGIC_CONTINUE;

//receive magic continue
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          MagicH;
	uint8_t          MagicL;
	uint8_t          KeyH;
	uint8_t          KeyL;
	uint8_t          PositionX;
	uint8_t          PositionY;
	uint8_t          Angle;
} PRECEIVE_MAGIC_CONTINUE, * LPPRECEIVE_MAGIC_CONTINUE;

//receive magic
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          MagicH;
	uint8_t          MagicL;
	uint8_t          SourceKeyH;
	uint8_t          SourceKeyL;
	uint8_t          TargetKeyH;
	uint8_t          TargetKeyL;
} PRECEIVE_MAGIC, * LPPRECEIVE_MAGIC;

//receive MonsterSkill
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          MagicH;
	uint8_t          MagicL;
	uint16_t          SourceKey;
	uint16_t          TargetKey;
} PRECEIVE_MONSTERSKILL, * LPPRECEIVE_MONSTERSKILL;

//receive magic target
typedef struct
{
	uint8_t          KeyH;
	uint8_t          KeyL;
} PRECEIVE_MAGIC_POSITION, * LPPRECEIVE_MAGIC_POSITION;

//send magic continue
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         MagicH;
	uint8_t         MagicL;
	uint8_t         PositionX;
	uint8_t         PositionY;
	uint8_t         Count;
	uint8_t         Key[10];
} PSEND_MAGIC_CONTINUES, * LPPSEND_MAGIC_CONTINUES;

//receive magic target
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         KeyH;
	uint8_t         KeyL;
	uint8_t         MagicH;
	uint8_t         MagicL;
	uint8_t         PositionX;
	uint8_t         PositionY;
	uint8_t         Count;
} PRECEIVE_MAGIC_POSITIONS, * LPPRECEIVE_MAGIC_POSITIONS;

//receive magic list count
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         Value;
	uint8_t         ListType;
} PHEADER_MAGIC_LIST_COUNT, * LPPHEADER_MAGIC_LIST_COUNT;

#pragma pack(push, 1)
//receive magic target
typedef struct
{
	uint8_t          Index;

	uint16_t          Type;
	uint8_t          Level;
} PRECEIVE_MAGIC_LIST, * LPPRECEIVE_MAGIC_LIST;
#pragma pack(pop)

//receive skill count.
typedef	struct
{
	PBMSG_HEADER  Header;
	uint8_t          KeyH;
	uint8_t          KeyL;
	uint8_t		  m_byType;
	uint8_t		  m_byCount;
} PRECEIVE_EX_SKILL_COUNT, * LPPRECEIVE_EX_SKILL_COUNT;

//receive gold
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          Flag;
	DWORD         Gold;
} PRECEIVE_GOLD, * LPPRECEIVE_GOLD;

//receive repair gold
typedef struct
{
	PBMSG_HEADER  Header;
	DWORD         Gold;
} PRECEIVE_REPAIR_GOLD, * LPPRECEIVE_REPAIR_GOLD;

//receive level up
typedef struct
{
	PSBMSG_HEADER Header;
	uint32_t Level;
	uint32_t LevelPoint;
	uint32_t LifeMax;
	uint32_t ManaMax;
	uint32_t ShieldMax;
	uint32_t SkillManaMax;
	uint32_t Experience;
	uint32_t BaseExperience;
	uint32_t NextExperience;
	uint16_t AddPoint;
	uint16_t MaxAddPoint;
	uint16_t MinusPoint;
	uint16_t MaxMinusPoint;
} PRECEIVE_LEVEL_UP, * LPPRECEIVE_LEVEL_UP;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t type;
	uint32_t curAtt1;
	uint32_t curAtt2;
} PRECEIVE_LIFE, * LPPRECEIVE_LIFE;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t type;
	uint32_t curAtt1;
	uint32_t curAtt2;
} PRECEIVE_MANA, * LPPRECEIVE_MANA;

//receive add point
typedef struct
{
	PSBMSG_HEADER Header;
	uint8_t Result;
	uint32_t LevelPoint;
	uint32_t LifeMax;
	uint32_t ManaMax;
	uint32_t ShieldMax;
	uint32_t SkillManaMax;
	uint32_t Strength;
	uint32_t Dexterity;
	uint32_t Vitality;
	uint32_t Energy;
	uint32_t Charisma;
} PRECEIVE_ADD_POINT, * LPPRECEIVE_ADD_POINT;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         KeyH;
	uint8_t         KeyL;
	uint8_t         PositionX;
	uint8_t         PositionY;
} PRECEIVE_MOVE_POSITION, * LPPRECEIVE_MOVE_POSITION;

typedef struct
{
	PBMSG_HEADER Header;
	uint16_t         Flag;
	uint8_t         Map;
	uint8_t         PositionX;
	uint8_t         PositionY;
	uint8_t         Angle;
} PRECEIVE_TELEPORT_POSITION, * LPPRECEIVE_TELEPORT_POSITION;

//receive damage
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         DamageH;
	uint8_t         DamageL;
	uint8_t         ShieldDamageH;
	uint8_t         ShieldDamageL;
#ifdef PROTO_EXTRA
	DWORD        btLife;
	DWORD        btShield;
	DWORD        btDamage;
	DWORD        btShieldDamage;
#endif // PROTO_EXTRA
} PRECEIVE_DAMAGE, * LPPRECEIVE_DAMAGE;

//receive create guild master
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         Name[8];
	uint8_t         Mark[32];
} PSEND_CREATE_GUILD, * LPPSEND_CREATE_GUILD;

//receive party info
typedef struct
{
	uint8_t         value;
} PRECEIVE_PARTY_INFO, * LPPRECEIVE_PARTY_INFO;

//receive party infos
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          Count;
} PRECEIVE_PARTY_INFOS, * LPPRECEIVE_PARTY_INFOS;

//receive party list
typedef struct
{
	uint8_t         ID[MAX_ID_SIZE];
	uint8_t         Number;
	uint8_t         Map;
	uint8_t         x;
	uint8_t         y;
	int          currHP;
	int          maxHP;
} PRECEIVE_PARTY_LIST, * LPPRECEIVE_PARTY_LIST;

//receive party list
typedef struct
{
	PSWMSG_HEADER Header;
	uint8_t          Count;
} PRECEIVE_PARTY_LISTS, * LPPRECEIVE_PARTY_LISTS;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         KeyH;
	uint8_t         KeyL;
	int			 ItemInfo;
	uint8_t		 ItemLevel;
}PRECEIVE_GETITEMINFO_FOR_PARTY, * LPPRECEIVE_GETITEMINFO_FOR_PARTY;

//receive pk
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         KeyH;
	uint8_t         KeyL;
	uint8_t         PK;
	DWORD        PenaltyTime;
} PRECEIVE_PK, * LPPRECEIVE_PK;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         Index;
	uint16_t         Time;
} PRECEIVE_HELPER_ITEM, * LPPRECEIVE_HELPER_ITEM;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         IP[15];
	uint16_t         Port;
} PRECEIVE_SERVER_ADDRESS, * LPPRECEIVE_SERVER_ADDRESS;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         Server;
} PRECEIVE_SERVER_BUSY, * LPPRECEIVE_SERVER_BUSY;

typedef struct
{
	uint8_t         KeyH;
	uint8_t         KeyL;
	uint8_t         Name[8];
	uint8_t         Mark[32];
} PRECEIVE_GUILD, * LPPRECEIVE_GUILD;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         KeyH;
	uint8_t         KeyL;
	uint8_t         Name[8];
	uint8_t         Mark[32];
} PRECEIVE_GUILD_MARK, * LPPRECEIVE_GUILD_MARK;

typedef struct
{
	uint8_t         KeyH;
	uint8_t         KeyL;
	uint8_t         GuildKeyH;
	uint8_t         GuildKeyL;
} PRECEIVE_GUILD_PLAYER, * LPPRECEIVE_GUILD_PLAYER;

// 길드원 목록
typedef struct
{
	uint8_t         ID[MAX_ID_SIZE];
	uint8_t         Number;
	uint8_t		 CurrentServer;
	uint8_t		 GuildStatus;
} PRECEIVE_GUILD_LIST, * LPPRECEIVE_GUILD_LIST;

// 길드원 목록 리스트
typedef struct
{
	PWMSG_HEADER Header;
	uint8_t         Result;
	uint8_t         Count;
	DWORD        TotalScore;
	uint8_t         Score;
	char		szRivalGuildName[MAX_GUILDNAME];
} PRECEIVE_GUILD_LISTS, * LPPRECEIVE_GUILD_LISTS;

//send guild leave
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          ID[MAX_ID_SIZE];
	uint8_t          ResidentNumber[10];
} PSEND_GUILD_LEAVE, * LPPSEND_GUILD_LEAVE;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         Name[8];
} PSEND_GUILD_WAR, * LPPSEND_GUILD_WAR;

//receive guild war
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         Name[8];
	uint8_t         Type;
	uint8_t         Team;
} PRECEIVE_WAR, * LPPRECEIVE_WAR;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         Score1;
	uint8_t         Score2;
	uint8_t         Type;
} PRECEIVE_WAR_SCORE, * LPPRECEIVE_WAR_SCORE;

typedef struct
{
	int			 GuildKey;
	uint8_t		 GuildStatus;
	uint8_t		 GuildType;
	uint8_t		 GuildRelationShip;
	uint8_t		 KeyH;
	uint8_t		 KeyL;
} PRECEIVE_GUILD_ID, * LPPRECEIVE_GUILD_ID;

typedef struct
{
	PBMSG_HEADER Header;
	int			GuildKey;
	uint8_t		GuildType;
	uint8_t		UnionName[MAX_GUILDNAME];
	uint8_t		GuildName[MAX_GUILDNAME];
	uint8_t		Mark[32];
} PPRECEIVE_GUILDINFO, * LPPPRECEIVE_GUILDINFO;

enum GUILD_REQ_COMMON_RESULT
{
	GUILD_ANS_NOTEXIST_GUILD = 0x10,
	GUILD_ANS_UNIONFAIL_BY_CASTLE = 0x10,
	GUILD_ANS_NOTEXIST_PERMISSION = 0x11,
	GUILD_ANS_NOTEXIST_EXTRA_STATUS = 0x12,
	GUILD_ANS_NOTEXIST_EXTRA_TYPE = 0x13,
	GUILD_ANS_EXIST_RELATIONSHIP_UNION = 0x15,
	GUILD_ANS_EXIST_RELATIONSHIP_RIVAL = 0x16,
	GUILD_ANS_EXIST_UNION = 0x17,
	GUILD_ANS_EXIST_RIVAL = 0x18,
	GUILD_ANS_NOTEXIST_UNION = 0x19,
	GUILD_ANS_NOTEXIST_RIVAL = 0x1A,
	GUILD_ANS_NOT_UNION_MASTER = 0x1B,
	GUILD_ANS_NOT_GUILD_RIVAL = 0x1C,
	GUILD_ANS_CANNOT_BE_UNION_MASTER_GUILD = 0x1D,
	GUILD_ANS_EXCEED_MAX_UNION_MEMBER = 0x1E,
	GUILD_ANS_CANCEL_REQUEST = 0x20,
	GUILD_ANS_UNION_MASTER_NOT_GENS = 0xA1,
	GUILD_ANS_GUILD_MASTER_NOT_GENS = 0xA2,
	GUILD_ANS_UNION_MASTER_DISAGREE_GENS = 0xA3,
};

typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			Value;
	uint8_t			GuildType;
} PMSG_GUILD_CREATE_RESULT, * LPPMSG_GUILD_CREATE_RESULT;

typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			byGuildType;
	uint8_t			byResult;
} PMSG_GUILD_ASSIGN_TYPE_RESULT, * LPPMSG_GUILD_ASSIGN_TYPE_RESULT;

typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			byType;
	uint8_t			byResult;
	char			szTargetName[MAX_ID_SIZE];
} PRECEIVE_GUILD_ASSIGN, * LPPRECEIVE_GUILD_ASSIGN;

typedef struct
{
	PWMSG_HEADER	Header;
	uint8_t			byCount;
	uint8_t			byResult;
	uint8_t			byRivalCount;
	uint8_t			byUnionCount;
} PMSG_UNIONLIST_COUNT, * LPPMSG_UNIONLIST_COUNT;

typedef struct
{
	uint8_t			byMemberCount;
	uint8_t			GuildMark[32];
	char			szGuildName[MAX_GUILDNAME];
} PMSG_UNIONLIST, * LPPMSG_UNIONLIST;

typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			byRelationShipType;
	uint8_t			byRequestType;
	uint8_t			byTargetUserIndexH;
	uint8_t			byTargetUserIndexL;
} PMSG_GUILD_RELATIONSHIP, * LPPMSG_GUILD_RELATIONSHIP;

typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			byRelationShipType;
	uint8_t			byRequestType;
	uint8_t			byResult;
	uint8_t			byTargetUserIndexH;
	uint8_t			byTargetUserIndexL;
} PMSG_GUILD_RELATIONSHIP_RESULT, * LPPMSG_GUILD_RELATIONSHIP_RESULT;

typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t			byResult;
	uint8_t			byRequestType;
	uint8_t			byRelationShipType;
} PMSG_BAN_UNIONGUILD, * LPPMSG_BAN_UNIONGUILD;

typedef struct
{
	PWMSG_HEADER	Header;
	uint8_t			byCount;
} PMSG_UNION_VIEWPORT_NOTIFY_COUNT, * LPPMSG_UNION_VIEWPORT_NOTIFY_COUNT;
typedef struct
{
	uint8_t			byKeyH;
	uint8_t			byKeyL;
	int				nGuildKey;
	uint8_t			byGuildRelationShip;
	char			szUnionName[MAX_GUILDNAME];
} PMSG_UNION_VIEWPORT_NOTIFY, * LPPMSG_UNION_VIEWPORT_NOTIFY;

//receive gold
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         Result;
	DWORD        StorageGold;
	DWORD        Gold;
} PRECEIVE_STORAGE_GOLD, * LPPRECEIVE_STORAGE_GOLD;

//receive soccer time
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint16_t         Time;
} PRECEIVE_SOCCER_TIME, * LPPRECEIVE_SOCCER_TIME;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         Name1[8];
	uint8_t         Score1;
	uint8_t         Name2[8];
	uint8_t         Score2;
} PRECEIVE_SOCCER_SCORE, * LPPRECEIVE_SOCCER_SCORE;

#pragma pack(push, 1)
typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t         HotKey[20];
	uint8_t         GameOption;
	uint8_t         KeyQWE[3];
	uint8_t		 ChatLogBox;
	uint8_t		 KeyR;
	int			 QWERLevel;
} PRECEIVE_OPTION, * LPPRECEIVE_OPTION;
#pragma pack(pop)

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t		 Cmd1;
	uint8_t		 Cmd2;
	uint8_t		 Cmd3;
} PRECEIVE_SERVER_COMMAND, * LPPRECEIVE_SERVER_COMMAND;


typedef struct
{
	PBMSG_HEADER Header;
	uint8_t		 m_MyRank;
	uint8_t         m_Count;
	uint8_t		 m_byRank;
} PDEVILRANK, * LPPDEVILRANK;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t        m_byType;
	int			m_nChipCount;
	short       m_shMutoNum[3];
} PRECEIVE_EVENT_CHIP_INFO, * LPPRECEIVE_EVENT_CHIP_INFO;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t        m_byType;
	unsigned int  m_unChipCount;
} PRECEIVE_ENVET_CHIP, * LPPRECEIVE_EVENT_CHIP;

typedef struct
{
	PBMSG_HEADER Header;
	short        m_shMutoNum[3];
} PRECEIVE_MUTONUMBER, * LPPRECEIVE_MUTONUMBER;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t		 m_byIsRegistered;
	char		 m_strGiftName[64];
} PRECEIVE_SCRATCH_TICKET_EVENT, * LPPRECEIVE_SCRATCH_TICKET_EVENT;

typedef struct
{
	PBMSG_HEADER Header;
	uint16_t		 wEffectNum;
} PRECEIVE_PLAY_SOUND_EFFECT, * LPPRECEIVE_PLAY_SOUND_EFFECT;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         m_byValue;
	uint8_t         m_byNumber;
} PHEADER_EVENT, * LPPHEADER_EVENT;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t            m_wEventType;
	uint8_t            m_wLeftEnterCount;
}PRECEIVE_EVENT_COUNT, * LPPRECEIVE_EVENT_COUNT;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         m_byCount;
	uint8_t         m_byQuest[50];
} PRECEIVE_QUEST_HISTORY, * LPPRECEIVE_QUEST_HISTORY;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         m_byQuestIndex;
	uint8_t         m_byState;
} PRECEIVE_QUEST_STATE, * LPPRECEIVE_QUEST_STATE;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         m_byQuestIndex;
	uint8_t         m_byResult;
	uint8_t         m_byState;
} PRECEIVE_QUEST_RESULT, * LPPRECEIVE_QUEST_RESULT;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         m_byKeyH;
	uint8_t         m_byKeyL;
	uint8_t         m_byReparation;
	uint8_t         m_byNumber;
} PRECEIVE_QUEST_REPARATION, * LPPRECEIVE_QUEST_REPARATION;

// GC[0xF6][0x0A]
typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			SubCode;

	uint16_t			m_wNPCIndex;
	uint16_t			m_wQuestCount;
} PMSG_NPCTALK_QUESTLIST, * LPPMSG_NPCTALK_QUESTLIST;

#pragma pack(push, 1)
typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			SubCode;
	DWORD			m_dwQuestIndex;
	uint8_t			m_byRequestCount;
	uint8_t			m_byRewardCount;
	uint8_t			m_byRandRewardCount;
} PMSG_NPC_QUESTEXP_INFO, * LPPMSG_NPC_QUESTEXP_INFO;
#pragma pack(pop)

enum QUEST_REQUEST_TYPE
{
	QUEST_REQUEST_NONE = 0x0000,
	QUEST_REQUEST_MONSTER = 0x0001,
	QUEST_REQUEST_SKILL = 0x0002,
	QUEST_REQUEST_ITEM = 0x0004,
	QUEST_REQUEST_LEVEL = 0x0008,
	QUEST_REQUEST_TUTORIAL = 0x0010,
	QUEST_REQUEST_BUFF = 0x0020,
	QUEST_REQUEST_EVENT_MAP_USER_KILL = 0x0040,
	QUEST_REQUEST_EVENT_MAP_MON_KILL = 0x0041,
	QUEST_REQUEST_EVENT_MAP_BLOOD_GATE = 0x0042,
	QUEST_REQUEST_EVENT_MAP_CLEAR_BLOOD = 0x0100,
	QUEST_REQUEST_EVENT_MAP_CLEAR_CHAOS = 0x0101,
	QUEST_REQUEST_EVENT_MAP_CLEAR_DEVIL = 0x0102,
	QUEST_REQUEST_EVENT_MAP_CLEAR_ILLUSION = 0x0103,
	QUEST_REQUEST_EVENT_MAP_DEVIL_POINT = 0x0104,
	QUEST_REQUEST_ZEN = 0x0105,
	QUEST_REQUEST_PVP_POINT = 0x0106,
	QUEST_REQUEST_NPC_TALK = 0x0107,
	QUEST_REQUEST_TYPE_MAX
};

#pragma pack(push, 1)
typedef struct
{
	DWORD	m_dwType;
	uint16_t	m_wIndex;
	DWORD	m_dwValue;
	uint16_t	m_wCurValue;
	uint8_t	m_byItemInfo[PACKET_ITEM_LENGTH];
} NPC_QUESTEXP_REQUEST_INFO, * LPNPC_QUESTEXP_REQUEST_INFO;
#pragma pack(pop)

// 보상
enum QUEST_REWARD_TYPE
{
	QUEST_REWARD_NONE = 0x0000,
	QUEST_REWARD_EXP = 0x0001,
	QUEST_REWARD_ZEN = 0x0002,
	QUEST_REWARD_ITEM = 0x0004,
	QUEST_REWARD_BUFF = 0x0008,
	QUEST_REWARD_CONTRIBUTE = 0x0010,
	QUEST_REWARD_RANDOM = 0x0020,
	QUEST_REWARD_TYPE_MAX
};

#pragma pack(push, 1)
typedef struct
{
	DWORD	m_dwType;
	uint16_t	m_wIndex;
	DWORD	m_dwValue;
	uint8_t	m_byItemInfo[PACKET_ITEM_LENGTH];
} NPC_QUESTEXP_REWARD_INFO, * LPNPC_QUESTEXP_REWARD_INFO;
#pragma pack(pop)

typedef struct
{
	NPC_QUESTEXP_REQUEST_INFO	NpcQuestRequestInfo[5];
	NPC_QUESTEXP_REWARD_INFO	NpcQuestRewardInfo[5];
} NPC_QUESTEXP_INFO, * LPNPC_QUESTEXP_INFO;

//----------------------------------------------------------------------------
// GC[0xF6][0x0D]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			SubCode;

	DWORD			m_dwQuestIndex;
	uint8_t			m_byResult;
} PMSG_ANS_QUESTEXP_COMPLETE, * LPPMSG_ANS_QUESTEXP_COMPLETE;

//----------------------------------------------------------------------------
// GC[0xF6][0x0F]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			SubCode;

	DWORD			m_dwQuestGiveUpIndex;
} PMSG_ANS_QUESTEXP_GIVEUP, * LPPMSG_ANS_QUESTEXP_GIVEUP;

//----------------------------------------------------------------------------
// GC[0xF6][0x1A]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			SubCode;

	uint8_t			m_byQuestCount;
} PMSG_ANS_QUESTEXP_PROGRESS_LIST, * LPPMSG_ANS_QUESTEXP_PROGRESS_LIST;

//----------------------------------------------------------------------------
// GC[0xF8][0x02]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			SubCode;

	uint8_t			m_byResult;
	uint8_t			m_byInfluence;
} PMSG_ANS_REG_GENS_MEMBER, * LPPMSG_ANS_REG_GENS_MEMBER;

//----------------------------------------------------------------------------
// GC[0xF8][0x04]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			SubCode;

	uint8_t			m_byResult;

} PMSG_ANS_SECEDE_GENS_MEMBER, * LPPMSG_ANS_SECEDE_GENS_MEMBER;

//----------------------------------------------------------------------------
// GC[0xF8][0x07]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			SubCode;

	uint8_t			m_byInfluence;
	int				m_nRanking;
	int				m_nGensClass;
	int				m_nContributionPoint;
	int				m_nNextContributionPoint;
} PMSG_MSG_SEND_GENS_INFO, * LPPMSG_MSG_SEND_GENS_INFO;

//----------------------------------------------------------------------------
// GC[0xF8][0x05]
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEADER    Header;
	uint8_t			SubCode;

	uint8_t			m_byCount;
} PMSG_SEND_GENS_MEMBER_VIEWPORT, * LPPMSG_SEND_GENS_MEMBER_VIEWPORT;

typedef struct
{
	uint8_t			m_byInfluence;
	uint8_t			m_byNumberH;
	uint8_t			m_byNumberL;
	int				m_nRanking;
	int				m_nGensClass;
	int				m_nContributionPoint;
} PMSG_GENS_MEMBER_VIEWPORT_INFO, * LPPMSG_GENS_MEMBER_VIEWPORT_INFO;
#endif	// ASG_ADD_GENS_SYSTEM

//----------------------------------------------------------------------------
// GC[0xF8][0x08]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			SubCode;
	uint8_t			m_byRewardResult;
}PMSG_GENS_REWARD_CODE, * LPPMSG_GENS_REWARD_CODE;

//----------------------------------------------------------------------------
// GC[0xF9][0x01]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER	Header;
	uint8_t			SubCode;

	uint16_t			m_wNPCIndex;
	DWORD			m_dwContributePoint;
} PMSG_ANS_NPC_CLICK, * LPPMSG_ANS_NPC_CLICK;

typedef struct
{
	uint8_t        m_byX;
	uint8_t        m_byY;
}PRECEIVE_MAP_ATTRIBUTE, * LPPRECEIVE_MAP_ATTRIBUTE;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t            m_byType;
	uint8_t            m_byMapAttr;
	uint8_t            m_byMapSetType;
	uint8_t            m_byCount;

	PRECEIVE_MAP_ATTRIBUTE  m_vAttribute[128 * 128];
}PRECEIVE_SET_MAPATTRIBUTE, * LPPRECEIVE_SET_MAPATTRIBUTE;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t            m_byPlayState;
	uint16_t            m_wRemainSec;
	uint16_t            m_wMaxKillMonster;
	uint16_t            m_wCurKillMonster;
	uint16_t            m_wIndex;
	uint8_t            m_byItemType;
}PRECEIVE_MATCH_GAME_STATE, * LPPRECEIVE_MATCH_GAME_STATE;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t		nResult;
	uint8_t		bIndexH;
	uint8_t		bIndexL;
	CHAR		szID[MAX_ID_SIZE];
} PMSG_ANS_DUEL_INVITE, * LPPMSG_ANS_DUEL_INVITE;

typedef struct _tagPMSG_REQ_DUEL_ANSWER		// SC2
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t		bIndexH;
	uint8_t		bIndexL;
	CHAR		szID[MAX_ID_SIZE];
} PMSG_REQ_DUEL_ANSWER, * LPPMSG_REQ_DUEL_ANSWER;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t		nResult;
	uint8_t		bIndexH;
	uint8_t		bIndexL;
	CHAR		szID[MAX_ID_SIZE];
} PMSG_ANS_DUEL_EXIT, * LPPMSG_ANS_DUEL_EXIT;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t		bIndexH1;
	uint8_t		bIndexL1;
	uint8_t		bIndexH2;
	uint8_t		bIndexL2;
	uint8_t		btDuelScore1;
	uint8_t		btDuelScore2;
} PMSG_DUEL_SCORE_BROADCAST, * LPPMSG_DUEL_SCORE_BROADCAST;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t		bIndexH1;
	uint8_t		bIndexL1;
	uint8_t		bIndexH2;
	uint8_t		bIndexL2;
	uint8_t		btHP1;
	uint8_t		btHP2;
	uint8_t		btShield1;
	uint8_t		btShield2;
} PMSG_DUEL_HP_BROADCAST, * LPPMSG_DUEL_HP_BROADCAST;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	struct
	{
		CHAR	szID1[MAX_ID_SIZE];
		CHAR	szID2[MAX_ID_SIZE];
		uint8_t	bStart;
		uint8_t	bWatch;
	} channel[4];
} PMSG_ANS_DUEL_CHANNELLIST, * LPPMSG_ANS_DUEL_CHANNELLIST;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t		nResult;
	uint8_t		nChannelId;
	CHAR		szID1[MAX_ID_SIZE];
	CHAR		szID2[MAX_ID_SIZE];
	uint8_t		bIndexH1;
	uint8_t		bIndexL1;
	uint8_t		bIndexH2;
	uint8_t		bIndexL2;
} PMSG_ANS_DUEL_JOINCNANNEL, * LPPMSG_ANS_DUEL_JOINCNANNEL;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	CHAR		szID[MAX_ID_SIZE];
} PMSG_DUEL_JOINCNANNEL_BROADCAST, * LPPMSG_DUEL_JOINCNANNEL_BROADCAST;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t		nResult;
} PMSG_ANS_DUEL_LEAVECNANNEL, * LPPMSG_ANS_DUEL_LEAVECNANNEL;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	CHAR		szID[MAX_ID_SIZE];
} PMSG_DUEL_LEAVECNANNEL_BROADCAST, * LPPMSG_DUEL_LEAVECNANNEL_BROADCAST;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t		nCount;
	struct
	{
		CHAR	szID[MAX_ID_SIZE];
	} user[10];
} PMSG_DUEL_OBSERVERLIST_BROADCAST, * LPPMSG_DUEL_OBSERVERLIST_BROADCAST;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	CHAR		szWinner[MAX_ID_SIZE];
	CHAR		szLoser[MAX_ID_SIZE];
} PMSG_DUEL_RESULT_BROADCAST, * LPPMSG_DUEL_RESULT_BROADCAST;

typedef struct
{
	PBMSG_HEADER Header;
	uint8_t         SubCode;
	uint8_t		nFlag;
} PMSG_DUEL_ROUNDSTART_BROADCAST, * LPPMSG_DUEL_ROUNDSTART_BROADCAST;

typedef struct tagPSHOPTITLE_HEADER
{
	PSWMSG_HEADER   Header;
	uint8_t			byCount;
} PSHOPTITLE_HEADERINFO, * LPPSHOPTITLE_HEADERINFO;

typedef struct tagPSHOPTITLE_DATA
{
	uint8_t	byIndexH;
	uint8_t	byIndexL;
	uint8_t	szTitle[36];	//. MAX_SHOPTITLE
} PSHOPTITLE_DATAINFO, * LPPSHOPTITLE_DATAINFO;

typedef struct tagPSHOPTITLE_CHANGE
{
	PBMSG_HEADER	Header;
	uint8_t			bySubcode;
	uint8_t			byIndexH;
	uint8_t			byIndexL;
	uint8_t			szTitle[36];	//. MAX_SHOPTITLE
	uint8_t			szId[MAX_ID_SIZE];
} PSHOPTITLE_CHANGEINFO, * LPPSHOPTITLE_CHANGEINFO;

typedef struct tagPSHOPSETPRICE_RESULT
{
	PBMSG_HEADER	Header;
	uint8_t			bySubcode;
	uint8_t			byResult;
	uint8_t			byItemPos;
} PSHOPSETPRICE_RESULTINFO, * LPPSHOPSETPRICE_RESULTINFO;

typedef struct tagCREATEPSHOP_RESULT
{
	PBMSG_HEADER	Header;
	uint8_t			bySubcode;
	uint8_t			byResult;
	uint8_t			byTypeValue;
} CREATEPSHOP_RESULTINFO, * LPCREATEPSHOP_RESULSTINFO;

typedef struct tagDESTROYPSHOP_RESULT
{
	PBMSG_HEADER	Header;
	uint8_t			bySubcode;
	uint8_t			byResult;
	uint8_t			byIndexH;
	uint8_t			byIndexL;
} DESTROYPSHOP_RESULTINFO, * LPDESTROYPSHOP_RESULTINFO;

typedef struct tagGETPSHOPITEMLIST_HEADER
{
	PWMSG_HEADER    Header;
	uint8_t			bySubcode;
	uint8_t			byResult;
	uint8_t			byIndexH;
	uint8_t			byIndexL;
	uint8_t			szId[MAX_ID_SIZE];
	uint8_t			szShopTitle[36];	//. MAX_SHOPTITLE
	uint8_t			byCount;
} GETPSHOPITEMLIST_HEADERINFO, * LPGETPSHOPITEMLIST_HEADERINFO;

typedef struct tagGETPSHOPITEM_DATA
{
	uint8_t	byPos;
	uint8_t	byItemInfo[PACKET_ITEM_LENGTH];
	INT		iItemPrice;
	uint16_t	JoBValue;
	uint16_t	JoSValue;
	uint16_t	JoCValue;
	uint8_t	PersonalShopType;
} GETPSHOPITEM_DATAINFO, * LPGETPSHOPITEM_DATAINFO;

typedef struct tagCLOSEPSHOP_RESULT
{
	PWMSG_HEADER    Header;
	uint8_t			bySubcode;
	uint8_t			byIndexH;
	uint8_t			byIndexL;
} CLOSEPSHOP_RESULTINFO, * LPCLOSEPSHOP_RESULTINFO;

typedef struct tagPURCHASEITEM_RESULT
{
	PSBMSG_HEADER	Header;
	uint8_t			byResult;
	uint8_t			byIndexH;
	uint8_t			byIndexL;
	uint8_t			byItemInfo[PACKET_ITEM_LENGTH];
	uint8_t			byPos;
} PURCHASEITEM_RESULTINFO, * LPPURCHASEITEM_RESULTINFO;

typedef struct tagSOLDITEM_RESULT
{
	PBMSG_HEADER	Header;
	uint8_t			bySubcode;
	uint8_t			byPos;
	uint8_t			szId[MAX_ID_SIZE];
} SOLDITEM_RESULTINFO, * LPSOLDITEM_RESULTINFO;

typedef struct tagDISPLAYEFFECT_NOTIFY
{
	PBMSG_HEADER	Header;
	uint8_t			byIndexH;
	uint8_t			byIndexL;
	uint8_t			byType;
} DISPLAYEREFFECT_NOTIFYINFO, * LPDISPLAYEREFFECT_NOTIFYINFO;

typedef struct
{
	PWMSG_HEADER    Header;
	uint8_t			MemoCount;
	uint8_t			MaxMemo;
	uint8_t			Count;
} FS_FRIEND_LIST_HEADER, * LPFS_FRIEND_LIST_HEADER;

typedef struct
{
	uint8_t			Name[MAX_ID_SIZE];
	uint8_t			Server;
} FS_FRIEND_LIST_DATA, * LPFS_FRIEND_LIST_DATA;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t			Result;
	uint8_t			Name[MAX_ID_SIZE];
	uint8_t			Server;
} FS_FRIEND_RESULT, * LPFS_FRIEND_RESULT;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t			Name[MAX_ID_SIZE];
} FS_ACCEPT_ADD_FRIEND_RESULT, * LPFS_ACCEPT_ADD_FRIEND_RESULT;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t			Name[MAX_ID_SIZE];
	uint8_t			Server;
} FS_FRIEND_STATE_CHANGE, * LPFS_FRIEND_STATE_CHANGE;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t			Result;
	DWORD			WindowGuid;
} FS_SEND_LETTER_RESULT, * LPFS_SEND_LETTER_RESULT;

typedef struct
{
	PBMSG_HEADER    Header;
	uint16_t		Index;
	uint8_t			Name[MAX_ID_SIZE];
	uint8_t			Date[30];
	uint8_t			Subject[MAX_LETTER_TITLE_LENGTH];
	uint8_t			Read;
} FS_LETTER_ALERT, * LPFS_LETTER_ALERT;

typedef struct
{
	PWMSG_HEADER Header;
	uint16_t  Index;
	uint16_t  MemoSize;
	uint32_t Equipment[EQUIPMENT_LENGTH];
	uint8_t  PhotoDir;
	uint8_t  PhotoAction;
	uint8_t  Memo[MAX_LETTERTEXT_LENGTH];
} FS_LETTER_TEXT, * LPFS_LETTER_TEXT;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t			Result;
	uint16_t			Index;
} FS_LETTER_RESULT, * LPFS_LETTER_RESULT;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t			IP[15];
	uint16_t			RoomNumber;
	DWORD			Ticket;
	uint8_t			Type;
	uint8_t			ID[10];
	uint8_t			Result;
} FS_CHAT_CREATE_RESULT, * LPFS_CHAT_CREATE_RESULT;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t			Result;
} FS_CHAT_JOIN_RESULT, * LPFS_CHAT_JOIN_RESULT;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t			Type;
	uint8_t			Index;
	uint8_t			Name[MAX_ID_SIZE];
} FS_CHAT_CHANGE_STATE, * LPFS_CHAT_CHANGE_STATE;

typedef struct
{
	PWMSG_HEADER    Header;
	uint16_t			RoomNumber;
	uint8_t			Count;
} FS_CHAT_USERLIST_HEADER, * LPFS_CHAT_USERLIST_HEADER;

typedef struct
{
	uint8_t			Index;
	uint8_t			Name[MAX_ID_SIZE];
} FS_CHAT_USERLIST_DATA, * LPFS_CHAT_USERLIST_DATA;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t			Result;
	DWORD			WindowGuid;
} FS_CHAT_INVITE_RESULT, * LPFS_CHAT_INVITE_RESULT;

typedef struct
{
	PBMSG_HEADER    Header;
	uint8_t			Index;
	uint8_t			MsgSize;
	uint8_t			Msg[100];
} FS_CHAT_TEXT, * LPFS_CHAT_TEXT;

//----------------------------------------------------------------------------
// GC [0x2D]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER	h;

	uint16_t		wOptionType;
	uint16_t		wEffectType;
	uint8_t		byEffectOption;
	int			wEffectTime;
	uint8_t		byBuffType;
} PMSG_ITEMEFFECTCANCEL, * LPPMSG_ITEMEFFECTCANCEL;

typedef struct
{
	PBMSG_HEADER    m_Header;
	uint8_t            m_byPetType;
	uint8_t            m_byCommand;
	uint8_t            m_byKeyH;
	uint8_t            m_byKeyL;
}PRECEIVE_PET_COMMAND, * LPPRECEIVE_PET_COMMAND;


typedef struct
{
	PBMSG_HEADER    m_Header;
	uint8_t            m_byPetType;
	uint8_t            m_bySkillType;
	uint8_t            m_byKeyH;
	uint8_t            m_byKeyL;
	uint8_t            m_byTKeyH;
	uint8_t            m_byTKeyL;
}PRECEIVE_PET_ATTACK, * LPPRECEIVE_PET_ATTACK;


typedef struct
{
	PBMSG_HEADER    m_Header;
	uint8_t            m_byPetType;
	uint8_t            m_byInvType;
	uint8_t            m_byPos;
	uint8_t            m_byLevel;
	int             m_iExp;
	uint8_t			m_byLife;
}PRECEIVE_PET_INFO, * LPPRECEIVE_PET_INFO;

typedef struct
{
	PBMSG_HEADER    m_Header;
	uint8_t			m_subCode;
	uint8_t			m_Type;
	uint16_t			m_Time;
}PMSG_MATCH_TIMEVIEW, * LPPMSG_MATCH_TIMEVIEW;

typedef struct
{
	void			Clear()
	{
		memset(&m_MatchTeamName1, NULL, MAX_ID_SIZE);
		memset(&m_MatchTeamName2, NULL, MAX_ID_SIZE);
		m_Score1 = 0;
		m_Score2 = 0;
		m_Type = 0;
	}

	PBMSG_HEADER	m_Header;
	uint8_t			m_subCode;
	uint8_t			m_Type;

	char			m_MatchTeamName1[MAX_ID_SIZE];
	uint16_t			m_Score1;

	char			m_MatchTeamName2[MAX_ID_SIZE];
	uint16_t			m_Score2;

}PMSG_MATCH_RESULT, * LPPMSG_MATCH_RESULT;

typedef struct
{
	PBMSG_HEADER	m_Header;
	uint8_t			m_subCode;
	uint8_t			m_x;
	uint8_t			m_y;
}PMSG_SOCCER_GOALIN, * LPPMSG_SOCCER_GOALIN;

extern PMSG_MATCH_RESULT		g_wtMatchResult;
extern PMSG_MATCH_TIMEVIEW		g_wtMatchTimeLeft;
extern int g_iGoalEffect;

typedef struct
{
	PBMSG_HEADER    m_Header;
	MServerInfo     m_vSvrInfo;
}PHEADER_MAP_CHANGESERVER_INFO, * LPPHEADER_MAP_CHANGESERVER_INFO;


enum CASTLESIEGE_STATE
{
	CASTLESIEGE_STATE_NONE = -1,
	CASTLESIEGE_STATE_IDLE_1 = 0,
	CASTLESIEGE_STATE_REGSIEGE = 1,
	CASTLESIEGE_STATE_IDLE_2 = 2,
	CASTLESIEGE_STATE_REGMARK = 3,
	CASTLESIEGE_STATE_IDLE_3 = 4,
	CASTLESIEGE_STATE_NOTIFY = 5,
	CASTLESIEGE_STATE_READYSIEGE = 6,
	CASTLESIEGE_STATE_STARTSIEGE = 7,
	CASTLESIEGE_STATE_ENDSIEGE = 8,
	CASTLESIEGE_STATE_ENDCYCLE = 9,
};

//----------------------------------------------------------------------------
// GC [0xB2][0x00]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t		btResult;
	CHAR		cCastleSiegeState;
	uint8_t		btStartYearH;
	uint8_t		btStartYearL;
	uint8_t		btStartMonth;
	uint8_t		btStartDay;
	uint8_t		btStartHour;
	uint8_t		btStartMinute;
	uint8_t		btEndYearH;
	uint8_t		btEndYearL;
	uint8_t		btEndMonth;
	uint8_t		btEndDay;
	uint8_t		btEndHour;
	uint8_t		btEndMinute;
	uint8_t		btSiegeStartYearH;
	uint8_t		btSiegeStartYearL;
	uint8_t		btSiegeStartMonth;
	uint8_t		btSiegeStartDay;
	uint8_t		btSiegeStartHour;
	uint8_t		btSiegeStartMinute;
	CHAR		cOwnerGuild[8];
	CHAR		cOwnerGuildMaster[10];

	CHAR		btStateLeftSec1;
	CHAR		btStateLeftSec2;
	CHAR		btStateLeftSec3;
	CHAR		btStateLeftSec4;
} PMSG_ANS_CASTLESIEGESTATE, * LPPMSG_ANS_CASTLESIEGESTATE;

//----------------------------------------------------------------------------
// GC [0xB2][0x01]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t		btResult;
	CHAR		szGuildName[8];
} PMSG_ANS_REGCASTLESIEGE, * LPPMSG_ANS_REGCASTLESIEGE;

//----------------------------------------------------------------------------
// GC [0xB2][0x02]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t		btResult;
	uint8_t		btIsGiveUp;
	CHAR		szGuildName[8];
} PMSG_ANS_GIVEUPCASTLESIEGE, * LPPMSG_ANS_GIVEUPCASTLESIEGE;

//----------------------------------------------------------------------------
// GC [0xB2][0x03]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t		btResult;
	CHAR		szGuildName[8];
	uint8_t		btGuildMark1;
	uint8_t		btGuildMark2;
	uint8_t		btGuildMark3;
	uint8_t		btGuildMark4;
	uint8_t		btIsGiveUp;
	uint8_t		btRegRank;
} PMSG_ANS_GUILDREGINFO, * LPPMSG_ANS_GUILDREGINFO;

//----------------------------------------------------------------------------
// GC [0xB2][0x04]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t		btResult;
	CHAR		szGuildName[8];
	uint8_t		btGuildMark1;
	uint8_t		btGuildMark2;
	uint8_t		btGuildMark3;
	uint8_t		btGuildMark4;
} PMSG_ANS_REGGUILDMARK, * LPPMSG_ANS_REGGUILDMARK;

//----------------------------------------------------------------------------
// GC [0xB2][0x05]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t		btResult;
	INT			iNpcNumber;
	INT			iNpcIndex;
} PMSG_ANS_NPCBUY, * LPPMSG_ANS_NPCBUY;

//----------------------------------------------------------------------------
// GC [0xB2][0x06]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t		btResult;
	INT			iNpcNumber;
	INT			iNpcIndex;
	INT			iNpcHP;
	INT			iNpcMaxHP;
} PMSG_ANS_NPCREPAIR, * LPPMSG_ANS_NPCREPAIR;

//----------------------------------------------------------------------------
// GC [0xB2][0x07]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t		btResult;
	INT			iNpcNumber;
	INT			iNpcIndex;
	INT			iNpcUpType;
	INT			iNpcUpValue;
} PMSG_ANS_NPCUPGRADE, * LPPMSG_ANS_NPCUPGRADE;

//----------------------------------------------------------------------------
// CG [0xB2][0x08]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t		btResult;
	uint8_t		btTaxRateChaos;
	uint8_t		btTaxRateStore;
	uint8_t		btMoney1;
	uint8_t		btMoney2;
	uint8_t		btMoney3;
	uint8_t		btMoney4;
	uint8_t		btMoney5;
	uint8_t		btMoney6;
	uint8_t		btMoney7;
	uint8_t		btMoney8;
} PMSG_ANS_TAXMONEYINFO, * LPPMSG_ANS_TAXMONEYINFO;

//----------------------------------------------------------------------------
// CG [0xB2][0x09]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t		btResult;
	uint8_t		btTaxType;
	uint8_t		btTaxRate1;
	uint8_t		btTaxRate2;
	uint8_t		btTaxRate3;
	uint8_t		btTaxRate4;
} PMSG_ANS_TAXRATECHANGE, * LPPMSG_ANS_TAXRATECHANGE;

//----------------------------------------------------------------------------
// CG [0xB2][0x0A]
//----------------------------------------------------------------------------
typedef struct
{
	PSBMSG_HEADER	Header;
	int iNpcDefense1[4];
	int iNpcDefense2[4];
	int iNpcUpMaxHP1[4];
	int iNpcUpMaxHP2[4];
	int iNpcUpRecLevel[4];
} PMSG_INFO_STATUE_AND_GATE, * LPPMSG_INFO_STATUE_AND_GATE;
//----------------------------------------------------------------------------
// CG [0xB2][0x10]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t		btResult;
	uint8_t		btMoney1;
	uint8_t		btMoney2;
	uint8_t		btMoney3;
	uint8_t		btMoney4;
	uint8_t		btMoney5;
	uint8_t		btMoney6;
	uint8_t		btMoney7;
	uint8_t		btMoney8;
} PMSG_ANS_MONEYDRAWOUT, * LPPMSG_ANS_MONEYDRAWOUT;

//----------------------------------------------------------------------------
// CG [0xB2][0x1A]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	Header;
	uint8_t		btTaxType;
	uint8_t		btTaxRate;
} PMSG_ANS_MAPSVRTAXINFO, * LPPMSG_ANS_MAPSVRTAXINFO;

//----------------------------------------------------------------------------
// GC [0xB3]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE    m_Header;
	uint8_t		btResult;
	INT			iCount;
} PMSG_ANS_NPCDBLIST, * LPPMSG_ANS_NPCDBLIST;

typedef struct
{
	INT			iNpcNumber;
	INT			iNpcIndex;
	INT			iNpcDfLevel;
	INT			iNpcRgLevel;
	INT			iNpcMaxHp;
	INT			iNpcHp;
	uint8_t		btNpcX;
	uint8_t		btNpcY;
	uint8_t		btNpcLive;
} PMSG_NPCDBLIST, * LPPMSG_NPCDBLIST;

//----------------------------------------------------------------------------
// GC [0xB4]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	h;
	uint8_t		btResult;
	INT			iCount;
} PMSG_ANS_CSREGGUILDLIST, * LPPMSG_ANS_CSREGGUILDLIST;

typedef struct
{
	CHAR		szGuildName[8];
	uint8_t		btRegMarks1;
	uint8_t		btRegMarks2;
	uint8_t		btRegMarks3;
	uint8_t		btRegMarks4;
	uint8_t		btIsGiveUp;
	uint8_t		btSeqNum;
} PMSG_CSREGGUILDLIST, * LPPMSG_CSREGGUILDLIST;

//----------------------------------------------------------------------------
// GC [0xB5]
//----------------------------------------------------------------------------
typedef struct
{
	PREQUEST_DEFAULT_SUBCODE	h;
	uint8_t		btResult;
	INT			iCount;
} PMSG_ANS_CSATTKGUILDLIST, * LPPMSG_ANS_CSATTKGUILDLIST;

typedef struct
{
	uint8_t		btCsJoinSide;
	uint8_t		btGuildInvolved;
	CHAR		szGuildName[8];
	INT			iGuildScore;
} PMSG_CSATTKGUILDLIST, * LPPMSG_CSATTKGUILDLIST;


typedef struct
{
	PBMSG_HEADER    m_Header;
	uint8_t            m_bySubCode;
}PBMSG_HEADER2;

//----------------------------------------------------------------------------
// GC [0xA4][0x00]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t	byResult;
	uint8_t	byQuestIndex;
	int		anKillCountInfo[10];
} PMSG_ANS_QUEST_MONKILL_INFO, * LPPMSG_ANS_QUEST_MONKILL_INFO;

typedef struct
{
	PBMSG_HEADER2       m_Header;
	uint8_t                m_byResult;
	uint8_t                m_byHuntZoneEnter;
}PMSG_CSHUNTZONEENTER, * LPPMSG_CSHUNTZONEENTER;

typedef struct
{
	PBMSG_HEADER2       m_Header;
	uint8_t                m_byResult;
	uint8_t                m_byEnable;
	INT                 m_iCurrPrice;
	INT                 m_iMaxPrice;
	INT                 m_iUnitPrice;
}PRECEIVE_CASTLE_HUNTZONE_INFO, * LPPRECEIVE_CASTLE_HUNTZONE_INFO;

typedef struct
{
	PBMSG_HEADER2       m_Header;
	uint8_t                m_byResult;
}PRECEVIE_CASTLE_HUNTZONE_RESULT, * LPPRECEVIE_CASTLE_HUNTZONE_RESULT;

typedef struct
{
	PBMSG_HEADER2       m_Header;
	uint8_t                m_byGuildMark[32];
}PRECEIVE_CASTLE_FLAG, * LPPRECEIVE_CASTLE_FLAG;

typedef struct
{
	PBMSG_HEADER2        m_Header;
	uint8_t                m_byResult;
	uint8_t                m_byKeyH;
	uint8_t                m_byKeyL;
}PRECEIVE_GATE_STATE, * LPPRECEIVE_GATE_STATE;


typedef struct
{
	PBMSG_HEADER2        m_Header;
	uint8_t                m_byResult;
	uint8_t                m_byOperator;
	uint8_t                m_byKeyH;
	uint8_t                m_byKeyL;
}PRECEIVE_GATE_OPERATOR, * LPPRECEIVE_GATE_OPERATOR;


typedef struct
{
	PBMSG_HEADER2        m_Header;
	uint8_t                m_byOperator;
	uint8_t                m_byKeyH;
	uint8_t                m_byKeyL;
}PRECEIVE_GATE_CURRENT_STATE, * LPPRECEIVE_GATE_CURRENT_STATE;


typedef struct
{
	PBMSG_HEADER2       m_Header;
	uint8_t                m_byIndexH;
	uint8_t                m_byIndexL;
	uint8_t                m_byKeyH;
	uint8_t                m_byKeyL;
	uint8_t                m_byState;
}PRECEIVE_SWITCH_PROC, * LPPRECEIVE_SWITCH_PROC;


typedef struct
{
	PBMSG_HEADER2       m_Header;
	uint8_t                m_byCrownState;

	DWORD				m_dwCrownAccessTime;

}PRECEIVE_CROWN_STATE, * LPPRECEIVE_CROWN_STATE;


typedef struct
{
	PBMSG_HEADER2		m_Header;
	uint8_t				m_byIndex1;
	uint8_t				m_byIndex2;
	uint8_t				m_bySwitchState;
	uint8_t				m_JoinSide;

	uint8_t				m_szGuildName[8];
	uint8_t				m_szUserName[MAX_ID_SIZE + 1];
}PRECEIVE_CROWN_SWITCH_INFO, * LPRECEIVE_CROWN_SWITCH_INFO;


typedef struct
{
	PBMSG_HEADER2       m_Header;
	uint8_t                m_byBasttleCastleState;
	uint8_t                m_szGuildName[8];
}PRECEIVE_BC_PROCESS, * LPPRECEIVE_BC_PROCESS;

typedef struct
{
	PBMSG_HEADER2       m_Header;
	uint8_t                m_byKeyH;
	uint8_t                m_byKeyL;

	uint8_t                m_byBuildTime;
}PRECEIVE_MONSTER_BUILD_TIME, * LPPRECEIVE_MONSTER_BUILD_TIME;

typedef struct
{
	PBMSG_HEADER2       m_Header;
	uint8_t                m_byResult;

	uint8_t                m_byWeaponType;

	uint8_t                m_byKeyH;
	uint8_t                m_byKeyL;
}PRECEIVE_CATAPULT_STATE, * LPPRECEIVE_CATAPULT_STATE;

typedef struct
{
	PBMSG_HEADER2        m_Header;
	uint8_t                m_byResult;
	uint8_t                m_byKeyH;
	uint8_t                m_byKeyL;

	uint8_t                m_byWeaponType;

	uint8_t                m_byTargetX;
	uint8_t                m_byTargetY;
}PRECEIVE_WEAPON_FIRE, * LPPRECEIVE_WEAPON_FIRE;

typedef struct
{
	PBMSG_HEADER2        m_Header;
	uint8_t                m_byWeaponType;

	uint8_t                m_byTargetX;
	uint8_t                m_byTargetY;
}PRECEIVE_BOMBING_ALERT, * LPPRECEIVE_BOMBING_ALERT;

typedef struct
{
	PBMSG_HEADER2        m_Header;
	uint8_t                m_byKeyH;
	uint8_t                m_byKeyL;

	uint8_t                m_byWeaponType;
}PRECEIVE_BOMBING_TARGET, * LPPRECEIVE_BOMBING_TARGET;

typedef struct
{
	uint8_t  m_byObjType;
	uint8_t  m_byTypeH;
	uint8_t  m_byTypeL;
	uint8_t  m_byKeyH;
	uint8_t  m_byKeyL;
	uint8_t  m_byPosX;
	uint8_t  m_byPosY;
	uint32_t m_byEquipment[EQUIPMENT_LENGTH];
	uint8_t  s_BuffCount;
	uint8_t  s_BuffEffectState[MAX_BUFF_SLOT_INDEX];
}PRECEIVE_PREVIEW_PORT, * LPPRECEIVE_PREVIEW_PORT;

typedef struct
{
	PBMSG_HEADER2       m_Header;
	uint8_t                m_byTeam;
	uint8_t                m_byX;
	uint8_t                m_byY;
	uint8_t                m_byCmd;
}PRECEIVE_GUILD_COMMAND, * LPPRECEIVE_GUILD_COMMAND;

typedef struct
{
	uint8_t                m_byX;
	uint8_t                m_byY;
}PRECEIVE_MEMBER_LOCATION, * LPPRECEIVE_MEMBER_LOCATION;

typedef struct
{
	uint8_t                m_byType;
	uint8_t                m_byX;
	uint8_t                m_byY;
}PRECEIVE_NPC_LOCATION, * LPPRECEIVE_NPC_LOCATION;

typedef struct
{
	PBMSG_HEADER2       m_Header;
	uint8_t                m_byResult;
}PRECEIVE_MAP_INFO_RESULT, * LPPRECEIVE_MAP_INFO_RESULT;

typedef struct
{
	PBMSG_HEADER2       m_Header;
	uint8_t                m_byHour;
	uint8_t                m_byMinute;
}PRECEIVE_MATCH_TIMER, * LPPRECEIVE_MATCH_TIMER;

typedef struct
{
	PBMSG_HEADER2		m_Header;
	uint8_t				m_iJewelType;
	uint8_t				m_iJewelMix;
}PMSG_REQ_JEWEL_MIX, * LPPMSG_REQ_JEWEL_MIX;

typedef struct
{
	PBMSG_HEADER2		m_Header;
	uint8_t				m_iResult;
}PMSG_ANS_JEWEL_MIX, * LPPMSG_ANS_JEWEL_MIX;

typedef struct
{
	PBMSG_HEADER2		m_Header;
	uint8_t				m_iJewelType;
	uint8_t				m_iJewelLevel;
	uint8_t				m_iJewelPos;
}PMSG_REQ_JEWEL_UNMIX, * LPPMSG_REQ_JEWEL_UNMIX;

typedef struct
{
	PBMSG_HEADER2		m_Header;

	int		iMasterLevelSkill;
}PMSG_REQ_MASTERLEVEL_SKILL, * LPPMSG_REQ_MASTERLEVEL_SKILL;

typedef struct
{
	PBMSG_HEADER2		m_Header;
	uint8_t				m_iResult;
}PMSG_ANS_JEWEL_UNMIX, * LPPMSG_ANS_JEWEL_UNMIX;

//--------------------------------------------------------------------------
// GC [0xBD][0x00]
//--------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t			btOccupationState;
	uint8_t			btCrywolfState;

} PMSG_ANS_CRYWOLF_INFO, * LPPMSG_ANS_CRYWOLF_INFO;

//--------------------------------------------------------------------------
// GC [0xBD][0x02] 방어막, 제단 상태 정보
//--------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	INT				iCrywolfStatueHP;
	uint8_t			btAltarState1;
	uint8_t			btAltarState2;
	uint8_t			btAltarState3;
	uint8_t			btAltarState4;
	uint8_t			btAltarState5;

} PMSG_ANS_CRYWOLF_STATE_ALTAR_INFO, * LPPMSG_ANS_CRYWOLF_STATE_ALTAR_INFO;

//--------------------------------------------------------------------------
// GC [0xBD][0x03]
//--------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t			bResult;
	uint8_t			btAltarState;
	uint8_t			btObjIndexH;
	uint8_t			btObjIndexL;

} PMSG_ANS_CRYWOLF_ALTAR_CONTRACT, * LPPPMSG_ANS_CRYWOLF_ALTAR_CONTRACT;

//--------------------------------------------------------------------------
// GC [0xBD][0x04]
//--------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t			btHour;
	uint8_t			btMinute;

} PMSG_ANS_CRYWOLF_LEFTTIME, * LPPPMSG_ANS_CRYWOLF_LEFTTIME;

//--------------------------------------------------------------------------
// GC [0xBD][0x0C]
//--------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2       h;

	uint8_t                btObjClassH;
	uint8_t                btObjClassL;

	uint8_t                btSourceX;
	uint8_t                btSourceY;

	uint8_t                btPointX;
	uint8_t                btPointY;
} PMSG_NOTIFY_REGION_MONSTER_ATTACK, * LPPMSG_NOTIFY_REGION_MONSTER_ATTACK;

typedef struct
{
	PBMSG_HEADER2	h;
	int				btBossHP;
	uint8_t			btMonster2;

} PMSG_ANS_CRYWOLF_BOSSMONSTER_INFO, * LPPMSG_ANS_CRYWOLF_BOSSMONSTER_INFO;


typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t			btPlusChaosRate;
} PMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE, * LPPMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE;

typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t			btRank;			//0 : D    1 : C   2 : B   3 : A   4 : S
	int				iGettingExp;
} PMSG_ANS_CRYWOLF_PERSONAL_RANK, * LPPMSG_ANS_CRYWOLF_PERSONAL_RANK;

typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t			btCount;
} PMSG_ANS_CRYWOLF_HERO_LIST_INFO_COUNT, * LPPMSG_ANS_CRYWOLF_HERO_LIST_INFO_COUNT;

typedef struct
{
	uint8_t iRank;
	char szHeroName[MAX_ID_SIZE];
	int iHeroScore;
	uint8_t btHeroClass;
} PMSG_ANS_CRYWOLF_HERO_LIST_INFO, * LPPMSG_ANS_CRYWOLF_HERO_LIST_INFO;

//----------------------------------------------------------------------------
// CG [0xD1][0x00]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t	btState;
	uint8_t	btDetailState;
	uint8_t	btEnter;
	uint8_t	btUserCount;
	int		iRemainTime;
} PMSG_ANS_KANTURU_STATE_INFO, * LPPMSG_ANS_KANTURU_STATE_INFO;

//----------------------------------------------------------------------------
// GC [0xD1][0x01]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t		btResult;
} PMSG_ANS_ENTER_KANTURU_BOSS_MAP, * LPPMSG_ANS_ENTER_KANTURU_BOSS_MAP;

//----------------------------------------------------------------------------
// GC [0xD1][0x02]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t		btCurrentState;
	uint8_t		btCurrentDetailState;
} PMSG_ANS_KANTURU_CURRENT_STATE, * LPPMSG_ANS_KANTURU_CURRENT_STATE;

//----------------------------------------------------------------------------
// GC [0xD1][0x03]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t		btState;
	uint8_t		btDetailState;

} PMSG_ANS_KANTURU_STATE_CHANGE, * LPPMSG_ANS_KANTURU_STATE_CHANGE;

//----------------------------------------------------------------------------
// GC [0xD1][0x04]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t		btResult;

} PMSG_ANS_KANTURU_BATTLE_RESULT, * LPPMSG_ANS_KANTURU_BATTLE_RESULT;

//----------------------------------------------------------------------------
// GC [0xD1][0x05]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	int			btTimeLimit;

} PMSG_ANS_KANTURU_BATTLE_SCENE_TIMELIMIT, * LPPMSG_ANS_KANTURU_BATTLE_SCENE_TIMELIMIT;

//----------------------------------------------------------------------------
// GC [0xD1][0x06]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t		btObjClassH;
	uint8_t		btObjClassL;

	uint8_t		btType;

} PMSG_NOTIFY_KANTURU_WIDE_AREA_ATTACK, * LPPMSG_NOTIFY_KANTURU_WIDE_AREA_ATTACK;

//----------------------------------------------------------------------------
// GC [0xD1][0x07]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t		bMonsterCount;
	uint8_t		btUserCount;

} PMSG_NOTIFY_KANTURU_USER_MONSTER_COUNT, * LPPMSG_NOTIFY_KANTURU_USER_MONSTER_COUNT;


//----------------------------------------------------------------------------
// CG [0xBF][0x00]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t			btCursedTempleIdx;
	uint8_t			iItemPos;
} PMSG_REQ_ENTER_CURSED_TEMPLE, * LPPMSG_REQ_ENTER_CURSED_TEMPLE;

//----------------------------------------------------------------------------
// GC [0xBF][0x00]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t			Result;
} PMSG_RESULT_ENTER_CURSED_TEMPLE, * LPPMSG_RESULT_ENTER_CURSED_TEMPLE;

//----------------------------------------------------------------------------
// GC [0xBF][0x01]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint16_t		wRemainSec;
	uint16_t		btUserIndex;
	uint8_t		btX;
	uint8_t		btY;
	uint8_t		btAlliedPoint;
	uint8_t		btIllusionPoint;
	uint8_t		btMyTeam;

	uint8_t		btPartyCount;
} PMSG_CURSED_TAMPLE_STATE, * LPPMSG_CURSED_TAMPLE_STATE;

typedef struct
{
	uint16_t		wPartyUserIndex;
	uint8_t		byMapNumber;
	uint8_t		btX;
	uint8_t		btY;
} PMSG_CURSED_TAMPLE_PARTY_POS, * LPPMSG_CURSED_TAMPLE_PARTY_POS;

//----------------------------------------------------------------------------
// CG [0xBF][0x02]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t          MagicH;
	uint8_t          MagicL;
	uint16_t		wTargetObjIndex;
	uint8_t		Dis;
} PMSG_CURSED_TEMPLE_USE_MAGIC, * LPPMSG_CURSED_TEMPLE_USE_MAGIC;

//----------------------------------------------------------------------------
// GC [0xBF][0x03]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t		btUserCount[6];
} PMSG_CURSED_TEMPLE_USER_COUNT, * LPPMSG_CURSED_TEMPLE_USER_COUNT;

//----------------------------------------------------------------------------
// GC [0xBF][0x02]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t        MagicResult;
	uint8_t          MagicH;
	uint8_t          MagicL;
	uint16_t		wSourceObjIndex;
	uint16_t		wTargetObjIndex;
} PMSG_CURSED_TEMPLE_USE_MAGIC_RESULT, * LPPMSG_CURSED_TEMPLE_USE_MAGIC_RESULT;

//----------------------------------------------------------------------------
// GC [0xBF][0x04]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t		btAlliedPoint;
	uint8_t		btIllusionPoint;

	uint8_t		btUserCount;
} PMSG_CURSED_TEMPLE_RESULT, * LPPMSG_CURSED_TEMPLE_RESULT;

typedef struct
{
	char		GameId[MAX_ID_SIZE];
	uint8_t		byMapNumber;
	uint8_t		btTeam;
	uint8_t		btClass;
	int			nAddExp;
} PMSG_CURSED_TEMPLE_USER_ADD_EXP, * LPPMSG_CURSED_TEMPLE_USER_ADD_EXP;


//----------------------------------------------------------------------------
// GC [0xBF][0x06]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
	uint8_t		btSkillPoint;
} PMSG_CURSED_TEMPLE_SKILL_POINT, * LPPMSG_CURSED_TEMPLE_SKILL_POINT;

//----------------------------------------------------------------------------
// GC [0xBF][0x07]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t          MagicH;
	uint8_t          MagicL;

	uint16_t		wObjIndex;
} PMSG_CURSED_TEMPLE_SKILL_END, * LPPMSG_CURSED_TEMPLE_SKILL_END;

//----------------------------------------------------------------------------
// GC [0xBF][0x08]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
	uint16_t		wUserIndex;
	char		Name[MAX_ID_SIZE];
} PMSG_RELICS_GET_USER, * LPPMSG_RELICS_GET_USER;

//----------------------------------------------------------------------------
// GC [0xBF][0x09]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
	uint8_t		btTempleNumber;
	uint8_t		btIllusionTempleState;	// 0: wait, 1: wait->ready, 2: ready->play, 3: play->end, 
} PMSG_ILLUSION_TEMPLE_STATE, * LPPMSG_ILLUSION_TEMPLE_STATE;

//----------------------------------------------------------------------------
// GC [0xBF][0x0a]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	Header;

	uint8_t			MagicH;
	uint8_t			MagicL;

	uint16_t			wUserIndex;
	uint8_t			byCount;
} PRECEIVE_CHAIN_MAGIC, * LPPRECEIVE_CHAIN_MAGIC;

typedef struct
{
	uint16_t			wTargetIndex;
} PRECEIVE_CHAIN_MAGIC_OBJECT, * LPPRECEIVE_CHAIN_MAGIC_OBJECT;

typedef struct
{
	PSBMSG_HEADER h;
	uint16_t nMLevel;
	uint16_t nMLPoint;
	uint32_t LifeMax;
	uint32_t ManaMax;
	uint32_t ShieldMax;
	uint32_t SkillManaMax;
	uint8_t btMExp[8];
	uint8_t btMBaseExp[8];
	uint8_t btMNextExp[8];
} PMSG_MASTERLEVEL_INFO, * LPPMSG_MASTERLEVEL_INFO;

typedef struct
{
	PSBMSG_HEADER h;
	uint16_t Level;
	uint16_t LevelUpPoint;
	uint16_t MinLevel;
	uint16_t MaxLevel;

	uint32_t LifeMax;
	uint32_t ManaMax;
	uint32_t ShieldMax;
	uint32_t SkillManaMax;

	uint8_t btMExp[8];
	uint8_t btMBaseExp[8];
	uint8_t btMNextExp[8];
} PMSG_MASTERLEVEL_UP, * LPPMSG_MASTERLEVEL_UP;

typedef struct
{
	PBMSG_HEADER	h;
	uint8_t		subcode;
	uint8_t		btResult;
	short       nMLPoint;
	int			nSkillNum;
	int			nSkillLevel;
	float		fSkillValue;
	float		fSkillNextValue;
	uint8_t		bySkillValueType;
} PMSG_ANS_MASTERLEVEL_SKILL, * LPPMSG_ANS_MASTERLEVEL_SKILL;

typedef struct PMSG_ANS_MASTERLEVEL_SKILL_LIST_COUNT
{
	PWMSG_HEADER h; // C2:F3:53 GS->C
	uint8_t subcode;
	int nCount;
} *LPPMSG_ANS_MASTERLEVEL_SKILL_LIST_COUNT;

typedef struct PMSG_ANS_MASTERLEVEL_SKILL_LIST
{
	uint8_t byIndex;
	uint8_t byPoint;
	float fValue;
	float fNextValue;
} *LPPMSG_ANS_MASTERLEVEL_SKILL_LIST;
//----------------------------------------------------------------------------
// GC [0xD1][0x10] 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t			btState;
	uint8_t			btDetailState;

	uint8_t			btEnter;
	int				iRemainTime;
} PMSG_ANS_RAKLION_STATE_INFO, * LPPMSG_ANS_RAKLION_STATE_INFO;

//----------------------------------------------------------------------------
// GC [0xD1][0x11] 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t			btCurrentState;
	uint8_t			btCurrentDetailState;
} PMSG_ANS_RAKLION_CURRENT_STATE, * LPPMSG_ANS_RAKLION_CURRENT_STATE;

//----------------------------------------------------------------------------
// GC [0xD1][0x12]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t			btState;
	uint8_t			btDetailState;
} PMSG_ANS_RAKLION_STATE_CHANGE, * LPPMSG_ANS_RAKLION_STATE_CHANGE;

//----------------------------------------------------------------------------
// GC [0xD1][0x13]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t			btResult;
} PMSG_ANS_RAKLION_BATTLE_RESULT, * LPPMSG_ANS_RAKLION_BATTLE_RESULT;

//----------------------------------------------------------------------------
// GC [0xD1][0x14]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t			btObjClassH;
	uint8_t			btObjClassL;

	uint8_t			btType;

} PMSG_NOTIFY_RAKLION_WIDE_AREA_ATTACK, * LPPMSG_NOTIFY_RAKLION_WIDE_AREA_ATTACK;

//----------------------------------------------------------------------------
// GC [0xD1][0x15]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;

	uint8_t			btMonsterCount;
	uint8_t			btUserCount;
} PMSG_NOTIFY_RAKLION_USER_MONSTER_COUNT, * LPPMSG_NOTIFY_RAKLION_USER_MONSTER_COUNT;

//----------------------------------------------------------------------------
// CG[0xBF][0x0b]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
	int			nRegCoinCnt;
} PMSG_REQ_GET_COIN_COUNT, * LPPMSG_REQ_GET_COIN_COUNT;

//----------------------------------------------------------------------------
// GC[0xBF][0x0b]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
	int			nCoinCnt;
}PMSG_ANS_GET_COIN_COUNT, * LPPMSG_ANS_GET_COIN_COUNT;

//----------------------------------------------------------------------------
// CG[0xBF][0x0c]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
}PMSG_REQ_REGEIST_COIN, * LPPMSG_REQ_REGEIST_COIN;

//----------------------------------------------------------------------------
// GC0xBF][0x0c]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
	uint8_t		btResult;
	int			nCurCoinCnt;
}PMSG_ANS_REGEIST_COIN, * LPPMSG_ANS_REGEIST_COIN;

//----------------------------------------------------------------------------
// CG[0xBF][0x0d]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
	int			nCoinCnt;
}PMSG_REQ_TRADE_COIN, * LPPMSG_REG_TREADE_COIN;

//----------------------------------------------------------------------------
// GC[0xBF][0x0d]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
	uint8_t		btResult;
}PMSG_ANS_TRADE_COIN, * LPPMSG_ANS_TREADE_COIN;

//----------------------------------------------------------------------------
// GC [0xBF][0x0E]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t		btResult;
} PMSG_RESULT_ENTER_DOPPELGANGER, * LPPMSG_RESULT_ENTER_DOPPELGANGER;
//----------------------------------------------------------------------------
// GC [0xBF][0x0F]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t		btPosIndex;
} PMSG_DOPPELGANGER_MONSTER_POSITION, * LPPMSG_DOPPELGANGER_MONSTER_POSITION;
//----------------------------------------------------------------------------
// GC [0xBF][0x10]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
	uint8_t		btDoppelgangerState;	// 0: wait, 1: wait->ready, 2: ready->play, 3: play->end, 
} PMSG_DOPPELGANGER_STATE, * LPPMSG_DOPPELGANGER_STATE;
//----------------------------------------------------------------------------
// GC [0xBF][0x11]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
	uint8_t		btIceworkerState;
	uint8_t		btPosIndex;
} PMSG_DOPPELGANGER_ICEWORKER_STATE, * LPPMSG_DOPPELGANGER_ICEWORKER_STATE;
//----------------------------------------------------------------------------
// GC [0xBF][0x12] 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint16_t		wRemainSec;
	uint8_t		btUserCount;
	uint8_t		btDummy;
	uint8_t		UserPosData;
} PMSG_DOPPELGANGER_PLAY_INFO, * LPPMSG_DOPPELGANGER_PLAY_INFO;

typedef struct
{
	uint16_t		wUserIndex;
	uint8_t		byMapNumber;
	uint8_t		btPosIndex;
} PMSG_DOPPELGANGER_USER_POS, * LPPMSG_DOPPELGANGER_USER_POS;
//----------------------------------------------------------------------------
// GC [0xBF][0x13]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t		btResult;
	DWORD		dwRewardExp;
}PMSG_DOPPELGANGER_RESULT, * LPPMSG_DOPPELGANGER_RESULT;
//----------------------------------------------------------------------------
// GC [0xBF][0x14]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
	uint8_t		btMaxGoalCnt;
	uint8_t		btGoalCnt;
}PMSG_DOPPELGANGER_MONSTER_GOAL, * LPPMSG_DOPPELGANGER_MONSTER_GOAL;

#ifdef PBG_ADD_SECRETBUFF
//----------------------------------------------------------------------------
// GC [0xBF][0x15]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2 h;
	uint8_t		btFatiguePercentage;
}PMSG_FATIGUEPERCENTAGE, * LPPMSG_FATIGUEPERCENTAGE;
#endif //PBG_ADD_SECRETBUFF


#ifdef LJH_ADD_SYSTEM_OF_EQUIPPING_ITEM_FROM_INVENTORY
//----------------------------------------------------------------------------
// GC [0xBF][0x20] 
//----------------------------------------------------------------------------
typedef struct _tagPMSG_ANS_INVENTORY_EQUIPMENT_ITEM
{
	PBMSG_HEADER2	h;

	uint8_t	btItemPos;
	uint8_t	btResult;
} PMSG_ANS_INVENTORY_EQUIPMENT_ITEM, * LPPMSG_ANS_INVENTORY_EQUIPMENT_ITEM;
#endif //LJH_ADD_SYSTEM_OF_EQUIPPING_ITEM_FROM_INVENTORY

//----------------------------------------------------------------------------
// GC[0x8E][0x01]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	DWORD		dwKeyValue;
}PMSG_MAPMOVE_CHECKSUM, * LPPMSG_MAPMOVE_CHECKSUM;

//----------------------------------------------------------------------------
// GC[0x8E][0x03]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t		btResult;
}PMSG_ANS_MAPMOVE, * LPPMSG_ANS_MAPMOVE;

//----------------------------------------------------------------------------
// GC [0xF7][0x02]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t		Result;
	uint8_t		Day;
	uint8_t		Zone;
	uint8_t		Wheather;
	DWORD		RemainTick;
} PMSG_RESULT_ENTER_EMPIREGUARDIAN, * LPPMSG_RESULT_ENTER_EMPIREGUARDIAN;

//----------------------------------------------------------------------------
// GC [0xF7][0x04]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t		Type;
	DWORD		RemainTick;
	uint8_t		MonsterCount;
} PMSG_REMAINTICK_EMPIREGUARDIAN, * LPPMSG_REMAINTICK_EMPIREGUARDIAN;

//----------------------------------------------------------------------------
// GC [0xF7][0x06]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2	h;
	uint8_t		Result;
	DWORD		Exp;
} PMSG_CLEAR_RESULT_EMPIREGUARDIAN, * LPPMSG_CLEAR_RESULT_EMPIREGUARDIAN;

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// (0xD2)(0x01)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;
}PMSG_CASHSHOP_CASHPOINT_REQ, * LPPMSG_CASHSHOP_CASHPOINT_REQ;

//----------------------------------------------------------------------------
// (0xD2)(0x01)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	uint8_t				btViewType;

	double				dTotalCash;
	double				dCashCredit;		// C (CreditCard)
	double				dCashPrepaid;		// P (PrepaidCard)
	double				dTotalPoint;
	double				dTotalMileage;
}PMSG_CASHSHOP_CASHPOINT_ANS, * LPPMSG_CASHSHOP_CASHPOINT_ANS;

//----------------------------------------------------------------------------
// (0xD2)(0x02)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	uint8_t				byShopOpenType;
}PMSG_CASHSHOP_SHOPOPEN_REQ, * LPPMSG_CASHSHOP_SHOPOPEN_REQ;

//----------------------------------------------------------------------------
// (0xD2)(0x02)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	uint8_t				byShopOpenResult;
}PMSG_CASHSHOP_SHOPOPEN_ANS, * LPPMSG_CASHSHOP_SHOPOPEN_ANS;

//----------------------------------------------------------------------------
// (0xD2)(0x03)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	long				lBuyItemPackageSeq;
	long				lBuyItemDisplaySeq;
	long				lBuyItemPriceSeq;
	uint16_t				wItemCode;
}PMSG_CASHSHOP_BUYITEM_REQ, * LPPMSG_CASHSHOP_BUYITEM_REQ;

//----------------------------------------------------------------------------
// 아이템 구매 결과 (0xD2)(0x03)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	uint8_t				byResultCode;
	long				lItemLeftCount;
}PMSG_CASHSHOP_BUYITEM_ANS, * LPPMSG_CASHSHOP_BUYITEM_ANS;

//----------------------------------------------------------------------------
// (0xD2)(0x04)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	long				lGiftItemPackageSeq;
	long				lDiftItemDisplaySeq;
	long				lGiftItemPriceSeq;
	long				lSaleZone;
	char				chReceiveUserID[MAX_ID_SIZE + 1];
	char				chMessage[MAX_GIFT_MESSAGE_SIZE];
}PMSG_CASHSHOP_GIFTSEND_REQ, * LPPMSG_CASHSHOP_GIFTSEND_REQ;

//----------------------------------------------------------------------------
// (0xD2)(0x04)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	uint8_t				byResultCode;
	long				lItemLeftCount;
	double				dLimitedCash;
}PMSG_CASHSHOP_GIFTSEND_ANS, * LPPMSG_CASHSHOP_GIFTSEND_ANS;

//----------------------------------------------------------------------------
// (0xD2)(0x05)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;
	int					iPageIndex;
	char				chStorageType;
}PMSG_CASHSHOP_STORAGELIST_REQ, * LPPMSG_CASHSHOP_STORAGELIST_REQ;

//----------------------------------------------------------------------------
// (0xD2)(0x06)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;
	uint16_t				wTotalItemCount;
	uint16_t				wCurrentItemCount;
	uint16_t				wPageIndex;
	uint16_t				wTotalPage;
}PMSG_CASHSHOP_STORAGECOUNT, * LPPMSG_CASHSHOP_STORAGECOUNT;

//----------------------------------------------------------------------------
// (0xD2)(0x0D)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	long				lStorageIndex;
	long				lItemSeq;
	long				lStorageGroupCode;
	long				lProductSeq;
	long				lPriceSeq;
	double				dCashPoint;
	char				chItemType;
}PMSG_CASHSHOP_STORAGELIST, * LPPMSG_CASHSHOP_STORAGELIST;

//----------------------------------------------------------------------------
// (0xD2)(0x0E)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	long				lStorageIndex;
	long				lItemSeq;
	long				lStorageGroupCode;
	long				lProductSeq;
	long				lPriceSeq;
	double				dCashPoint;
	char				chItemType;

	char				chSendUserName[MAX_ID_SIZE + 1];
	char				chMessage[MAX_GIFT_MESSAGE_SIZE];
}PMSG_CASHSHOP_GIFTSTORAGELIST, * LPPMSG_CASHSHOP_GIFTSTORAGELIST;

//----------------------------------------------------------------------------
// (0xD2)(0x07)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	double				dCashValue;

	char				chReceiveUserID[MAX_ID_SIZE + 1];
	char				chMessage[MAX_GIFT_MESSAGE_SIZE];
}PMSG_CASHSHOP_CASHSEND_REQ, * LPPMSG_CASHSHOP_CASHSEND_REQ;

//----------------------------------------------------------------------------
// (0xD2)(0x07)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	double				dGiftCashLimit;
	uint8_t				byResultCode;
}PMSG_CASHSHOP_CASHSEND_ANS, * LPPMSG_CASHSHOP_CASHSEND_ANS;

//----------------------------------------------------------------------------
// (0xD2)(0x08)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;
}PMSG_CASHSHOP_ITEMBUY_CONFIRM_REQ, * LPPMSG_CASHSHOP_ITEMBUY_CONFIRM_REQ;

//----------------------------------------------------------------------------
// (0xD2)(0x08)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	double				dPresentedCash;
	double				dPresenteLimitCash;

	uint8_t				byResult;
	uint8_t				byItemBuyPossible;
	uint8_t				byPresendPossible;
}PMSG_CASHSHOP_ITEMBUY_CONFIRM_ANS, * LPPMSG_CASHSHOP_ITEMBUY_CONFIRM_ANS;

//----------------------------------------------------------------------------
// (0xD2)(0x09)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	long				lPackageSeq;
}PMSG_CASHSHOP_ITEMBUY_LEFT_COUNT_REQ, * LPPMSG_CASHSHOP_ITEMBUY_LEFT_COUNT_REQ;

//----------------------------------------------------------------------------
// (0xD2)(0x09)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	long				lPackageSeq;
	long				lLeftCount;
}PMSG_CASHSHOP_ITEMBUY_LEFT_COUNT_ANS, * LPPMSG_CASHSHOP_ITEMBUY_LEFT_COUNT_ANS;

//----------------------------------------------------------------------------
// (0xD2)(0x0A)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	long				lStorageSeq;
	long				lStorageItemSeq;
	char				chStorageItemType;
}PMSG_CASHSHOP_STORAGE_ITEM_THROW_REQ, * LPPMSG_CASHSHOP_STORAGE_ITEM_THROW_REQ;

//----------------------------------------------------------------------------
// (0xD2)(0x0A)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	uint8_t				byResult;
}PMSG_CASHSHOP_STORAGE_ITEM_THROW_ANS, * LPPMSG_CASHSHOP_STORAGE_ITEM_THROW_ANS;

//----------------------------------------------------------------------------
// (0xD2)(0x0B)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	long				lStorageSeq;
	long				lStorageItemSeq;

	uint16_t				wItemCode;
	char				chStorageItemType;
}PMSG_CASHSHOP_STORAGE_ITEM_USE_REQ, * LPPMSG_CASHSHOP_STORAGE_ITEM_USE_REQ;

//----------------------------------------------------------------------------
// (0xD2)(0x0B)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	uint8_t				byResult;
}PMSG_CASHSHOP_STORAGE_ITEM_USE_ANS, * LPPMSG_CASHSHOP_STORAGE_ITEM_USE_ANS;

//----------------------------------------------------------------------------
// (0xD2)(0x0C)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2		h;

	uint16_t				wSaleZone;
	uint16_t				wYear;
	uint16_t				wYearIdentify;
}PMSG_CASHSHOP_VERSION_UPDATE, * LPPMSG_CASHSHOP_VERSION_UPDATE;

//----------------------------------------------------------------------------
// (0xD2)(0x13)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2			h;

	long				lEventCategorySeq;
}PMSG_CASHSHOP_EVENTITEM_REQ, * LPPMSG_CASHSHOP_EVENTITEM_REQ;


//----------------------------------------------------------------------------
// (0xD2)(0x13)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER2			h;
	uint16_t				wEventItemListCount;
}PMSG_CASHSHOP_EVENTITEM_COUNT, * LPPMSG_CASHSHOP_EVENTITEM_COUNT;


typedef struct
{
	PBMSG_HEADER2			h;
	long				lPackageSeq[INGAMESHOP_DISPLAY_ITEMLIST_SIZE];
}PMSG_CASHSHOP_EVENTITEM_LIST, * LPPMSG_CASHSHOP_EVENTITEM_LIST;

typedef struct
{
	PBMSG_HEADER2			h;
	uint16_t				wBannerZone;
	uint16_t				wYear;
	uint16_t				wYearIdentify;
}PMSG_CASHSHOP_BANNER_UPDATE, * LPPMSG_CASHSHOP_BANNER_UPDATE;

#pragma pack(pop)

#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM


#ifdef KJH_ADD_PERIOD_ITEM_SYSTEM

typedef struct
{
	PBMSG_HEADER2		h;
	uint8_t				byPeriodItemCount;
}PMSG_PERIODITEMEX_ITEMCOUNT, * LPPMSG_PERIODITEMEX_ITEMCOUNT;


typedef struct
{
	PBMSG_HEADER2		h;
	uint16_t			wItemCode;
	uint16_t			wItemSlotIndex;
	long				lExpireDate;
}PMSG_PERIODITEMEX_ITEMLIST, * LPPMSG_PERIODITEMEX_ITEMLIST;

#endif // KJH_ADD_PERIOD_ITEM_SYSTEM


#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
//----------------------------------------------------------------------------
// 0x4A
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER  Header;
	uint8_t          AttackH;
	uint8_t          AttackL;
	uint8_t          SourceKeyH;
	uint8_t          SourceKeyL;
	uint8_t          TargetKeyH;
	uint8_t          TargetKeyL;
} PRECEIVE_STRAIGHTATTACK, * LPPRECEIVE_STRAIGHTATTACK;
//----------------------------------------------------------------------------
// 0x4B
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEADER Header;
	uint16_t		 MagicNumber;
	uint16_t		 TargerIndex[DARKSIDE_TARGET_MAX];
} PRECEIVE_DARKSIDE_INDEX, * LPPRECEIVE_DARKSIDE_INDEX;
#endif //PBG_ADD_NEWCHAR_MONK_SKILL


typedef struct
{
	PSBMSG_HEADER Header;

	DWORD m_PowerATK;
	DWORD m_KillCount;
	DWORD m_DeadCount;

	DWORD m_DoubleDmgRate;
	DWORD m_TripleDmgRate;
	DWORD m_CriticalDmgRate;
	DWORD m_ExcellentDmgRate;
	DWORD m_IncIgnoreShield;
	DWORD m_IncIgnoreDefense;
	DWORD AttackSuccessRate;
	//--
	DWORD m_ReflectDmg;
	DWORD m_ReflectDmgRate;
	DWORD m_ResIgnoreShield;
	DWORD m_ResIgnoreDefense;
	DWORD m_DmgReductionRate;
	DWORD m_ResDoubleDmgRate;
	DWORD m_ResAttackStunRate;
	DWORD m_ResCriticalDmgRate;
	DWORD m_ResExcellentDmgRate;
	DWORD DefenseSuccessRate;

	DWORD DefFullHPRestoreRate;
	DWORD DefFullMPRestoreRate;
	DWORD DefFullSDRestoreRate;
	DWORD DefFullBPRestoreRate;
	DWORD OffFullHPRestoreRate;
	DWORD OffFullMPRestoreRate;
	DWORD OffFullSDRestoreRate;
	DWORD OffFullBPRestoreRate;
}PRECEIVE_CHARACTER_ADVANCE, * LPPRECEIVE_CHARACTER_ADVANCE;
//////////////////////////////////////////////////////////////////////////
// ?????????????????????????????????????
//////////////////////////////////////////////////////////////////////////
extern CWsctlc SocketClient;
extern int HeroKey;

extern int SummonLife;
extern int SendGetItem;
extern int SendDropItem;

extern bool EnableGuildWar;
extern int  GuildWarIndex;
extern char GuildWarName[8 + 1];
extern int  GuildWarScore[2];

extern bool EnableSoccer;
extern uint8_t HeroSoccerTeam;
extern int  SoccerTime;
extern char SoccerTeamName[2][8 + 1];
extern bool SoccerObserver;

#ifdef PKD_ADD_ENHANCED_ENCRYPTION
extern CSessionCryptor g_SessionCryptorCS;
extern CSessionCryptor g_SessionCryptorSC;
#else
extern CSimpleModulus g_SimpleModulusCS;
extern CSimpleModulus g_SimpleModulusSC;
#endif // PKD_ADD_ENHANCED_ENCRYPTION

#ifdef ACC_PACKETSIZE
extern int g_iTotalPacketRecv;
extern int g_iTotalPacketSend;
extern DWORD g_dwPacketInitialTick;
#endif //ACC_PACKETSIZE


extern BOOL CreateSocket(char* IpAddr, unsigned short Port);
extern void DeleteSocket();
extern void ProtocolCompiler(CWsctlc* pSocketClient = &SocketClient, int iTranslation = 0, int iParam = 0);
extern void ReceiveCharacterList(uint8_t* ReceiveBuffer);
extern void ReceiveMovePosition(uint8_t* ReceiveBuffer);
extern void ReceiveMoveCharacter(uint8_t* ReceiveBuffer);
extern BOOL TranslateProtocol(int HeadCode, uint8_t* ReceiveBuffer, int Size, BOOL bEncrypted);

void InitGame();
void InitGuildWar();


bool Check_Switch(PRECEIVE_CROWN_SWITCH_INFO* Data);
bool Delete_Switch();

typedef struct _CROWN_SWITCH_INFO
{
	uint8_t				m_bySwitchState;
	uint8_t				m_JoinSide;
	uint8_t				m_szGuildName[9];
	uint8_t				m_szUserName[MAX_ID_SIZE + 1];

	_CROWN_SWITCH_INFO()
	{
		Reset();
	}

	void Reset()
	{
		m_bySwitchState = 0;
		m_JoinSide = 0;
		ZeroMemory(m_szGuildName, sizeof(m_szGuildName));
		ZeroMemory(m_szUserName, sizeof(m_szUserName));
	}
} CROWN_SWITCH_INFO;
extern CROWN_SWITCH_INFO* Switch_Info;

extern MASTER_LEVEL_VALUE	Master_Level_Data;