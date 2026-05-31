#pragma once

typedef struct
{
	int Index;
	int Level;
	int VipLevel;
	int Days;
	std::string textName;
} CUSTOM_ITEM_TICKET_VIP;

class CCustomItemTicketVip
{
public:
	CCustomItemTicketVip();
	virtual~CCustomItemTicketVip();
	static CCustomItemTicketVip* Instance();

	bool LoadFiles(const std::string& filename);
	bool LoadFileTxt(const std::string& filename);
	bool LoadFileXml(const std::string& filename, bool& is_open);

	CUSTOM_ITEM_TICKET_VIP* GetInfo(int index, int level);
	bool ChangeTicket(LPOBJ lpObj, int ItemIndex, int level);
private:
	std::vector<CUSTOM_ITEM_TICKET_VIP> ticketList;
};

#define gCustomTicket				(CCustomItemTicketVip::Instance())