#pragma once
#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CComponent* Get_Component(COMPONENTID eID, const _tchar* pLayeTag, const _tchar* pObjTag, const _tchar* pComponentTag);
	CGameObject* Get_GameObject(const _tchar* pLayeTag, const _tchar* pObjTag);


public:
	virtual  HRESULT Ready_Scene();
	virtual  void    LateReady_Scene();
	virtual  _int    Update_Scene(const _float& fTimeDelta);
	virtual  void    LateUpdate_Scene(const _float& fTimeDelta);
	virtual  void    Render_Scene()PURE;

	//0922 Seonhwan
	virtual  void    init() { ; }


protected:
	map<const _tchar*, CLayer*>  m_mapLayer;
	LPDIRECT3DDEVICE9			 m_pGraphicDev; 


public:
	virtual  void  Free();


	//8월 26일 작업
public:
	map<const _tchar*, CLayer*>& GetLayerMapPtr() { return m_mapLayer; }
};

END