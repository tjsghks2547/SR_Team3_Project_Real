#pragma once
#include "GameObject.h"	

BEGIN(Engine)

class CAnimator2;
class CTransform;


class CWaterFall : public Engine::CGameObject
{
private:
	explicit CWaterFall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWaterFall();

public:
	virtual  HRESULT  Ready_GameObject();
	virtual  _int     Update_GameObject(const _float& fTimeDelta);
	virtual  void     LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void     Render_GameObject();


private:
	HRESULT  Add_Component();
	HRESULT  SetUp_Material();
	void     Key_Input(const _float& fTimeDelta);

private:
	Engine::CAnimator2* m_pAnimatorCom;
	Engine::CTransform* m_pTransformCom;


public:
	static CWaterFall* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};
END
