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
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	void Set_Active(_bool _bIsActive) { m_bIsActive = _bIsActive; }
	_bool Is_Active() { return m_bIsActive; }
	void Set_ImageID(_int _iId) { m_iImageID = _iId; }
	_int Get_ImageID() { return m_iImageID; }
	void Set_StoneID(_int _iId) { m_iStoneID = _iId; }
	_int Get_StoneID() { return m_iStoneID; }
	void Launch();

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

public:
	static CStone* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_bool m_bIsActive;
	_bool m_bIsLaunched;
	_vec3 m_vVelocity;
	vector<IDirect3DTexture9*> m_vecTexture;
	_int m_iStoneID;
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