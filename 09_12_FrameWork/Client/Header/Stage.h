#pragma once

#include "Scene.h"
#include "BackGround.h"

#include "Player.h"
#include "Monster.h"
#include "UI.h"
#include "Pipe.h"
#include "PipeBoard.h"
#include "BoardCursor.h"

#include "Terrain.h"
#include "DynamicCamera.h"

#include "SkyBox.h"
#include "TestMap.h"
#include "Map.h"


class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage();


public:
	virtual   HRESULT   Ready_Scene();
	virtual   _int		Update_Scene(const _float& fTimeDelta);
	virtual   void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual   void      Render_Scene();


private:
	HRESULT   Ready_LightInfo();
	HRESULT   Ready_Layer_Environmnet(const _tchar* pLayerTag);
	HRESULT   Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT   Ready_Layer_UI(const _tchar* pLayerTag);

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual  void Free();

};

