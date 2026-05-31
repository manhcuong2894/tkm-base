#pragma once

#if(GAMESERVER_UPDATE>=701)
#include "..\\..\\Util\\mapm\\M_APM.h"
#endif
#include "User.h"

#if(GAMESERVER_LANGUAGE==0)
#define PROTOCOL_CODE1 0xD3
#define PROTOCOL_CODE2 0xD7
#define PROTOCOL_CODE3 0xDF
#define PROTOCOL_CODE4 0x10
#elif(GAMESERVER_LANGUAGE==1)
#define PROTOCOL_CODE1 0xD4
#define PROTOCOL_CODE2 0x11
#define PROTOCOL_CODE3 0x15
#define PROTOCOL_CODE4 0xDB
#elif(GAMESERVER_LANGUAGE==2)
#define PROTOCOL_CODE1 0x1D
#define PROTOCOL_CODE2 0xDC
#define PROTOCOL_CODE3 0xD6
#define PROTOCOL_CODE4 0xD7
#elif(GAMESERVER_LANGUAGE==3)
#define PROTOCOL_CODE1 0xD9
#define PROTOCOL_CODE2 0xD7
#define PROTOCOL_CODE3 0xD0
#define PROTOCOL_CODE4 0x1D
#elif(GAMESERVER_LANGUAGE==4)
#define PROTOCOL_CODE1 0x00
#define PROTOCOL_CODE2 0x00
#define PROTOCOL_CODE3 0x00
#define PROTOCOL_CODE4 0x00
#elif(GAMESERVER_LANGUAGE==5)
#define PROTOCOL_CODE1 0xD6
#define PROTOCOL_CODE2 0xDD
#define PROTOCOL_CODE3 0xDF
#define PROTOCOL_CODE4 0xD2
#elif(GAMESERVER_LANGUAGE==6)
#define PROTOCOL_CODE1 0xDD
#define PROTOCOL_CODE2 0xD6
#define PROTOCOL_CODE3 0xDF
#define PROTOCOL_CODE4 0x11
#elif(GAMESERVER_LANGUAGE==7)
#define PROTOCOL_CODE1 0xD9
#define PROTOCOL_CODE2 0x15
#define PROTOCOL_CODE3 0xDC
#define PROTOCOL_CODE4 0x1D
#endif

#define SET_NUMBERHB(x) ((uint8_t)((DWORD)(x)>>(DWORD)8))
#define SET_NUMBERLB(x) ((uint8_t)((DWORD)(x)&0xFF))
#define SET_NUMBERHW(x) ((uint16_t)((DWORD)(x)>>(DWORD)16))
#define SET_NUMBERLW(x) ((uint16_t)((DWORD)(x)&0xFFFF))
#define SET_NUMBERHDW(x) ((DWORD)((QWORD)(x)>>(QWORD)32))
#define SET_NUMBERLDW(x) ((DWORD)((QWORD)(x)&0xFFFFFFFF))

#define MAKE_NUMBERW(x,y) ((uint16_t)(((uint8_t)((y)&0xFF))|((uint8_t)((x)&0xFF)<<8)))
#define MAKE_NUMBERDW(x,y) ((DWORD)(((uint16_t)((y)&0xFFFF))|((uint16_t)((x)&0xFFFF)<<16)))
#define MAKE_NUMBERQW(x,y) ((QWORD)(((DWORD)((y)&0xFFFFFFFF))|((DWORD)((x)&0xFFFFFFFF)<<32)))

//**********************************************//
//************ Packet Base *********************//
//**********************************************//

struct PBMSG_HEAD
{
	void set(uint8_t head, uint8_t size) // OK
	{
		this->type = 0xC1;
		this->size = size;
		this->head = head;
	}

	void setE(uint8_t head, uint8_t size) // OK
	{
		this->type = 0xC3;
		this->size = size;
		this->head = head;
	}

	void SetSize(uint8_t size)
	{
		this->size = size;
	}

	uint8_t type;
	uint8_t size;
	uint8_t head;
};

struct PSBMSG_HEAD
{
	void set(uint8_t head, uint8_t subh, uint8_t size) // OK
	{
		this->type = 0xC1;
		this->size = size;
		this->head = head;
		this->subh = subh;
	}

	void setE(uint8_t head, uint8_t subh, uint8_t size) // OK
	{
		this->type = 0xC3;
		this->size = size;
		this->head = head;
		this->subh = subh;
	}

	void SetSize(uint8_t size)
	{
		this->size = size;
	}

	uint8_t type;
	uint8_t size;
	uint8_t head;
	uint8_t subh;
};

struct PWMSG_HEAD
{
	void set(uint8_t head, uint16_t size) // OK
	{
		this->type = 0xC2;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
	}

	void setE(uint8_t head, uint16_t size) // OK
	{
		this->type = 0xC4;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
	}

	void SetSize(uint16_t size)
	{
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
	}

	uint8_t type;
	uint8_t size[2];
	uint8_t head;
};

struct PSWMSG_HEAD
{
	void set(uint8_t head, uint8_t subh, uint16_t size) // OK
	{
		this->type = 0xC2;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
		this->subh = subh;
	}

	void setE(uint8_t head, uint8_t subh, uint16_t size) // OK
	{
		this->type = 0xC4;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
		this->subh = subh;
	}

	void SetSize(uint16_t size)
	{
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
	}

	uint8_t type;
	uint8_t size[2];
	uint8_t head;
	uint8_t subh;
};

//struct PBMSG_HEAD2	// Packet - Byte Type
//{
//
//public:
//
//	void set(uint8_t head,uint8_t subh,uint8_t size)
//	{
//		this->type = 0xC1;
//		this->size = size;
//		this->head = head;
//		this->subh = subh;
//	};
//
//	uint8_t type;
//	uint8_t size;
//	uint8_t head;
//	uint8_t subh;
//};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_CHAT_RECV
{
	PBMSG_HEAD header; // C1:00
	char name[10];
	char message[MAX_CHAT_SIZE];
};

struct PMSG_CHAT_WHISPER_RECV
{
	PBMSG_HEAD header; // C1:00
	char name[10];
	char message[MAX_CHAT_SIZE];
};

struct PMSG_MAIN_CHECK_RECV
{
	PBMSG_HEAD header; // C1:03
	DWORD key;
};

struct PMSG_LIVE_CLIENT_RECV
{
	PBMSG_HEAD header; // C1:0E
	DWORD TickCount;
	DWORD PhysiSpeed;
	DWORD MagicSpeed;
};

struct PMSG_CHARACTER_OFF_RECV
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE3]
	uint8_t State;
};

struct PMSG_POSITION_RECV
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE3]
	uint8_t x;
	uint8_t y;
};

struct PMSG_ACTION_RECV
{
	PBMSG_HEAD header; // C1:18
	uint8_t dir;
	uint8_t action;
	uint8_t index[2];
};

struct PMSG_EVENT_REMAIN_TIME_RECV
{
	PBMSG_HEAD header; // C1:91
	uint8_t EventType;
	uint8_t ItemLevel;
};

struct PMSG_PET_ITEM_COMMAND_RECV
{
	PBMSG_HEAD header; // C1:A7
	uint8_t type;
	uint8_t command;
	uint8_t index[2];
};

struct PMSG_PET_ITEM_INFO_RECV
{
	PBMSG_HEAD header; // C1:A9
	uint8_t type;
	uint8_t flag;
	uint8_t slot;
};

struct PMSG_MAP_SERVER_MOVE_AUTH_RECV
{
	PSBMSG_HEAD header; // C1:B1:01
	char account[12];
	char name[12];
	DWORD AuthCode1;
	DWORD AuthCode2;
	DWORD AuthCode3;
	DWORD AuthCode4;
	DWORD TickCount;
	uint8_t ClientVersion[5];
	uint8_t ClientSerial[16];
};

struct PMSG_FRIEND_MESSAGE_RECV
{
	PWMSG_HEAD header; // C1:C5
	DWORD guid;
	char name[10];
	char subject[60];
	uint8_t dir;
	uint8_t action;
	uint16_t size;
	char text[1000];
};

struct PMSG_MOVE_RECV
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE1]
	uint8_t x;
	uint8_t y;
	uint8_t path[8];
};

struct PMSG_CONNECT_ACCOUNT_RECV
{
#if(GAMESERVER_LANGUAGE==1)
#pragma pack(1)
	PSBMSG_HEAD header; // C3:F1:01
	char account[10];
	char password[20];
	char ComputerHardwareId[36];
	DWORD TickCount;
	uint8_t ClientVersion[5];
	uint8_t ClientSerial[16];
#if(GAMESERVER_UPDATE>=803)
	uint8_t LanguageCode;
#endif
#pragma pack()
#else
#pragma pack(1)
	PSBMSG_HEAD header; // C3:F1:01
	char account[10];
	char password[12];
	DWORD TickCount;
	uint8_t ClientVersion[5];
	uint8_t ClientSerial[16];
#pragma pack()
#endif
};

struct PMSG_CLOSE_CLIENT_RECV
{
	PSBMSG_HEAD header; // C1:F1:02
	uint8_t type;
};

struct PMSG_CHARACTER_CREATE_RECV
{
	PSBMSG_HEAD header; // C1:F3:01
	char name[10];
	uint8_t Class;
};

struct PMSG_CHARACTER_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:F3:02
	char name[10];
	char PersonalCode[10];
};

typedef struct PMSG_CHARACTER_INFO_RECV
{
	PSBMSG_HEAD header; // C1:F3:03
	char name[10];
}*LPPMSG_CHARACTER_INFO_RECV;

struct PMSG_LEVEL_UP_POINT_RECV
{
	PSBMSG_HEAD header; // C1:F3:06
	uint8_t type;
};

struct PMSG_CHARACTER_NAME_CHECK_RECV
{
	PSBMSG_HEAD header; // C1:F3:15
	char name[10];
};

struct PMSG_CHARACTER_NAME_CHANGE_RECV
{
	PSBMSG_HEAD header; // C1:F3:16
	char OldName[10];
	char NewName[10];
};

struct PMSG_OPTION_CHANGE_SKIN_RECV
{
	PSBMSG_HEAD header; // C1:F3:21
	uint8_t ChangeSkin;
};

struct PMSG_OPTION_DATA_RECV
{
#pragma pack(1)
	PSBMSG_HEAD header; // C1:F3:30
	uint8_t SkillKey[20];
	uint8_t GameOption;
	uint8_t QKey;
	uint8_t WKey;
	uint8_t EKey;
	uint8_t ChatWindow;
	uint8_t RKey;
	DWORD QWERLevel;
#pragma pack()
};

struct PMSG_CLIENT_SECURITY_BREACH_RECV
{
	PSBMSG_HEAD header; // C1:F3:31
	uint8_t code[4];
};

struct PMSG_SNS_DATA_RECV
{
	PWMSG_HEAD header; // C1:FB
	uint8_t result;
	uint8_t data[256];
};

struct PMSG_SNS_DATA_LOG_RECV
{
	PBMSG_HEAD header; // C1:FC
	uint8_t code[3];
};

struct PMSG_OFFTRADE_RECV
{
	PSBMSG_HEAD header;
	int Type;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_CHAT_SEND
{
	PBMSG_HEAD header; // C1:00
	char name[10];
	char message[MAX_CHAT_SIZE];
};

struct PMSG_CHAT_TARGET_SEND
{
	PBMSG_HEAD header; // C1:01
	uint8_t index[2];
	char message[MAX_CHAT_SIZE];
};

struct PMSG_CHAT_WHISPER_SEND
{
	PBMSG_HEAD header; // C1:02
	char name[10];
	char message[MAX_CHAT_SIZE];
};

struct PMSG_MAIN_CHECK_SEND
{
	PBMSG_HEAD header; // C1:03
	uint16_t key;
};

struct PMSG_EVENT_STATE_SEND
{
	PBMSG_HEAD header; // C1:0B
	uint8_t state;
	uint8_t event;
};

struct PMSG_SERVER_MSG_SEND
{
	PBMSG_HEAD header; // C1:0C
	uint8_t MsgNumber;
};

struct PMSG_WEATHER_SEND
{
	PBMSG_HEAD header; // C1:0F
	uint8_t weather;
};

struct PMSG_DAMAGE_SEND
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE2]
	uint8_t index[2];
	uint8_t damage[2];
#if(GAMESERVER_UPDATE>=701)
	uint8_t type[2];
#else
	uint8_t type;
#endif
	uint8_t ShieldDamage[2];
#if(GAMESERVER_UPDATE>=701)
	uint8_t attribute;
#endif
#if(GAMESERVER_EXTRA==1)
	DWORD ViewCurHP;
	DWORD ViewCurSD;
	DWORD ViewDamageHP;
	DWORD ViewDamageSD;
#endif
};

struct PMSG_POSITION_SEND
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE3]
	uint8_t index[2];
	uint8_t x;
	uint8_t y;
};

struct PMSG_MONSTER_DIE_SEND
{
	PBMSG_HEAD header; // C1:16
	uint8_t index[2];
	uint8_t experience[2];
	uint8_t damage[2];
#if(GAMESERVER_EXTRA==1)
	DWORD ViewDamageHP;
#endif
};

struct PMSG_USER_DIE_SEND
{
	PBMSG_HEAD header; // C1:17
	uint8_t index[2];
	uint8_t skill[2];
	uint8_t killer[2];
};

struct PMSG_ACTION_SEND
{
	PBMSG_HEAD header; // C1:18
	uint8_t index[2];
	uint8_t dir;
	uint8_t action;
	uint8_t target[2];
};

struct PMSG_LIFE_SEND
{
	PBMSG_HEAD header; // C1:26
	uint8_t type;
	uint32_t curAtt1;
	uint32_t curAtt2;
};

struct PMSG_MANA_SEND
{
	PBMSG_HEAD header; // C1:27
	uint8_t type;
	uint32_t curAtt1;
	uint32_t curAtt2;
};

struct PMSG_ITEM_SPECIAL_TIME_SEND
{
	PBMSG_HEAD header; // C3:29
	uint8_t number;
	uint16_t time;
};

struct PMSG_MAP_ATTR_SEND
{
	PBMSG_HEAD header; // C1:46
	uint8_t type;
	uint8_t attr;
	uint8_t flag;
	uint8_t count;
};

struct PMSG_MAP_ATTR
{
	uint8_t x;
	uint8_t y;
	uint8_t tx;
	uint8_t ty;
};

struct PMSG_PARTY_ITEM_INFO_SEND
{
	PBMSG_HEAD header; // C1:47
	uint8_t index[2];
	DWORD ItemInfo;
	uint8_t level;
};

struct PMSG_EFFECT_INFO_SEND
{
	PBMSG_HEAD header; // C1:48
	uint8_t index[2];
	uint8_t effect;
};

struct PMSG_MONSTER_SKILL_SEND
{
	PBMSG_HEAD header; // C1:69
	uint8_t skill[2];
	uint16_t index;
	uint16_t target;
};

struct PMSG_EVENT_REMAIN_TIME_SEND
{
	PBMSG_HEAD header; // C1:91
	uint8_t EventType;
	uint8_t RemainTimeH;
	uint8_t EnteredUser;
	uint8_t RemainTimeL;
};

struct PMSG_TIME_COUNT_SEND
{
	PBMSG_HEAD header; // C1:92
	uint8_t type;
};

struct PMSG_REWARD_EXPERIENCE_SEND
{
	PBMSG_HEAD header; // C1:9C
	uint8_t index[2];
	uint16_t experience[2];
	uint8_t damage[2];
#if(GAMESERVER_EXTRA==1)
	DWORD ViewDamageHP;
#endif
};

struct PMSG_EVENT_ENTER_COUNT_SEND
{
	PBMSG_HEAD header; // C1:9F
	uint8_t EventType;
	uint8_t EnterCount;
};

struct PMSG_PET_ITEM_INFO_SEND
{
	PBMSG_HEAD header; // C1:A9
	uint8_t type;
	uint8_t flag;
	uint8_t slot;
	uint8_t level;
	UINT experience;
#if(GAMESERVER_UPDATE>=401)
	uint8_t durability;
#endif
};

struct PMSG_MAP_SERVER_MOVE_SEND
{
	PSBMSG_HEAD header; // C1:B1:00
	char IpAddress[16];
	uint16_t ServerPort;
	uint16_t ServerCode;
	DWORD AuthCode1;
	DWORD AuthCode2;
	DWORD AuthCode3;
	DWORD AuthCode4;
};

struct PMSG_MAP_SERVER_MOVE_AUTH_SEND
{
	PSBMSG_HEAD header; // C1:B1:01
	uint8_t result;
};

struct PMSG_TAX_INFO_SEND
{
	PSBMSG_HEAD header; // C1:B2:1A
	uint8_t type;
	uint8_t rate;
};

struct PMSG_KILL_COUNT_SEND
{
	PSBMSG_HEAD header; // C1:B8:01
	uint8_t count;
};

struct PMSG_CREATION_STATE_SEND
{
	PSBMSG_HEAD header; // C1:B9:01
	uint8_t index[2];
	uint8_t state;
};

struct PMSG_MONSTER_AREA_SKILL_SEND
{
	PSBMSG_HEAD header; // C1:BD:0C
	uint8_t MonsterClass[2];
	uint8_t sx;
	uint8_t sy;
	uint8_t tx;
	uint8_t ty;
};

struct PMSG_FRIEND_STATE_SEND
{
	PBMSG_HEAD header; // C1:C4
	char name[10];
	uint8_t state;
};

struct PMSG_FRIEND_MESSAGE_LIST_SEND
{
	PBMSG_HEAD header; // C1:C6
	uint16_t number;
	char name[10];
	char date[30];
	char subject[60];
	uint8_t read;
};

struct PMSG_MOVE_SEND
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE1]
	uint8_t index[2];
	uint8_t x;
	uint8_t y;
	uint8_t dir;
};

struct PMSG_ELEMENTAL_DAMAGE_SEND
{
	PBMSG_HEAD header; // C1:D8
	uint8_t index[2];
	uint8_t attribute;
	DWORD damage;
#if(GAMESERVER_EXTRA==1)
	DWORD ViewCurHP;
	DWORD ViewCurSD;
#endif
};

struct PMSG_CHARACTER_CREATION_ENABLE_SEND
{
	PBMSG_HEAD header; // C1:DE
	uint8_t flag;
	uint8_t result;
};

struct PMSG_LIFE_UPDATE_SEND
{
	PSBMSG_HEAD header; // C1:EC:10
	INDEX_DATA(Index);
	DWORD MaxHP;
	DWORD CurHP;
};

struct PMSG_CHARACTER_ATTACK_SPEED_SEND
{
	PSBMSG_HEAD header; // C1:EC:30
	DWORD PhysiSpeed;
	DWORD MagicSpeed;
};

struct PMSG_ENTER_EVENT_MAP_ERROR_SEND
{
	PSBMSG_HEAD header; // C1:EF:09
	DWORD result;
};

struct PMSG_CONNECT_CLIENT_SEND
{
	PSBMSG_HEAD header; // C1:F1:00
	uint8_t result;
	uint8_t index[2];
	uint8_t ClientVersion[5];
};

struct PMSG_CONNECT_ACCOUNT_SEND
{
	PSBMSG_HEAD header; // C1:F1:01
	uint8_t result;
};

struct PMSG_CLOSE_CLIENT_SEND
{
	PSBMSG_HEAD header; // C1:F1:02
	uint8_t result;
};

struct PMSG_CHARACTER_LIST_SEND
{
	PSWMSG_HEAD header; // C1:F3:00
	uint8_t ClassCode;
	uint8_t MoveCnt;
	uint8_t count;
#if(GAMESERVER_UPDATE>=602)
	uint8_t ExtWarehouse;
#endif
};

struct PMSG_CHARACTER_LIST
{
	uint8_t slot;
	char Name[10];
	uint16_t Level;
	uint8_t CtlCode;
	uint8_t Class;
	DWORD Equipment[EQUIPMENT_NEW_LENGTH];
	uint8_t GuildStatus;
#if(GAMESERVER_UPDATE>=602)
	DWORD Reset;
	DWORD MasterReset;
#endif
};

struct PMSG_CHARACTER_CREATE_SEND
{
	PSBMSG_HEAD header; // C1:F3:01
	uint8_t result;
	char name[10];
	uint8_t slot;
	uint16_t level;
	uint8_t Class;
	uint8_t equipment[24];
};

struct PMSG_CHARACTER_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:F3:02
	uint8_t result;
};

struct PMSG_CHARACTER_INFO_SEND
{
	PSBMSG_HEAD header; // C3:F3:03
	uint8_t X;
	uint8_t Y;
	uint8_t Map;
	uint8_t Dir;
	uint8_t Experience[8];
	uint8_t BaseExperience[8];
	uint8_t NextExperience[8];
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

	uint32_t Money;
	uint8_t PKLevel;
	uint8_t CtlCode;
	uint16_t FruitAddPoint;
	uint16_t MaxFruitAddPoint;
	uint16_t FruitSubPoint;
	uint16_t MaxFruitSubPoint;
#if(GAMESERVER_UPDATE>=602)
	uint8_t ExtInventory;
	uint8_t AccountLevel;
#endif
#if(GAMESERVER_EXTRA==1)
	uint32_t ViewReset;
	uint32_t MaxCharacterLevel;
#ifdef INVENTORY_EXT_BUY
	uint32_t PriceInventoryExt;
#endif
#endif
	DWORD PenaltyTime;
};

struct PMSG_CHARACTER_REGEN_SEND
{
	PSBMSG_HEAD header; // C3:F3:04
	uint8_t X;
	uint8_t Y;
	uint8_t Map;
	uint8_t Dir;
	DWORD Life;
	DWORD Mana;
	DWORD Shield;
	DWORD BP;
	uint8_t Experience[8];
	DWORD Money;
};

struct PMSG_LEVEL_UP_SEND
{
	PSBMSG_HEAD header; // C1:F3:05
	uint32_t Level;
	uint32_t LevelPoint;
	uint32_t LifeMax;
	uint32_t ManaMax;
	uint32_t ShieldMax;
	uint32_t SkillManaMax;
	uint32_t Experience;
	uint32_t BaseExperience;
	uint32_t NextExperience;

	uint16_t FruitAddPoint;
	uint16_t MaxFruitAddPoint;
	uint16_t FruitSubPoint;
	uint16_t MaxFruitSubPoint;

};

struct PMSG_LEVEL_UP_POINT_SEND
{
	PSBMSG_HEAD header; // C1:F3:06
	uint8_t result;

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
};

struct PMSG_MONSTER_DAMAGE_SEND
{
	PSBMSG_HEAD header; // C1:F3:07
	uint8_t damage[2];
	uint8_t ShieldDamage[2];
#if(GAMESERVER_EXTRA==1)
	DWORD ViewCurHP;
	DWORD ViewCurSD;
	DWORD ViewDamageHP;
	DWORD ViewDamageSD;
#endif
};

struct PMSG_PK_LEVEL_SEND
{
	PSBMSG_HEAD header; // C1:F3:08
	uint8_t index[2];
	uint8_t PKLevel;
	DWORD PenaltyTime;
};

struct PMSG_CHARACTER_NAME_CHECK_SEND
{
	PSBMSG_HEAD header; // C1:F3:15
	char name[10];
	uint8_t result;
};

struct PMSG_CHARACTER_NAME_CHANGE_SEND
{
	PSBMSG_HEAD header; // C1:F3:16
	char OldName[10];
	char NewName[10];
	uint8_t result;
};

struct PMSG_SUMMON_LIFE_SEND
{
	PSBMSG_HEAD header; // C1:F3:20
	uint8_t life;
};

struct PMSG_TIME_VIEW_SEND
{
	PSBMSG_HEAD header; // C1:F3:22
	uint16_t time;
};

struct PMSG_OPTION_DATA_SEND
{
#pragma pack(1)
	PSBMSG_HEAD header; // C1:F3:30
	uint8_t SkillKey[20];
	uint8_t GameOption;
	uint8_t QKey;
	uint8_t WKey;
	uint8_t EKey;
	uint8_t ChatWindow;
	uint8_t RKey;
	DWORD QWERLevel;
#pragma pack()
};

struct PMSG_MATH_AUTHENTICATOR_SEND
{
	PSBMSG_HEAD header; // C1:F3:32
	DWORD function;
	float value;
	char result[64];
};

struct PMSG_FIREWORKS_SEND
{
	PSBMSG_HEAD header; // C1:F3:40
	uint8_t type;
	uint8_t x;
	uint8_t y;
};

struct PMSG_SERVER_COMMAND_SEND
{
	PSBMSG_HEAD header; // C1:F3:40
	uint8_t type;
	uint8_t cmd1;
	uint8_t cmd2;
};

struct PMSG_ACHERON_ENTER_SEND
{
	PSBMSG_HEAD header; // C1:F8:21
	uint8_t result;
};

struct PMSG_SNS_DATA_SEND
{
	PWMSG_HEAD header; // C1:FB
	uint8_t result;
	uint8_t data[256];
};

struct PMSG_NEW_CHARACTER_INFO_SEND
{
	PSBMSG_HEAD header; // C1:F3:E0
	uint16_t Level;
	uint32_t Reset;
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
	uint32_t SkillMana;
	uint32_t SkillManaMax;
	uint32_t Shield;
	uint32_t ShieldMax;

	uint16_t FruitAddPoint;
	uint16_t MaxFruitAddPoint;
	uint16_t FruitSubPoint;
	uint16_t MaxFruitSubPoint;

	uint8_t Experience[8];
	uint8_t BaseExperience[8];
	uint8_t NextExperience[8];
};

struct PMSG_NEW_CHARACTER_CALC_SEND
{
	PSBMSG_HEAD header; // C1:F3:E1
	DWORD ViewCurHP;
	DWORD ViewMaxHP;
	DWORD ViewCurMP;
	DWORD ViewMaxMP;
	DWORD ViewCurBP;
	DWORD ViewMaxBP;
	DWORD ViewCurSD;
	DWORD ViewMaxSD;
	DWORD ViewAddStrength;
	DWORD ViewAddDexterity;
	DWORD ViewAddVitality;
	DWORD ViewAddEnergy;
	DWORD ViewAddLeadership;
	DWORD ViewPhysiDamageMin;
	DWORD ViewPhysiDamageMax;
	DWORD ViewMagicDamageMin;
	DWORD ViewMagicDamageMax;
	DWORD ViewCurseDamageMin;
	DWORD ViewCurseDamageMax;
	DWORD ViewMulPhysiDamage;
	DWORD ViewDivPhysiDamage;
	DWORD ViewMulMagicDamage;
	DWORD ViewDivMagicDamage;
	DWORD ViewMulCurseDamage;
	DWORD ViewDivCurseDamage;
	DWORD ViewMagicDamageRate;
	DWORD ViewCurseDamageRate;
	DWORD ViewPhysiSpeed;
	DWORD ViewMagicSpeed;
	DWORD ViewAttackSuccessRate;
	DWORD ViewAttackSuccessRatePvP;
	DWORD ViewDefense;
	DWORD ViewDefenseSuccessRate;
	DWORD ViewDefenseSuccessRatePvP;
	DWORD ViewDamageMultiplier;
	DWORD ViewRFDamageMultiplierA;
	DWORD ViewRFDamageMultiplierB;
	DWORD ViewRFDamageMultiplierC;
	DWORD ViewDarkSpiritAttackDamageMin;
	DWORD ViewDarkSpiritAttackDamageMax;
	DWORD ViewDarkSpiritAttackSpeed;
	DWORD ViewDarkSpiritAttackSuccessRate;
	DWORD PowerATTK;
};

struct PMSG_NEW_HEALTH_BAR_SEND
{
	PSWMSG_HEAD header; // C2:F3:E2
	uint8_t count;
};

struct PMSG_NEW_HEALTH_BAR
{
	uint16_t index;
	uint8_t type;
	uint8_t rate;
	uint8_t rate2;
};

struct PMSG_NEW_GENS_BATTLE_INFO_SEND
{
	PSBMSG_HEAD header; // C1:F3:E3
	uint8_t GensBattleMapCount;
	uint8_t GensMoveIndexCount;
	uint8_t GensBattleMap[120];
	uint8_t GensMoveIndex[120];
};

struct PMSG_NEW_MESSAGE_SEND
{
	PSBMSG_HEAD header; // C1:F3:E4
	char message[128];
};

struct PMSG_OFFTRADE_SEND
{
	PSBMSG_HEAD header;
	int Type;
};

struct PMSG_SHOPACTIVE_SEND
{
	PSBMSG_HEAD header;
	int Active;
	int Type;
};


struct PMSG_PING_SEND
{
	PSBMSG_HEAD header; // C1:F3:F1
};

//**********************************************//
//**********************************************//
//**********************************************//

extern void ProtocolCore(uint8_t head, uint8_t* lpMsg, int size, int aIndex, int encrypt, int serial);
extern void CGChatRecv(PMSG_CHAT_RECV* lpMsg, int aIndex);
extern void CGChatWhisperRecv(PMSG_CHAT_WHISPER_RECV* lpMsg, int aIndex);
extern void CGMainCheckRecv(PMSG_MAIN_CHECK_RECV* lpMsg, int aIndex);
extern void CGLiveClientRecv(PMSG_LIVE_CLIENT_RECV* lpMsg, int aIndex);
extern void CGPositionRecv(PMSG_POSITION_RECV* lpMsg, int aIndex);
extern void CGActionRecv(PMSG_ACTION_RECV* lpMsg, int aIndex);
extern void CGEventRemainTimeRecv(PMSG_EVENT_REMAIN_TIME_RECV* lpMsg, int aIndex);
extern void CGPetItemCommandRecv(PMSG_PET_ITEM_COMMAND_RECV* lpMsg, int aIndex);
extern void CGPetItemInfoRecv(PMSG_PET_ITEM_INFO_RECV* lpMsg, int aIndex);
extern void CGMapServerMoveAuthRecv(PMSG_MAP_SERVER_MOVE_AUTH_RECV* lpMsg, int aIndex);
extern void CGMoveRecv(PMSG_MOVE_RECV* lpMsg, int aIndex);
extern void CGConnectAccountRecv(PMSG_CONNECT_ACCOUNT_RECV* lpMsg, int aIndex);
extern void CGCloseClientRecv(PMSG_CLOSE_CLIENT_RECV* lpMsg, int aIndex);
extern void CGCharacterListRecv(int aIndex);
extern void CGCharacterCreateRecv(PMSG_CHARACTER_CREATE_RECV* lpMsg, int aIndex);
extern void CGCharacterDeleteRecv(PMSG_CHARACTER_DELETE_RECV* lpMsg, int aIndex);
extern void CGCharacterInfoRecv(PMSG_CHARACTER_INFO_RECV* lpMsg, int aIndex);
extern void CGLevelUpPointRecv(PMSG_LEVEL_UP_POINT_RECV* lpMsg, int aIndex);
extern void CGReloadInterfaceMain(PMSG_LEVEL_UP_POINT_RECV* lpMsg, int aIndex);
extern void CGPositionRecv(PMSG_POSITION_RECV* lpMsg, int aIndex);
extern void CGCharacterOffRecv(PMSG_CHARACTER_OFF_RECV* lpMsg, int aIndex);


extern void CGCharacterMoveViewportEnableRecv(int aIndex);
extern void CGCharacterNameCheckRecv(PMSG_CHARACTER_NAME_CHECK_RECV* lpMsg, int aIndex);
extern void CGCharacterNameChangeRecv(PMSG_CHARACTER_NAME_CHANGE_RECV* lpMsg, int aIndex);
extern void CGOptionChangeSkinRecv(PMSG_OPTION_CHANGE_SKIN_RECV* lpMsg, int aIndex);
extern void CGOptionDataRecv(PMSG_OPTION_DATA_RECV* lpMsg, int aIndex);
extern void CGClientSecurityBreachRecv(PMSG_CLIENT_SECURITY_BREACH_RECV* lpMsg, int aIndex);
extern void CGAcheronEnterRecv(int aIndex);
extern void CGSNSDataRecv(PMSG_SNS_DATA_RECV* lpMsg, int aIndex);
extern void CGSNSDataLogRecv(PMSG_SNS_DATA_LOG_RECV* lpMsg, int aIndex);
extern void GCChatTargetSend(LPOBJ lpObj, int aIndex, char* message);
extern void GCChatTargetNewSend(LPOBJ lpObj, int aIndex, char* message, ...);
extern void GCChatWhisperSend(int aIndex, char* name, char* message);
extern void GCMainCheckSend(int aIndex);
extern void GCEventStateSend(int aIndex, uint8_t state, uint8_t event);
extern void GCEventStateSendToAll(int map, uint8_t state, uint8_t event);
extern void GCServerMsgSend(int aIndex, uint8_t msg);
extern void GCWeatherSend(int aIndex, uint8_t weather);
extern void GCDamageSend(int aIndex, int bIndex, uint8_t flag, int damage, int type, int ShieldDamage);
extern void GCUserDieSend(LPOBJ lpObj, int aIndex, int skill, int bIndex);
extern void GCActionSend(LPOBJ lpObj, uint8_t action, int aIndex, int bIndex);
extern void GCMoneySend(int aIndex, DWORD money);
#ifdef TRADE_SYSTEM_COIN
extern void GCCoinSend(int aIndex, DWORD coin1, DWORD coin2, DWORD coin3);
#endif // TRADE_SYSTEM_COIN
extern void GCLifeSend(int aIndex, uint8_t type, uint32_t life, uint32_t shield);
extern void GCManaSend(int aIndex, uint8_t type, uint32_t mana, uint32_t bp);
extern void GCItemUseSpecialTimeSend(int aIndex, uint8_t number, int time);
extern void GCMapAttrSend(int aIndex, uint8_t type, uint8_t attr, uint8_t flag, uint8_t count, PMSG_MAP_ATTR* lpInfo);
extern void GCPartyItemInfoSend(int aIndex, CItem* lpItem);
extern void GCEffectInfoSend(int aIndex, uint8_t effect);
extern void GCGuildMasterQuestionSend(int aIndex);
extern void GCMonsterSkillSend(LPOBJ lpObj, LPOBJ lpTarget, int skill);
extern void GCMonsterDieSend(int aIndex, int bIndex, int experience, int damage, uint8_t flag);
extern void GCRewardExperienceSend(int aIndex, int experience);
extern void GCEventEnterCountSend(int aIndex, uint8_t EventType, uint8_t EnterCount);
extern void GCPetItemInfoSend(int aIndex, uint8_t type, uint8_t flag, uint8_t slot, uint8_t level, DWORD experience, uint8_t durability);
extern void GCMapServerMoveAuthSend(int aIndex, uint8_t result);
extern void GCTaxInfoSend(int aIndex, uint8_t type, uint8_t rate);
extern void GCCreationStateSend(int aIndex, uint8_t state);
extern void GCMonsterAreaSkillSend(int aIndex, int MonsterClass, int sx, int sy, int tx, int ty);
extern void GCElementalDamageSend(int aIndex, int bIndex, uint8_t attribute, int damage);
extern void GCCharacterCreationEnableSend(int aIndex, uint8_t flag, uint8_t result);
extern void GCLifeUpdateSend(LPOBJ lpObj);
extern void GCCharacterAttackSpeedSend(LPOBJ lpObj);
extern void GCConnectClientSend(int aIndex, uint8_t result);
extern void GCConnectAccountSend(int aIndex, uint8_t result);
extern void GCConnectAccountSend(int aIndex, uint8_t result, SOCKET socket);
extern void GCCloseClientSend(int aIndex, uint8_t result);
extern void GCCharacterRegenSend(LPOBJ lpObj);
extern void GCLevelUpSend(LPOBJ lpObj);
extern void GCMonsterDamageSend(int aIndex, int damage, int ShieldDamage);
extern void GCPKLevelSend(int aIndex, int PKLevel, DWORD PenaltyTime);
extern void GCSummonLifeSend(int aIndex, int life, int MaxLife);
extern void GCTimeViewSend(int aIndex, int time);
extern void GCMathAuthenticatorSend(int aIndex);
extern void GCFireworksSend(LPOBJ lpObj, int x, int y);
extern void GCServerCommandSend(int aIndex, uint8_t type, uint8_t cmd1, uint8_t cmd2);
extern void GCNewCharacterInfoSend(LPOBJ lpObj);
extern void GCNewCharacterCalcSend(LPOBJ lpObj);
extern void GCNewHealthBarSend(LPOBJ lpObj);
extern void GCNewGensBattleInfoSend(LPOBJ lpObj);
extern void GCNewMessageSend(LPOBJ lpObj, char* message);
extern void GCPingRecv(int aIndex);
extern void CommandSelect(LPOBJ lpObj, char* message, int Npc);

//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

struct PMSG_REQ_CASTLESIEGESTATE
{
	PSBMSG_HEAD h;
};

struct PMSG_REQ_REGCASTLESIEGE
{
	PSBMSG_HEAD h;
};

struct PMSG_REQ_GIVEUPCASTLESIEGE
{
	PSBMSG_HEAD h;
	uint8_t btGiveUp;
};

struct PMSG_REQ_GUILDREGINFO
{
	PSBMSG_HEAD h;
};

struct CSP_ANS_GUILDREGINFO
{
	PSBMSG_HEAD h;
	int iResult;	// 4
	uint16_t wMapSvrNum;	// 8
	int iIndex;	// C
	char szGuildName[8];	// 10
	int iRegMarkCount;	// 18
	uint8_t bIsGiveUp;	// 1C
	uint8_t btRegRank;	// 1D
};

struct PMSG_REQ_REGGUILDMARK
{
	PSBMSG_HEAD h;
	uint8_t btItemPos;
};

struct CSP_ANS_GUILDREGMARK
{
	struct PSBMSG_HEAD h;
	int iResult;
	uint16_t wMapSvrNum;
	int iIndex;
	char szGuildName[8];
	int iItemPos;
	int iRegMarkCount;
};

struct PMSG_REQ_NPCBUY
{
	PSBMSG_HEAD h;
	int iNpcNumber;
	int iNpcIndex;
};

struct PMSG_REQ_NPCREPAIR
{
	PSBMSG_HEAD h;
	int iNpcNumber;
	int iNpcIndex;
};

struct PMSG_REQ_NPCUPGRADE
{
	PSBMSG_HEAD h;
	int iNpcNumber;
	int iNpcIndex;
	int iNpcUpType;
	int iNpcUpValue;
};

struct PMSG_REQ_TAXMONEYINFO
{
	PSBMSG_HEAD h;
};

struct PMSG_REQ_TAXRATECHANGE
{
	PSBMSG_HEAD h;
	uint8_t btTaxType;
	uint8_t btTaxRate1;
	uint8_t btTaxRate2;
	uint8_t btTaxRate3;
	uint8_t btTaxRate4;
};

struct PMSG_REQ_MONEYDRAWOUT
{
	PSBMSG_HEAD h;
	uint8_t btMoney1;
	uint8_t btMoney2;
	uint8_t btMoney3;
	uint8_t btMoney4;
};

struct PMSG_REQ_CSGATEOPERATE
{
	PSBMSG_HEAD h;
	uint8_t btOperate;
	uint8_t btIndex1;
	uint8_t btIndex2;
};

struct PMSG_REQ_MINIMAPDATA
{
	PSBMSG_HEAD h;
};

struct PMSG_REQ_STOPMINIMAPDATA
{
	PSBMSG_HEAD h;
};

struct PMSG_REQ_CSCOMMAND
{
	PSBMSG_HEAD h;
	uint8_t btTeam;
	uint8_t btX;
	uint8_t btY;
	uint8_t btCommand;
};

struct PMSG_REQ_CSHUNTZONEENTER
{
	PSBMSG_HEAD h;
	uint8_t btHuntZoneEnter;
};

struct PMSG_REQ_NPCDBLIST
{
	PBMSG_HEAD h;
	uint8_t btMonsterCode;
};

struct PMSG_REQ_CSREGGUILDLIST
{
	PBMSG_HEAD h;
};

struct PMSG_REQ_CSATTKGUILDLIST
{
	PBMSG_HEAD h;
};

struct PMSG_REQ_GUILDMARK_OF_CASTLEOWNER
{
	PSBMSG_HEAD h;
};

struct PMSG_ANS_CASTLESIEGESTATE
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	char cCastleSiegeState;
	uint8_t btStartYearH;
	uint8_t btStartYearL;
	uint8_t btStartMonth;
	uint8_t btStartDay;
	uint8_t btStartHour;
	uint8_t btStartMinute;
	uint8_t btEndYearH;
	uint8_t btEndYearL;
	uint8_t btEndMonth;
	uint8_t btEndDay;
	uint8_t btEndHour;
	uint8_t btEndMinute;
	uint8_t btSiegeStartYearH;
	uint8_t btSiegeStartYearL;
	uint8_t btSiegeStartMonth;
	uint8_t btSiegeStartDay;
	uint8_t btSiegeStartHour;
	uint8_t btSiegeStartMinute;
	char cOwnerGuild[8];
	char cOwnerGuildMaster[10];
	uint8_t btStateLeftSec1;
	uint8_t btStateLeftSec2;
	uint8_t btStateLeftSec3;
	uint8_t btStateLeftSec4;
};

struct PMSG_ANS_REGCASTLESIEGE
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	char szGuildName[8];
};

struct PMSG_ANS_GIVEUPCASTLESIEGE
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	uint8_t btIsGiveUp;
	char szGuildName[8];
};

struct PMSG_ANS_GUILDREGINFO
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	char szGuildName[8];
	uint8_t btGuildMark1;
	uint8_t btGuildMark2;
	uint8_t btGuildMark3;
	uint8_t btGuildMark4;
	uint8_t btIsGiveUp;
	uint8_t btRegRank;
};

struct PMSG_ANS_REGGUILDMARK
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	char szGuildName[8];
	uint8_t btGuildMark1;
	uint8_t btGuildMark2;
	uint8_t btGuildMark3;
	uint8_t btGuildMark4;
};

struct PMSG_ANS_NPCBUY
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	int iNpcNumber;
	int iNpcIndex;
};

struct PMSG_ANS_NPCREPAIR
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	int iNpcNumber;
	int iNpcIndex;
	int iNpcHP;
	int iNpcMaxHP;
};

struct PMSG_ANS_NPCUPGRADE
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	int iNpcNumber;
	int iNpcIndex;
	int iNpcUpType;
	int iNpcUpValue;
};

struct PMSG_ANS_TAXMONEYINFO
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	uint8_t btTaxRateChaos;
	uint8_t btTaxRateStore;
	uint8_t btMoney1;
	uint8_t btMoney2;
	uint8_t btMoney3;
	uint8_t btMoney4;
	uint8_t btMoney5;
	uint8_t btMoney6;
	uint8_t btMoney7;
	uint8_t btMoney8;
};

struct PMSG_ANS_TAXRATECHANGE
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	uint8_t btTaxType;
	uint8_t btTaxRate1;
	uint8_t btTaxRate2;
	uint8_t btTaxRate3;
	uint8_t btTaxRate4;
};

struct PMSG_ANS_MONEYDRAWOUT
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	uint8_t btMoney1;
	uint8_t btMoney2;
	uint8_t btMoney3;
	uint8_t btMoney4;
	uint8_t btMoney5;
	uint8_t btMoney6;
	uint8_t btMoney7;
	uint8_t btMoney8;
};

struct PMSG_ANS_CSGATESTATE
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	uint8_t btIndex1;
	uint8_t btIndex2;
};

struct PMSG_ANS_CSGATEOPERATE
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	uint8_t btOperate;
	uint8_t btIndex1;
	uint8_t btIndex2;
};

struct PMSG_ANS_CSGATECURSTATE
{
	PSBMSG_HEAD h;
	uint8_t btOperate;
	uint8_t btIndex1;
	uint8_t btIndex2;
};

struct PMSG_ANS_NOTIFYSWITCHPROC
{
	PSBMSG_HEAD h;
	uint8_t btIndex1;
	uint8_t btIndex2;
	uint8_t btUserIndex1;
	uint8_t btUserIndex2;
	uint8_t btSwitchState;
};

struct PMSG_ANS_NOTIFYCROWNPROC
{
	PSBMSG_HEAD h;
	uint8_t btCrownState;
	DWORD dwAccumulatedCrownAccessTime;
};

struct PMSG_ANS_CSCOMMAND
{
	PSBMSG_HEAD h;
	uint8_t btTeam;
	uint8_t btX;
	uint8_t btY;
	uint8_t btCommand;
};

struct PMSG_ANS_NPCDBLIST
{
	PWMSG_HEAD h;
	uint8_t btResult;
	int iCount;
};

struct PMSG_ANS_CSATTKGUILDLIST
{
	PWMSG_HEAD h;
	uint8_t btResult;
	int iCount;
};

struct PMSG_INFO_STATUE_AND_GATE
{
	PSBMSG_HEAD h;
	int iNpcDefense1[4];
	int iNpcDefense2[4];
	int iNpcUpMaxHP1[4];
	int iNpcUpMaxHP2[4];
	int iNpcUpRecLevel[4];
};

struct PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE
{
	PSBMSG_HEAD h; // +0x0(0x4)
	uint8_t btResult; // +0x4(0x1)
};

struct PMSG_ANS_GUILDMARK_OF_CASTLEOWNER
{
	PSBMSG_HEAD h;	// C1:B9:02
	uint8_t GuildMarkOfCastleOwner[32];	// 4
};

struct PMSG_ANS_CSHUNTZONEENTER
{
	PSBMSG_HEAD h;	// C1:B2:1F
	uint8_t btResult;	// 4
	uint8_t btHuntZoneEnter;	// 5
};

struct PMSG_ANS_NOTIFYCSPROGRESS
{
	PSBMSG_HEAD h;	// C1:B2:18
	uint8_t btCastleSiegeState;	// 4
	uint8_t szGuildName[8];	// 5
};

struct PMSG_ANS_MINIMAPDATA
{
	PSBMSG_HEAD h;
	uint8_t btResult;	// 4
};

struct PMSG_ANS_CSLEFTTIMEALARM
{
	PSBMSG_HEAD h;	// C1:B2:1E
	uint8_t btHour;	// 4
	uint8_t btMinute;	// 5
};

struct PMSG_ANS_MAPSVRTAXINFO
{
	PSBMSG_HEAD h;	// C1:B2:1A
	uint8_t btTaxType;	// 4
	uint8_t btTaxRate;	// 5
};

struct CSP_ANS_CSINITDATA
{
	PWMSG_HEAD h;	// 0
	int iResult;		// 4
	uint16_t wMapSvrNum;	// 8
	uint16_t wStartYear;	// a
	uint8_t btStartMonth;	// c
	uint8_t btStartDay;	// d
	uint16_t wEndYear; // e
	uint8_t btEndMonth; // 10
	uint8_t btEndDay;	// 11
	uint8_t btIsSiegeGuildList;	// 12
	uint8_t btIsSiegeEnded;	// 13
	uint8_t btIsCastleOccupied;	// 14
	char szCastleOwnGuild[8]; // 15
	__int64 i64CastleMoney; // 20
	int iTaxRateChaos; // 28
	int iTaxRateStore; // 2c
	int iTaxHuntZone; // 30
	int iFirstCreate; // 34
	int iCount; // 38
};

struct CSP_CSINITDATA
{
	int iNpcNumber; // 0
	int iNpcIndex; // 4
	int iNpcDfLevel; // 8
	int iNpcRgLevel; // c
	int iNpcMaxHp; // 10
	int iNpcHp; // 14
	uint8_t btNpcX; // 18
	uint8_t btNpcY; // 19
	uint8_t btNpcDIR; // 1a
};

struct CSP_CALCREGGUILDLIST
{
	char szGuildName[8]; // 0
	int iRegMarkCount; // 8
	int iGuildMemberCount; // c
	int iGuildMasterLevel; // 10
	int iSeqNum; // 14
};

struct CSP_CSGUILDUNIONINFO
{
	char szGuildName[8]; // 0
	int iCsGuildID; // 8
};

struct CSP_CSLOADTOTALGUILDINFO
{
	char szGuildName[8]; // 0
	int iCsGuildID; // 8
	int iGuildInvolved; // c
	int iGuildScore; //10 season 2.5 add-on
};

struct PMSG_CSATTKGUILDLIST
{
	uint8_t btCsJoinSide; // 0
	uint8_t btGuildInvolved; // 1
	char szGuildName[8]; // 2
	int iGuildScore; //A season 2.5 add-on
};

struct CSP_ANS_NPCDATA
{
	PWMSG_HEAD h; // 0
	int iResult; // 4
	uint16_t wMapSvrNum; // 8
	int iIndex; // c
	int iCount; // 10
};

struct PMSG_ANS_NOTIFYCSSTART
{
	PSBMSG_HEAD h; // 0
	uint8_t btStartState; // 4
};

struct PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE
{
	PSBMSG_HEAD h;
	int iPrice;
};

struct PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE
{
	PSBMSG_HEAD h;
	uint8_t btResult;
	uint8_t btUsable;
	int iCurrentPrice;
	int iMaxPrice;
	int iUnitOfPrice;
};

struct PMSG_UNION_VIEWPORT_NOTIFY_COUNT
{
	struct PWMSG_HEAD h;
	uint8_t btCount;
};

struct PMSG_UNION_VIEWPORT_NOTIFY
{
	uint8_t btNumberH;
	uint8_t btNumberL;
	int iGuildNumber;
	uint8_t btGuildRelationShip;
	char szUnionName[8];
};

struct PMSG_DATA_PACKAGE
{
	PBMSG_HEAD header; //6
	int key;	// 4
};



void CGReqCastleSiegeState(PMSG_REQ_CASTLESIEGESTATE* lpMsg, int iIndex);
void GCAnsCastleSiegeState(int iIndex, int iResult, char* lpszGuildName, char* lpszGuildMaster);
void CGReqRegCastleSiege(PMSG_REQ_REGCASTLESIEGE* lpMsg, int iIndex);
void GCAnsRegCastleSiege(int iIndex, int iResult, char* lpszGuildName);
void CGReqGiveUpCastleSiege(PMSG_REQ_GIVEUPCASTLESIEGE* lpMsg, int iIndex);
void GCAnsGiveUpCastleSiege(int iIndex, int iResult, int bGiveUp, int iMarkCount, char* lpszGuildName);
void CGReqGuildRegInfo(PMSG_REQ_GUILDREGINFO* lpMsg, int iIndex);
void GCAnsGuildRegInfo(int iIndex, int iResult, CSP_ANS_GUILDREGINFO* lpMsgResult);
void CGReqRegGuildMark(PMSG_REQ_REGGUILDMARK* lpMsg, int iIndex);
void GCAnsRegGuildMark(int iIndex, int iResult, CSP_ANS_GUILDREGMARK* lpMsgResult);
void CGReqNpcBuy(PMSG_REQ_NPCBUY* lpMsg, int iIndex);
void GCAnsNpcBuy(int iIndex, int iResult, int iNpcNumber, int iNpcIndex);
void CGReqNpcRepair(PMSG_REQ_NPCREPAIR* lpMsg, int iIndex);
void GCAnsNpcRepair(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcHP, int iNpcMaxHP);
void CGReqNpcUpgrade(PMSG_REQ_NPCUPGRADE* lpMsg, int iIndex);
void GCAnsNpcUpgrade(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue);
void GCAnsNpcStatueAndGate(int iIndex);
void CGReqTaxMoneyInfo(PMSG_REQ_TAXMONEYINFO* lpMsg, int iIndex);
void GCAnsTaxMoneyInfo(int iIndex, int iResult, uint8_t btTaxRateChaos, uint8_t btTaxRateStore, __int64 i64Money);
void CGReqTaxRateChange(PMSG_REQ_TAXRATECHANGE* lpMsg, int iIndex);
void GCAnsTaxRateChange(int iIndex, int iResult, uint8_t btTaxType, int iTaxRate);
void CGReqMoneyDrawOut(PMSG_REQ_MONEYDRAWOUT* lpMsg, int iIndex);
void GCAnsMoneyDrawOut(int iIndex, int iResult, __int64 i64Money);
void GCAnsCsGateState(int iIndex, int iResult, int iGateIndex);
void CGReqCsGateOperate(PMSG_REQ_CSGATEOPERATE* lpMsg, int iIndex);
void GCAnsCsGateOperate(int iIndex, int iResult, int iGateIndex, int iGateOperate);
void GCAnsCsGateCurState(int iIndex, int iGateIndex, int iGateOperate);
void GCAnsCsAccessSwitchState(int iIndex, int iSwitchIndex, int iSwitchUserIndex, uint8_t btSwitchState);
void GCAnsCsAccessCrownState(int iIndex, uint8_t btCrownState);
void GCAnsCsNotifyStart(int iIndex, uint8_t btStartState);
void GCAnsCsNotifyProgress(int iIndex, uint8_t btCastleSiegeState, char* lpszGuildName);
void GCAnsCsMapSvrTaxInfo(int iIndex, uint8_t btTaxType, uint8_t btTaxRate);
void CGReqCsMiniMapData(PMSG_REQ_MINIMAPDATA* lpMsg, int iIndex);
void GCAnsCsMiniMapData(int iIndex, uint8_t btResult);
void CGReqStopCsMiniMapData(PMSG_REQ_STOPMINIMAPDATA* lpMsg, int iIndex);
void CGReqCsSendCommand(PMSG_REQ_CSCOMMAND* lpMsg, int iIndex);
void GCAnsCsSendCommand(int iCsJoinSide, uint8_t btTeam, uint8_t btX, uint8_t btY, uint8_t btCommand);
void GCAnsCsLeftTimeAlarm(uint8_t btHour, uint8_t btMinute);
void GCAnsSelfCsLeftTimeAlarm(int iIndex, uint8_t btHour, uint8_t btMinute);
void CGReqCsSetEnterHuntZone(PMSG_REQ_CSHUNTZONEENTER* lpMsg, int iIndex);
void GCAnsCsSetEnterHuntZone(int iIndex, uint8_t btResult, uint8_t btEnterHuntZone);
void CGReqNpcDbList(PMSG_REQ_NPCDBLIST* lpMsg, int iIndex);
void CGReqCsRegGuildList(PMSG_REQ_CSREGGUILDLIST* lpMsg, int iIndex);
void CGReqCsAttkGuildList(PMSG_REQ_CSATTKGUILDLIST* lpMsg, int iIndex);
void CGReqGuildMarkOfCastleOwner(PMSG_REQ_GUILDMARK_OF_CASTLEOWNER* aRecv, int iIndex);
void CGReqCastleHuntZoneEntrance(PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE* aRecv, int iIndex);

void CGReqtimeState(PMSG_DATA_PACKAGE* lpMsg, int iIndex);
