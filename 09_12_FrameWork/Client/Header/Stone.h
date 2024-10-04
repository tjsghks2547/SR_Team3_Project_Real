#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CStone :public Engine::CGameObject
{
private:
	explicit CStone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStone();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	void Disable_Collider();
	void Set_StoneID(_int _iId) { m_iStoneID = _iId; }
	_int Get_StoneID() { return m_iStoneID; }
	_bool Is_Launched() { return m_bIsLaunched; }
	//void Set_Pushable(OBJ_TYPE _eType) { m_eObjType = _eType; }

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pShadowTextureCom;
	Engine::CTransform* m_pShadowTransformCom;
	Engine::CCollider* m_pBoundBox;

public:
	static CStone* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Launch();

private:
	_bool m_bIsLaunched;	
	vector<IDirect3DTexture9*> m_vecTexture;
	_int m_iStoneID;
	_vec3 m_vVelocity;	

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};