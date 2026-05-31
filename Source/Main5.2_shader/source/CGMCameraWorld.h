#pragma once

class CGMCameraControll
{
public:
	CGMCameraControll();
	virtual~CGMCameraControll();
	void InitCamera();
	void SwitchAction();
	void SyncCameraTargets();
	void UpdateSmoothCamera();
	void UpdateSmoothCameraWithMouse();
	void setZoom(float fStepZoom);
	void setAngleX(int32_t pitchDirection, float fStepAngle);
	void setAngleY(int32_t pitchDirection, float fStepAngle, float fStepZ);
	float GetFrustrumCollision();
	bool is_active();

	static CGMCameraControll* Instance() {
		static CGMCameraControll s_Instance;
		return &s_Instance;
	}
private:
	uint8_t InitCam;
	uint8_t bEnable;
	int32_t m_vMouseX;
	int32_t m_vMouseY;
	float m_vCameraFOV;
	float m_vTargetAngleX;
	float m_vTargetAngleY;
	float m_vTargetDistance;
	float m_vFrustrumCollision;
};

#define gCameraController			(CGMCameraControll::Instance())