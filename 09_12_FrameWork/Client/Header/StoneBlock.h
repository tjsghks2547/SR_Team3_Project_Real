#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CStoneBlock :public Engine::CGameObject
{
private:
	explicit CStoneBlock(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStoneBlock();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	HRESULT    Add_Component();

public:
	_bool Is_Move() { return m_bIsMove; }
	_int Get_ImageID() { return m_iImageID; }	
	void Set_ImageID(_int _iID) { m_iImageID = _iID; }	

private:	
	_bool m_bIsMove;
	_vec3 m_fTargetPos;
	vector<IDirect3DTexture9*> m_vecTexture;	
	_int m_iImageID;

public:
	static CStoneBlock* Create(LPDIRECT3DDEVICE9 pGraphicDev);		
	void Move_StoneBlock(_bool _bIsUp, _vec3 _fTargetPos)
	{
		m_bIsMove = true;
		m_fTargetPos = _fTargetPos;
	};


private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};