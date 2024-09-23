#pragma once
#include "GameObject.h"


BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CBoundBox;

END


class CObject : public Engine::CGameObject
{

private:
	explicit CObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CObject();

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
	Engine::CRcTex*     m_pBufferCom;
	Engine::CTexture*   m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CBoundBox*  m_pBoundBox;

public:
	static CObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

private:
	vector<IDirect3DTexture9*> m_vecTexture;
	map<string, IDirect3DTexture9*> m_mapTexture;


private:
	//LPD3DXMESH  m_pMesh;
};

