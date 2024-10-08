#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CAnimator2;	
class CTransform;		
class CCollider;	

class CElectricEffect : public Engine::CGameObject
{

private:
	explicit CElectricEffect(LPDIRECT3DDEVICE9 pGraphicDev);	
	virtual ~CElectricEffect();	

public:
	virtual  HRESULT  Ready_GameObject();
	virtual  _int     Update_GameObject(const _float& fTimeDelta);
	virtual  void     LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void     Render_GameObject();

private:
	HRESULT  Add_Component();

private:
	Engine::CAnimator2* m_pAnimatorCom;	
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

public:
	static CElectricEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

private:
	virtual void Free();
	IDirect3DTexture9* m_pTexture; 
public:
	virtual void OnCollision(CGameObject* _pOther);


};

END