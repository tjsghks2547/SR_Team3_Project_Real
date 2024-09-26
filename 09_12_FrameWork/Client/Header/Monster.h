#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTriCol;
class CTransform;

END

class CMonster :public Engine::CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();


private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom; 
	//Engine::CTriCol*		m_pBufferCom;
	//Engine::CTransform*		m_pTransformCom;

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

