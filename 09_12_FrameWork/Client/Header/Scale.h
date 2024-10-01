#pragma once
#include "PuzzleObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCollider;

END

class CScale :public Engine::CPuzzleObject
{
private:
	explicit CScale(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScale();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	void Set_ImageID(_int _iId) { m_iImageID = _iId; }
	_int Get_ImageID() { return m_iImageID; }
	void Init_Position(float _fX, float _fZ);
	void Set_LeftPedestal(CGameObject* _pObj) { m_pLeftPedestal = _pObj; }
	void Set_RightPedestal(CGameObject* _pObj) { m_pRightPedestal = _pObj; }	
	void Match_Puzzle() override;
	void Measure_Weight();

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

	Engine::CTexture* m_pLeftCompTextureCom;
	Engine::CTransform* m_pLeftCompTransformCom;

	Engine::CTexture* m_pRightCompTextureCom;
	Engine::CTransform* m_pRightCompTransformCom;

public:
	static CScale* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:	
	vector<IDirect3DTexture9*> m_vecTexture;
	CGameObject* m_pLeftPedestal;
	CGameObject* m_pRightPedestal;
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