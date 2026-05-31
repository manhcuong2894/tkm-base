// ---
#include "Protocol.h"
// ---
#define MAX_RANK  10
// ---
struct CUSTOM_RANKING
{
	int index;
	char Name[20];
	char Col1[20];
	char Col2[20];
};

struct CUSTOM_RANKING_DATA
{
	char szName[20];
	BYTE Class;
	BYTE Vip;
	int Score;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//
struct SDHP_CUSTOM_RANKING_SEND
{
	PSBMSG_HEAD header;
	WORD index;
	WORD type;
};

struct SDHP_RANKING_CHARACTER_INFO_SEND
{
	PSBMSG_HEAD header; // C1:04
	int characterIndex;
	WORD index;
	char name[11];
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//
struct SDHP_CUSTOM_RANKING_RECV
{
	PWMSG_HEAD header;
	int index;
	int type;
	int count;
};

struct SDHP_RANKING_CHARACTER_INFO_RECV
{
	PWMSG_HEAD header; // C2:0xF6
	int characterIndex;
	WORD index;
	char name[11];
	BYTE DBClass;
	BYTE Inventory[13][5];
};

//**********************************************//
//********** GameServer -> Cliente    **********//
//**********************************************//

struct PMSG_CUSTOM_RANKING_SEND
{
	PSWMSG_HEAD header;
	int RankIndex;
	char rankname[20];
	char col1[20];
	char col2[20];
	int count;
};

struct PMSG_CUSTOM_RANKING_COUNT_SEND
{
	PSBMSG_HEAD header; // C1:BF:51
	int count;
};

struct PMSG_RANKING_CHARACTER_INFO_SEND
{
	PSBMSG_HEAD header; // C1:BF:51
	int characterIndex;
	DWORD Equipment[EQUIPMENT_NEW_LENGTH];
};

//**********************************************//
//********** Cliente -> GameServer    **********//
//**********************************************//

struct PMSG_CUSTOM_RANKING_COUNT_RECV
{
	PSBMSG_HEAD header; // C1:BF:51
};

struct PMSG_CUSTOM_RANKING_RECV
{
	PSBMSG_HEAD header; // C1:BF:51
	BYTE type;
};

struct PMSG_RANKING_CHARACTER_INFO_RECV
{
	PSBMSG_HEAD header; // C1:F3:03
	int characterIndex;
	char name[10];
};

class CCustomRanking
{
public:
	void Load(char* path);
	void GCReqRankingPlayer(int Index, PMSG_RANKING_CHARACTER_INFO_RECV* lpMsg);
	void GDCustomRankingPlayerRecv(SDHP_RANKING_CHARACTER_INFO_RECV* lpMsg);
	void GCReqRanking(int Index, PMSG_CUSTOM_RANKING_RECV* pMsg);
	void GCReqRankingCount(int Index, PMSG_CUSTOM_RANKING_COUNT_RECV* lpMsg);
	void GDCustomRankingRecv(BYTE* ReceiveBuffer);

private:
	int TotalSize;
	CUSTOM_RANKING r_Data[MAX_RANK];
};
extern CCustomRanking gCustomRanking;
// ---
