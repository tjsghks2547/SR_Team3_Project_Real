#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CStoneBlockHole :public Engine::CGameObject
{
private:
	explicit CStoneBlockHole(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStoneBlockHole();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void Set_ImageID(_int _iId) { m_iImageID = _iId; }
	_int Get_ImageID() { return m_iImageID; }
	void Init(_float _fX, _float _fZ, _int _iID);
	void Active_Block();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	HRESULT    Add_Component();

public:
	static CStoneBlockHole* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

private:
	vector<IDirect3DTexture9*> m_vecTexture;
	CTransform* m_pStoneBlock;
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