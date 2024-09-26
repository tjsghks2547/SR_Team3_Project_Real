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



private:
	void  Key_Input(const _float& fTimeDelta);
	void  Mouse_Move(const _float& fTimeDelta);
	void  MoveToPlayer(const _float& fTimeDelta);

	void  GetPlayerInfo();

private:

	// 0923 동영 : 카메라 모드에 따른 화면 출력을 위한 변수
	CTransform* m_playerTransform;
	_vec3			m_vPlayerPos;
	_vec3			m_vPlayerLook;

	_vec3			m_MoveToPlayerEye;
	_vec3			m_MoveToPlayerAt;
	CAMERASTATE m_eCameraState;

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

