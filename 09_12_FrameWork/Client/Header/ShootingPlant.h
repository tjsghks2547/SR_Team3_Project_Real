#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CShootingPlant :public Engine::CGameObject
{
private:
	explicit CShootingPlant(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShootingPlant();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:	
	void Set_ImageID(_int _iId) { m_iImageID = _iId; }
	_int Get_ImageID() { return m_iImageID; }
	void Init_Pos(_float _fX, _float _fY);
	void Shoot(const _float& fTimeDelta);
	void Create_Bullet();

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;	
	Engine::CTransform* m_pTexTransformCom;
	Engine::CCollider* m_pBoundBox;

public:
	static CShootingPlant* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:	
	_bool m_bIsShooting;
	vector<IDirect3DTexture9*> m_vecTexture;
	vector<CGameObject*> m_vecBullets;
	_float m_fShootTimer;
	_float m_fShootTimerMax;
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