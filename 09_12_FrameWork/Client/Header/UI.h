#pragma once
#include "GameObject.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUI : public Engine::CGameObject
{
private:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI();
	virtual ~CUI();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();


private:
	HRESULT    Add_Component();

private:
	enum HPBAR { HPFRAME, HPBAR, HPBACK, HPEND };
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[HPEND];
	Engine::CTransform* m_pTransformCom[HPEND];

public:
	static CUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

