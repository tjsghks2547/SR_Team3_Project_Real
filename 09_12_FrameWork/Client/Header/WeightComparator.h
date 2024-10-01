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
	void Set_ImageID(_int _iId) { m_iImageID = _iId; }
	_int Get_ImageID() { return m_iImageID; }
	void Add_Pedestal(CGameObject* _pObj) { m_vecPedestal.push_back(_pObj); }
	void Add_Stone(CGameObject* _pObj) { m_vecStone.push_back(_pObj); }
	void Set_PedestalPos(float _fX, float _fZ);	
	void Puzzle_Clear();
	void Add_StoneBlock(CGameObject* _pObj) { m_vecStoneBlocks.push_back(_pObj); }
	void Add_StoneBlockHole(CGameObject* _pObj) { m_vecStoneBlocksHoles.push_back(_pObj); }
		 
private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CWeightComparator* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:	
	vector<IDirect3DTexture9*> m_vecTexture;
	vector<CGameObject*> m_vecPedestal;
	vector<CGameObject*> m_vecStone;
	vector<CGameObject*> m_vecStoneBlocks;
	vector<CGameObject*> m_vecStoneBlocksHoles;
	_int m_iImageID;

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