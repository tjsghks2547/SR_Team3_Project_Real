#pragma once
#include "Scene.h"
#include "DynamicCamera.h"
#include "Player.h"
#include "WorldHeartMap.h"

#include "Pipe.h"
#include "PipeBoard.h"
#include "Crystal.h"
#include "PressBlock.h"
#include "CrystalPuzzle.h"
#include "BoardCursor.h"
#include "MonkeyStatue.h"
#include "StoneBlock.h"
#include "StoneBlockHole.h"
#include "BugStatue.h"
#include "BugStatuePuzzle.h"
#include "CombinationPuzzle.h"
#include "CombinationStatue.h"
#include "MusicStatue.h"
#include "NoteStatue.h"
#include "FirePit.h"
#include "Scale.h"
#include "WeightComparator.h"
#include "Stone.h"
#include "StonePedestal.h"
#include "Catapult.h"
#include "ColorStone.h"
#include "ShootingPlant.h"
#include "LightFlower.h"

#include "DefaultUI.h"
#include "InvenUI.h"
#include "QuickSlot.h"


//여기다 넣을 오브젝트들 다 include하기

class CWorldHearStage : public CScene
{

private:
	explicit CWorldHearStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWorldHearStage();


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
	static CWorldHearStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual  void Free();
};

