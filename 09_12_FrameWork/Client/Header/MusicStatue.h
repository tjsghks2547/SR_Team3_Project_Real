#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CMusicStatue :public Engine::CGameObject
{
private:
	explicit CMusicStatue(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMusicStatue();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:	
	void Add_StoneBlock(CGameObject* _pObj) { m_vecStoneBlocks.push_back(_pObj); }
	void Add_StoneBlockHole(CGameObject* _pObj) { m_vecStoneBlocksHoles.push_back(_pObj); }
	void Add_FirePit(CGameObject* _pObj) { m_vecFirePits.push_back(_pObj); }
	void Play_Music();
	void Match_Note(_int _iNote);
	void Reset_FirePit();
	void Clear_Puzzle();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	HRESULT    Add_Component();

private:
	_bool m_bIsClear;
	_int m_iCurNote;
	vector<_int> m_vecKeyOrder;
	vector<CGameObject*> m_vecFirePits;
	vector<CGameObject*> m_vecStoneBlocks;
	vector<CGameObject*> m_vecStoneBlocksHoles;	

public:
	static CMusicStatue* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

private:
	virtual void Free();
};
