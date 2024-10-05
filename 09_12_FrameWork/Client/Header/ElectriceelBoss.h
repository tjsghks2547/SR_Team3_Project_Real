#pragma once
#include "GameObject.h"

enum class Electriceel_STATE
{
	IN_OUT_9,
	IN_OUT_11,
	IN_OUT_12,
	IN_OUT_1,
	IN_OUT_3,

	IDLE,
	ATTACK,

};



BEGIN(Engine)

class CAnimator2;
class CTransform;
class CCollider;

class CElectriceelBoss  : public Engine::CGameObject
{
private:
	explicit CElectriceelBoss(LPDIRECT3DDEVICE9 pGraphicDev);	
	virtual ~CElectriceelBoss();

public:
	void update_move();
	void update_state();
	void update_sound();
	void update_animation();


public:
	virtual  HRESULT  Ready_GameObject();
	virtual  _int     Update_GameObject(const _float& fTimeDelta);
	virtual  void     LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void     Render_GameObject();

private:
	HRESULT  Add_Component();
	HRESULT  SetUp_Material();
	void     Key_Input(const _float& fTimeDelta);

private:

	Electriceel_STATE m_eCurState;
	Electriceel_STATE m_ePreState;

	ULONGLONG dwtime;	
	Engine::CAnimator2* m_pAnimatorCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;

public:
	static CElectriceelBoss* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

private:
	vector<IDirect3DTexture9*> m_vecTexture;
	IDirect3DTexture9* m_pComeTex;

public:
	virtual void OnCollision(CGameObject* _pOther);

private:

	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

};

END