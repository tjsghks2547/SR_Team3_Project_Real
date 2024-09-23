#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CPipeBoardCom;

END

class CPressBlock :public Engine::CGameObject
{
private:
	explicit CPressBlock(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPressBlock();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

private:
	HRESULT    Add_Component();

public:
	void On_CollisionEnter();
	void On_CollisionExit();

private:	
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;	

public:
	static CPressBlock* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CGameObject* m_pGroup;
	vector<IDirect3DTexture9*> m_vecTexture;
	_int m_iImageID;	
	_bool m_bIsPressed;
	_bool m_bIsCleared;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};