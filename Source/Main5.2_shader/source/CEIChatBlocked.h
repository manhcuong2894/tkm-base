#pragma once
#include "CUIListBox.h"
#include "NewUICursedTempleSystem.h"

class CUITextInputBox;


namespace SEASON3B
{
	class CEIChatBlocked
	{
		enum IMAGE_LIST
		{
			IMAGE_MARK_POIN = CNewUICursedTempleSystem::IMAGE_CURSEDTEMPLESYSTEM_MINIMAPICON_HOLYITEM_PC,
		};
	public:
		CEIChatBlocked();
		virtual~CEIChatBlocked();

		void Create(int x, int y);
		void Release();
		void LoadImages();
		void UnLoadImages();
		void SetPos(int x, int y);

		void Render();
		bool Update(CNewUIObj* pObj);
		bool UpdateMouseEvent();
		void OpenInstance();
		void SaveInstance();
		void EnterInstance();
		bool CheckInstance(const unicode::t_char* pzText);
		void SetText(const unicode::t_char* pzText);
		bool HaveFocus();
	private:
		POINT m_Pos;
		CUIListBox* targetList;
		CNewUIButton* m_BtnRemove;
		CUITextInputBox* m_blockedInputBox;
	};

}
