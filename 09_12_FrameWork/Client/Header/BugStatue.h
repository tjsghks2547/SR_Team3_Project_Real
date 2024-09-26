#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CBugStatue :public Engine::CGameObject
{
private:
	explicit CBugStatue(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBugStatue();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	void Active_StoneBlock();
	_int Get_ImageID() { return m_iImageID; }

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	HRESULT    Add_Component();

public:
	static CBugStatue* Create(LPDIRECT3DDEVICE9 pGraphicDev);	
	void Set_Group(CGameObject* _pStatuePuzzle) { m_pGroup = _pStatuePuzzle; }

private:	
	vector<IDirect3DTexture9*> m_vecTexture;
	CTransform* m_pStoneBlock;
	CGameObject* m_pGroup;
	_int m_iImageID;	

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}


private:
	virtual void Free();
};
