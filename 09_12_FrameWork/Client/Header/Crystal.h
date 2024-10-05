#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CCrystal :public Engine::CGameObject
{
private:
	explicit CCrystal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrystal();

public:	
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	void Set_Active(_bool _bIsActive) { m_bIsActive = _bIsActive; }
	_bool Is_Active() { return m_bIsActive; }
	void Set_ImageID(_int _iId) { m_iImageID = _iId; }
	_int Get_ImageID() { return m_iImageID; }

private:	
	HRESULT    Add_Component();	

private:	
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CCrystal* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_bool m_bIsActive;
	vector<IDirect3DTexture9*> m_vecTexture;
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