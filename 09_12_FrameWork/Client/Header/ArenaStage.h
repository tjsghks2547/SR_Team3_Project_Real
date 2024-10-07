#pragma once
#include "Scene.h"
#include "DynamicCamera.h"
#include "Player.h"

#include "DefaultUI.h"
#include "StoreUI.h"
#include "InvenUI.h"
#include "QuickSlot.h"
#include "Object.h"

#include "ArenaMap.h"


class CArenaStage : public CScene
{
	
private:
	explicit CArenaStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CArenaStage();


public:
	virtual   HRESULT   Ready_Scene();	
	virtual   _int		Update_Scene(const _float& fTimeDelta);	
	virtual   void		LateUpdate_Scene(const _float& fTimeDelta);	
	virtual   void      Render_Scene();	
	virtual   void		init();	

private:
	HRESULT   Ready_LightInfo();	
	HRESULT   Ready_Layer_Environmnet(const _tchar* pLayerTag);	
	HRESULT   Ready_Layer_GameLogic(const _tchar* pLayerTag);	
	HRESULT   Ready_Layer_UI(const _tchar* pLayerTag);	


public:
	static CArenaStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

private:
	virtual  void Free();	


};

