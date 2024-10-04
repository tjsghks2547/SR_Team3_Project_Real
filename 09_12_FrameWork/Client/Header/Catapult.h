#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CCatapult :public Engine::CGameObject
{
private:
	explicit CCatapult(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCatapult();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	void Init_Pos(_float _fX, _float _fZ);

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTexture* m_pBucketTextureCom;	
	Engine::CTransform* m_pTransformCom;
	Engine::CTransform* m_pTexTransformCom;
	Engine::CTransform* m_pBucketTransformCom;
	Engine::CTransform* m_pInTransformCom;
	Engine::CTexture* m_pInTextureCom;

	Engine::CCollider* m_pBoundBox;

public:
	static CCatapult* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_bool m_bIndicator;
	_float m_fPullDuration;
	CGameObject* m_pStone;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};

/*
	투석기.. 
	
	버켓, 버켓 콜라이더.

*/