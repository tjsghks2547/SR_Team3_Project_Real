#pragma once
#include "PuzzleObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CWeightComparator :public Engine::CPuzzleObject
{
private:
	explicit CWeightComparator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWeightComparator();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	void Init(_float _fX, _float _fZ);
	void Puzzle_Clear();

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CWeightComparator* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:	
	_int m_iImageID;
	vector<IDirect3DTexture9*> m_vecTexture;
	vector<CGameObject*> m_vecPedestals;
	vector<CGameObject*> m_vecStones;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();

	// CPuzzleObject을(를) 통해 상속됨
	void Match_Puzzle() override;
};