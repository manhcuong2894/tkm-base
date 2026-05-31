#pragma once
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"

namespace SEASON3B
{
	class CNewGuildMatching : public CNewUIObj
	{
	private:
		CNewUIManager* m_pNewUIMng;
		POINT			m_Pos;
	public:
		CNewGuildMatching();
		virtual~CNewGuildMatching();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		void SetPos(int x, int y);

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		float GetLayerDepth();	//. 2.4f
		void EventOrderWindows(double WindowsX, double WindowsY);

		void OpenningProcess();
		void ClosingProcess();
	};
}
