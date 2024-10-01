#pragma once

#include "Camera.h"
#include "Base.h"
#include "Define.h"

enum class CAMERASTATE
{
	DEBUG = 0,
	PLAYER,
	EVENT
};
BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CPlayer;

class CDynamicCamera : public CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera();

public:
	HRESULT   Ready_GameObject(const _vec3* pEye
		, const _vec3* pAt
		, const _vec3* pUp
		, const _float& _fFov
		, const _float& _fAspect
		, const _float& _fNear
		, const _float& _fFar);

	virtual	  void		LateReady_GameObject();
	virtual   _int		Update_GameObject(const _float& fTimeDelta);
	virtual   void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void  ZoomTo(_float fRatio, _float fDuration);
	void  ShakeMove(_float fDuration);

private:
	void  Key_Input(const _float& fTimeDelta);
	void  Mouse_Move(const _float& fTimeDelta);
	void  MoveToPlayer(const _float& fTimeDelta);

	void  ZoomToTrigger(const _float& fTimeDelta);
	void  ResetZoom(const _float& fTimeDelta);

	void  ShakeMoveTrigger(const _float& fTimeDelta);
	void  GetPlayerInfo();

private:

	_matrix m_matCameraWorld;
	// 0923 동영 : 카메라 모드에 따른 화면 출력을 위한 변수
	CAMERASTATE m_eCameraState;

	CPlayer* m_Player;
	CTransform* m_playerTransform;
	_vec3 m_vPlayerPos; // 플레이어의 좌표

	_vec3 m_vIntervalPos; // 플레이어 좌표에서 이만큼 떨어진 곳에 카메라를 배치시킵니다.
	_vec3 m_vOriginInterval; // 평소의 IntervalPos 값
	float m_fMoveToPlayerSpeed;

	// Zoom
	// 외부에서 ZoomTo()함수에 의해 true가 돼, 업데이트에서 줌인/아웃이 진행
	bool m_bZoomTrigger;

	// 시간을 계산할 변수 += fTimeDelta하여 duration보다 커지면 줌인/아웃이 끝난다.
	float m_fZoomDeltaTime;

	// 카메라 줌 비율 100이 기본 상황 / 50이면 기본에서 절반만큼 축소, 150이면 기본에서 두배만큼 확대
	float m_fZoomRatio;

	// ZoomRatio까지 가기까지 걸리는 시간
	float m_fZoomDuration;

	// 목표지점
	_vec3 m_vTowardMove;

	// 시간을 계산할 변수 += fTimeDelta하여 3보다 커지면 줌인이 시작된다.
	float m_fZoomInTimer;

	// Shake =========================================================
	float m_fShakeDeltaTime;
	bool m_bShakeTrigger;
	float fShakeTickTime;
	float m_fShakeDuration;
	float m_fShakePower;
	float m_fPrevShakeAtYPos;

	float elapsedTime = 0.0f;
	float shakeAmplitude = 0.1f;  // 흔들림의 세기 (높이의 범위)
	float shakeFrequency = 20.0f;  // 흔들림의 빈도 (초당 진동 횟수)
	float shakeDuration = 0.01f;   // 흔들림의 지속 시간

	// 진동 효과를 적용할 시간
	float shakeTimer = shakeDuration;

public:
	static  CDynamicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const _vec3* pEye
		, const _vec3* pAt
		, const _vec3* pUp
		, const _float& _fFov = D3DXToRadian(60.f)
		, const _float& _fAspect = (float)WINCX / WINCY
		, const _float& _Near = 0.1f
		, const _float& _fFar = 1000.f);

private:
	virtual  void Free();

};

