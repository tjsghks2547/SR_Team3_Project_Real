#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CStonePedestal :public Engine::CGameObject
{
private:
	explicit CStonePedestal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStonePedestal();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:	
	void Set_Group(CGameObject* _pObj) { m_pGroup = _pObj; }
	CGameObject* Get_Stone() { return m_pCollided; }

private:
	HRESULT    Add_Component();	
		
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CStonePedestal* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CGameObject* m_pCollided;
	CGameObject* m_pGroup;
	vector<IDirect3DTexture9*> m_vecTexture;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};