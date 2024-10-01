#pragma once
#include "GameObject.h"
//#include "SoundManager.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CFirePit :public Engine::CGameObject
{
private:
	explicit CFirePit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFirePit();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	void Set_ImageID(_int _iID) { m_iImageID = _iID; }

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

private:
	HRESULT    Add_Component();

private:
	vector<IDirect3DTexture9*> m_vecTexture;
	_int m_iImageID;

public:
	static CFirePit* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};
