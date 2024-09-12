#pragma once

#include "Camera.h"
#include "Base.h"
#include "Define.h"

BEGIN(Engine)	

class CRcTex;


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


	virtual   _int		Update_GameObject(const _float& fTimeDelta);
	virtual   void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject() {} // 순수가상함수로 구현되 있어서 그냥 적어만 놓음.
	


private:
	void  Key_Input(const _float& fTimeDelta);
	void  Mouse_Move();
	void  Mouse_Fix();


private:
	_bool  m_bFix; 
	_bool  m_bCheck; 

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


	// 9월 5일 이후 만든 코드
private:
	CPlayer* m_pPlayer; 

public:
	void SetPlayerPtr(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }
};

