#include "stdafx.h"
#include "NewUISystem.h"
#include "ZzzInterface.h"
#include "CGMCameraWorld.h"
#include "WINHANDLE.h"
#include "CDirection.h"
#include "Utilities/Log/muConsoleDebug.h"

extern float vCameraFOV;
extern float vCameraAngleY;
extern float vTargetDistance;

CGMCameraControll::CGMCameraControll()
{
	this->InitCam = 1;
	this->bEnable = 0;
	this->m_vMouseX = -1;
	this->m_vMouseY = -1;
	this->m_vCameraFOV = 35.f;
	this->m_vTargetAngleY = -48.5f;
	this->m_vTargetAngleX = -45.0f;
	this->m_vTargetDistance = 150.f;
	this->m_vFrustrumCollision = 0.0f;
}

CGMCameraControll::~CGMCameraControll()
{
}

void CGMCameraControll::InitCamera()
{
	this->InitCam = 1;
}

void CGMCameraControll::SwitchAction()
{
	this->bEnable ^= 1;
	MouseWheel = 0;
}

void CGMCameraControll::SyncCameraTargets()
{
	if (this->InitCam != 0)
	{
		this->InitCam = 0;
		this->m_vMouseX = -1;
		this->m_vMouseY = -1;
		this->m_vCameraFOV = 35.f;
		this->m_vTargetAngleY = -48.5f;
		this->m_vTargetAngleX = -45.0f;
		this->m_vTargetDistance = 150.f;
		this->m_vFrustrumCollision = 0.0f;
	}
}

void CGMCameraControll::UpdateSmoothCamera()
{
	if (!g_pCatapultWindow->CheckCamera() && !(g_Direction.IsDirection() && !g_Direction.m_bDownHero))
	{
		this->SyncCameraTargets();

		if (bEnable != 0)
		{
			if (!g_pNewUISystem->CheckMouseUse() && MouseOnWindow == 0 && g_dwMouseUseUIID == 0)
			{
				this->UpdateSmoothCameraWithMouse();
			}
		}

		if (this->m_vTargetAngleX >= 360.0f)
		{
			this->m_vTargetAngleX -= 360.0f;
			CameraAngle[2] -= 360.0f;
		}
		else if (this->m_vTargetAngleX <= -360.0f)
		{
			this->m_vTargetAngleX += 360.0f;
			CameraAngle[2] += 360.0f;
		}

		LInterpolationF(vCameraFOV, vCameraFOV, this->m_vCameraFOV, 0.05f);
		LInterpolationF(vCameraAngleY, vCameraAngleY, this->m_vTargetAngleY, 0.05f);
		LInterpolationF(CameraAngle[2], CameraAngle[2], this->m_vTargetAngleX, 0.05f);
		LInterpolationF(vTargetDistance, vTargetDistance, this->m_vTargetDistance, 0.05f);

		if (CameraFOV > 35.f)
		{
			float factor = (CameraFOV - 35.f) / 35.f;
			factor = std::clamp(factor, 0.f, 1.f);
			m_vFrustrumCollision = -250.f * factor;
		}
		else
		{
			m_vFrustrumCollision = 0.f;
		}
	}

	this->m_vMouseX = (int32_t)MouseX;
	this->m_vMouseY = (int32_t)MouseY;
}

void CGMCameraControll::UpdateSmoothCameraWithMouse()
{
	if (this->m_vMouseX == -1)
		this->m_vMouseX = (int32_t)MouseX;

	if (this->m_vMouseY == -1)
		this->m_vMouseY = (int32_t)MouseY;

	int32_t yaw = (this->m_vMouseX - (int32_t)MouseX);
	int32_t pitch = ((int32_t)MouseY - this->m_vMouseY);

	if (MouseMButton)
	{
		float mid_center = pos_center(320.f) - 50.f;

		if (yaw != 0 && !CheckMouseIn(mid_center, 0, 100, GetWindowsY))
		{
			this->setAngleX(1, 1.0f * yaw);
		}

		if (pitch != 0 && CheckMouseIn(mid_center, 0, 100, GetWindowsY))
		{
			this->setAngleY(1, 1.0f * pitch, 18.0f * pitch);
		}
	}

	if (MouseWheel != 0)
	{
		this->setZoom((MouseWheel * 3.5f));
		MouseWheel = 0;
	}
}

void CGMCameraControll::setZoom(float fStepZoom)
{
	float fFOV = this->m_vCameraFOV - fStepZoom;

	if (fFOV >= 17.5f && fFOV <= 70.0f)
	{
		this->m_vCameraFOV = fFOV;
	}
}

void CGMCameraControll::setAngleX(int32_t pitchDirection, float fStepAngle)
{
	this->m_vTargetAngleX += pitchDirection * fStepAngle;
}

void CGMCameraControll::setAngleY(int32_t pitchDirection, float fStepAngle, float fStepZ)
{
	float fAngleY = this->m_vTargetAngleY + (pitchDirection * fStepAngle);

	if (fAngleY >= -93.5f && fAngleY <= -45.5f)
	{
		this->m_vTargetAngleY = fAngleY;
		this->m_vTargetDistance -= (pitchDirection * fStepZ);
	}

	if (this->m_vTargetDistance < -15.0f)
		this->m_vTargetDistance = -15.0f;
	if (this->m_vTargetDistance > 930.0f)
		this->m_vTargetDistance = 930.0f;
}

float CGMCameraControll::GetFrustrumCollision()
{
	if (SceneFlag == MAIN_SCENE)
		return m_vFrustrumCollision;
	else
		return 0.0f;
}

bool CGMCameraControll::is_active()
{
	return this->bEnable;
}
