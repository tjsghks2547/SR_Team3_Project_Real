#pragma once
#include "PuzzleObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CLightFlower :public Engine::CPuzzleObject
{
private:
	explicit CLightFlower(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightFlower();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual   void		LateReady_GameObject() override;

public:
	void Set_Glow(CGameObject* _pObj) { m_pGlow = _pObj; }
	void Match_Puzzle() override;

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

public:
	static CLightFlower* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CGameObject* m_pGlow;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};