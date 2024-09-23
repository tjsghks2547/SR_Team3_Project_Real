#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CMonkeyStatue :public Engine::CGameObject
{
private:
	explicit CMonkeyStatue(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonkeyStatue();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	void Active_StoneBlock();
	void Add_StoneBlock(CGameObject* pSb) { m_vecStoneBlocks.push_back(pSb); }
	void Add_StoneBlockHole(CGameObject* pSbh) { m_vecStoneBlocksHoles.push_back(pSbh); };

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	HRESULT    Add_Component();

public:
	static CMonkeyStatue* Create(LPDIRECT3DDEVICE9 pGraphicDev);		
	void Key_Input(const _float& fTimeDelta);		

private:
	_bool m_bIs;
	vector<CGameObject*> m_vecStoneBlocks;
	vector<CGameObject*> m_vecStoneBlocksHoles;

private:
	virtual void Free();
};
