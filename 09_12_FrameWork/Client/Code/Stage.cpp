#include "pch.h"
#include "Stage.h"
#include "Export_Utility.h"
#include "WorldHeartStage.h"


CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);	
	FAILED_CHECK_RETURN(Ready_Layer_Environmnet(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	StopAll();		
	Engine::PlayBGM(L"BGM_1_CentralArea.wav", 1.f);	

	//Play_Sound(L"BGM_19_TutorialAmbience.wav", SOUND_EFFECT, 1.f);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	return S_OK;

}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
	_int  iExit = Engine::CScene::Update_Scene(fTimeDelta);

	if (GetAsyncKeyState('M') & 0x8000)
	{
		Engine::CScene* pStage2 = CWorldHearStage::Create(m_pGraphicDev);	
		NULL_CHECK_RETURN(pStage2, -1);


		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage2), E_FAIL);
		//pStage2->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용 

		return 0;
	}

	if (GetAsyncKeyState('9') & 0x8000)
	{
		Engine::CScene* pStage2 = CArenaStage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pStage2, -1);


		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage2), E_FAIL);
		//pStage2->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용 

		return 0;
	}

	return iExit;
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
	Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CStage::Render_Scene()
{


}


HRESULT CStage::Ready_LightInfo()
{

	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = { 1.f,1.f,1.f,1.f };
	tLightInfo.Specular = { 1.f,1.f,1.f,1.f };
	tLightInfo.Ambient = { 1.f,1.f,1.f,1.f };
	tLightInfo.Direction = { 1.F, -1.f, 1.f };

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CStage::Ready_Layer_Environmnet(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr; 

	_vec3 Eye = { 0.f, 0.f, 0.f };
	_vec3 At  = {0.f, 1.f, 1.f };
	_vec3 Up = { 0.f, 1.f, 0.f };

	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &Eye, &At, &Up);	

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);


	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	//pGameObject = CTestMap::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StartMap", pGameObject), E_FAIL);
	// 
	//여기다가 맵 오브젝트들 넣기 파일 읽어오는기능 
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;


	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);

	pGameObject = CMap::Create(m_pGraphicDev);	
	NULL_CHECK_RETURN(pGameObject, E_FAIL);		
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);

	//pGameObject = CTestMap::Create(m_pGraphicDev);	
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StartMap", pGameObject), E_FAIL);	
	 
	//pGameObject = CTerrain::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, pGameObject);
	pGameObject->SetObjectKey(L"Player");

	pGameObject = CPlayerInteractionBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlayerInteractionBox", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, pGameObject);
	pGameObject->SetObjectKey(L"PlayerInteractionBox");

	pGameObject = CSwingEffect::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SwingEffect", pGameObject), E_FAIL);

	CGameObject* PlayerObj = pLayer->Get_GameObject(L"Layer_GameLogic", L"Player");
	CGameObject* InteractionObj = pLayer->Get_GameObject(L"Layer_GameLogic", L"PlayerInteractionBox");
	dynamic_cast<CPlayerInteractionBox*>(InteractionObj)->SetPlayer(
		dynamic_cast<CPlayer*>(PlayerObj));
	CGameObject* SwingEffect = pLayer->Get_GameObject(L"Layer_GameLogic", L"SwingEffect");
	dynamic_cast<CSwingEffect*>(SwingEffect)->SetPlayer(
		dynamic_cast<CPlayer*>(PlayerObj));

	//0926
	pGameObject = CExpressMonkey::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_ExMonkey", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);


	//1002
	pGameObject = CSheep::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Sheep", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

	pGameObject = CPig::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Pig", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

	pGameObject = CCow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Cow", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);

	//1003
	/*pGameObject = CBranch::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_vec3 ItemPos = { 300.f, 20.f, 700.f };
	dynamic_cast<CBranch*>(pGameObject)->Set_DropItem(ItemPos);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_Branch", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);*/

	pGameObject = CHeartFruit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_vec3 ItemPos = { 300.f, 20.f, 700.f };
	dynamic_cast<CHeartFruit*>(pGameObject)->Set_Pos(_vec3{ 300.f, 20.f, 700.f });
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HeartFruit", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);


	pGameObject = CExpressTicket::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	ItemPos = { 330.f, 20.f, 700.f };
	dynamic_cast<CExpressTicket*>(pGameObject)->Set_DropItem(ItemPos);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_ExpressTicket", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);

	pGameObject = CTestCol::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestCol", pGameObject), E_FAIL);
	dynamic_cast<CTestCol*>(pGameObject)->SetPlayer(
		dynamic_cast<CPlayer*>(PlayerObj)
	);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);


	pGameObject = CTestCol2::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestCol2", pGameObject), E_FAIL);
	dynamic_cast<CTestCol2*>(pGameObject)->SetPlayer(
		dynamic_cast<CPlayer*>(PlayerObj)
	);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);

	pGameObject = CMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	dynamic_cast<CMonster*>(pGameObject)->SetPlayer(
		dynamic_cast<CPlayer*>(PlayerObj)
	);

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

	// 1003 동영 일반몬스터
	pGameObject = CMonsterSlimeFast::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonsterSlimeFast", pGameObject), E_FAIL);
	dynamic_cast<CMonster*>(pGameObject)->SetPlayer(
		dynamic_cast<CPlayer*>(PlayerObj));
	dynamic_cast<CMonster*>(pGameObject)->GetLayer(pLayer);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

	_tchar* objectName = new _tchar[32];
	swprintf(objectName, 32, L"MonsterMothMage%d", 0);

	pGameObject = CMonsterMothMage::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(objectName, pGameObject), E_FAIL);

	dynamic_cast<CMonsterMothMage*>(pGameObject)->testNum = 0;

	dynamic_cast<CMonster*>(pGameObject)->SetPlayer(
		dynamic_cast<CPlayer*>(PlayerObj));

	dynamic_cast<CMonster*>(pGameObject)->GetLayer(pLayer);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

	CScene* pScene = CManagement::GetInstance()->GetCurScenePtr();

	//0927
	pGameObject = CTestObject::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Test_Object", pGameObject), E_FAIL);


	//1010
	pGameObject = CGrass::Create(m_pGraphicDev);	
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Grass", pGameObject), E_FAIL);
	dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(350.f, 20.f, 180.f);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);	

#pragma region Pipe Game

	_float fOffsetX = 100;
	_float fOffsetZ = 600;

	/*pGameObject = CPipeBoard::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PipeBoard", pGameObject), E_FAIL);*/
#pragma endregion

#pragma region Crystal Puzzle
	//Engine::CGameObject* pCGameObject = nullptr;
	//pCGameObject = CCrystalPuzzle::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pCGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrystalPuzzle", pCGameObject), E_FAIL);
	//CCrystalPuzzle* pCrystalPuzzle = static_cast<CCrystalPuzzle*>(pCGameObject);

	//pGameObject = CCrystal::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 18.f, fOffsetZ);
	//pCrystalPuzzle->Add_Crystal(pGameObject);

	//pGameObject = CPressBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_00", pGameObject), E_FAIL);	
	//static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX, fOffsetZ - 35.f);
	//pCrystalPuzzle->Add_PressBlock(pGameObject);

	//pGameObject = CCrystal::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 35.f, 18.f, fOffsetZ);
	//static_cast<CCrystal*>(pGameObject)->Set_ImageID(3);
	//pCrystalPuzzle->Add_Crystal(pGameObject);

	//pGameObject = CPressBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_01", pGameObject), E_FAIL);	
	//static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 4, fOffsetX + 35.f, fOffsetZ - 35.f);
	//pCrystalPuzzle->Add_PressBlock(pGameObject);

	//pGameObject = CCrystal::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 70.f, 18.f, fOffsetZ);
	//pCrystalPuzzle->Add_Crystal(pGameObject);

	//pGameObject = CPressBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_02", pGameObject), E_FAIL);
	//static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + 70.f, fOffsetZ - 35.f);
	//pCrystalPuzzle->Add_PressBlock(pGameObject);

#pragma endregion

#pragma region Monkey Statue Puzzle
	//pGameObject = CMonkeyStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyStatue_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 22.f,fOffsetZ);
	//CMonkeyStatue* pMS = static_cast<CMonkeyStatue*>(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_00", pGameObject), E_FAIL);
	//static_cast<CStoneBlock*>(pGameObject)->Init(100.f + fOffsetX, 20.f + fOffsetZ, true);
	//pMS->Add_StoneBlock(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_01", pGameObject), E_FAIL);	
	//static_cast<CStoneBlock*>(pGameObject)->Init(150.f + fOffsetX, 20.f + fOffsetZ, false);
	//pMS->Add_StoneBlock(pGameObject);

#pragma endregion

#pragma region Bug Statue Puzzle

	//pGameObject = CBugStatuePuzzle::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BugStatuePuzzle_00", pGameObject), E_FAIL);
	//CBugStatuePuzzle* pBS = static_cast<CBugStatuePuzzle*>(pGameObject);

	//pGameObject = CBugStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BugStatue_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(60.f, 4.f, 10.f);
	//static_cast<CBugStatue*>(pGameObject)->Set_Group(pBS);
	//pBS->Add_Statue(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_10", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(60.f, 3.f, 20.f);
	//static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	//pBS->Add_StoneBlock(pGameObject);

	//pGameObject = CStoneBlockHole::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlockHole_10", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(60.f, 0.05f, 22.5f);
	//pBS->Add_StoneBlockHole(pGameObject);

	//pGameObject = CBugStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BugStatue_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(40.f, 4.f, 10.f);
	//static_cast<CBugStatue*>(pGameObject)->Set_Group(pBS);
	//pBS->Add_Statue(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_11", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(40.f, 3.f, 20.f);
	//static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	//pBS->Add_StoneBlock(pGameObject);

	//pGameObject = CStoneBlockHole::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlockHole_11", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(40.f, 0.05f, 22.5f);
	//pBS->Add_StoneBlockHole(pGameObject);

	//pGameObject = CBugStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BugStatue_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(50.f, 4.f, 10.f);
	//static_cast<CBugStatue*>(pGameObject)->Set_Group(pBS);
	//pBS->Add_Statue(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_12", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(50.f, 3.f, 20.f);
	//static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	//pBS->Add_StoneBlock(pGameObject);

	//pGameObject = CStoneBlockHole::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlockHole_12", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(50.f, 0.05f, 22.5f);
	//pBS->Add_StoneBlockHole(pGameObject);
#pragma endregion

#pragma region Combination Puzzle

	//pGameObject = CCombinationPuzzle::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CCombinationPuzzle_00", pGameObject), E_FAIL);
	//CCombinationPuzzle* pCombination = static_cast<CCombinationPuzzle*>(pGameObject);

	//pGameObject = CCombinationStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CombinationStatue_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(40.f, 4.f, 40.f);
	//static_cast<CCombinationStatue*>(pGameObject)->Set_Group(pCombination);
	//pCombination->Add_Statue(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_13", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(45.f, 3.f, 50.f);
	//static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	//pCombination->Add_StoneBlock(pGameObject);

	//pGameObject = CStoneBlockHole::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlockHole_13", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(45.f, 0.05f, 52.5f);
	//pCombination->Add_StoneBlockHole(pGameObject);

	//pGameObject = CCombinationStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CombinationStatue_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(50.f, 4.f, 40.f);
	//static_cast<CCombinationStatue*>(pGameObject)->Set_Group(pCombination);
	//pCombination->Add_Statue(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_14", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(55.f, 3.f, 50.f);
	//static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	//pCombination->Add_StoneBlock(pGameObject);

	//pGameObject = CStoneBlockHole::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlockHole_14", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(55.f, 0.05f, 52.5f);
	//pCombination->Add_StoneBlockHole(pGameObject);

	//pGameObject = CCombinationStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CombinationStatue_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(60.f, 4.f, 40.f);
	//static_cast<CCombinationStatue*>(pGameObject)->Set_Group(pCombination);
	//pCombination->Add_Statue(pGameObject);
#pragma endregion

#pragma region Music Puzzle

	//pGameObject = CMusicStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MusicStatue_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(70.f, 4.f, 40.f);
	//CMusicStatue* pMusicStatue = static_cast<CMusicStatue*>(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_15", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(75.f, 3.f, 50.f);
	//static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	//pMusicStatue->Add_StoneBlock(pGameObject);

	//pGameObject = CStoneBlockHole::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlockHole_15", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(75.f, 0.05f, 52.5f);
	//pMusicStatue->Add_StoneBlockHole(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_16", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(85.f, 3.f, 50.f);
	//static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	//pMusicStatue->Add_StoneBlock(pGameObject);

	//pGameObject = CStoneBlockHole::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlockHole_16", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(85.f, 0.05f, 52.5f);
	//pMusicStatue->Add_StoneBlockHole(pGameObject);

	//pGameObject = CNoteStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoteStatue_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(75.f, 4.f, 20.f);
	//static_cast<CNoteStatue*>(pGameObject)->Set_NoteID(0);
	//static_cast<CNoteStatue*>(pGameObject)->Set_Group(pMusicStatue);

	//pGameObject = CNoteStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoteStatue_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(85.f, 4.f, 20.f);
	//static_cast<CNoteStatue*>(pGameObject)->Set_NoteID(1);
	//static_cast<CNoteStatue*>(pGameObject)->Set_Group(pMusicStatue);

	//pGameObject = CNoteStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoteStatue_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(95.f, 4.f, 20.f);
	//static_cast<CNoteStatue*>(pGameObject)->Set_NoteID(2);
	//static_cast<CNoteStatue*>(pGameObject)->Set_Group(pMusicStatue);

	//pGameObject = CNoteStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoteStatue_03", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(105.f, 4.f, 20.f);
	//static_cast<CNoteStatue*>(pGameObject)->Set_NoteID(3);
	//static_cast<CNoteStatue*>(pGameObject)->Set_Group(pMusicStatue);

	//pGameObject = CNoteStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoteStatue_04", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(115.f, 4.f, 20.f);
	//static_cast<CNoteStatue*>(pGameObject)->Set_NoteID(4);
	//static_cast<CNoteStatue*>(pGameObject)->Set_Group(pMusicStatue);

	//pGameObject = CNoteStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NoteStatue_05", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(125.f, 4.f, 20.f);
	//static_cast<CNoteStatue*>(pGameObject)->Set_NoteID(5);
	//static_cast<CNoteStatue*>(pGameObject)->Set_Group(pMusicStatue);

	//pGameObject = CFirePit::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirePit_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(75.f, 4.f, 60.f);
	//pMusicStatue->Add_FirePit(pGameObject);

	//pGameObject = CFirePit::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirePit_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(85.f, 4.f, 60.f);
	//pMusicStatue->Add_FirePit(pGameObject);

	//pGameObject = CFirePit::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirePit_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(95.f, 4.f, 60.f);
	//pMusicStatue->Add_FirePit(pGameObject);

	//pGameObject = CFirePit::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirePit_03", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(105.f, 4.f, 60.f);
	//pMusicStatue->Add_FirePit(pGameObject);

	//pGameObject = CFirePit::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirePit_04", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(115.f, 4.f, 60.f);
	//pMusicStatue->Add_FirePit(pGameObject);

	//pGameObject = CFirePit::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirePit_05", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(125.f, 4.f, 60.f);
	//pMusicStatue->Add_FirePit(pGameObject);

#pragma endregion

#pragma region Scale Puzzle


	//pGameObject = CScale::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CScale_00", pGameObject), E_FAIL);
	//static_cast<CScale*>(pGameObject)->Init_Position(170.f + fOffsetX, 150.f + fOffsetZ);

	//pGameObject = CWeightComparator::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WeightComparator_00", pGameObject), E_FAIL);	
	//static_cast<CWeightComparator*>(pGameObject)->Init(170.f + fOffsetX, 30.f + fOffsetZ);

#pragma endregion

#pragma region Catapult
	//pGameObject = CCatapult::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_00", pGameObject), E_FAIL);
	//static_cast<CCatapult*>(pGameObject)->Init_Pos(200.f, 450.f);

	//pGameObject = CStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStone_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(250.f, 15.f, 450.f);

	//pGameObject = CStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStone_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(150.f, 15.f, 400.f);

	//pGameObject = CBreakableStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CBreakableStone_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(200.f, 14.f, 700.f);

#pragma endregion

#pragma region ShootingPlant
	//pGameObject = CLightFlower::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LightFlower_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(200.f, 30.f, 610.f);
	//CLightFlower* pLightFlower = static_cast<CLightFlower*>(pGameObject);

	//pGameObject = CFlowerGlow::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FlowerGlow_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(200.f, 15.f, 610.f);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(200.f, 0.05f, 600.f);
	//pLightFlower->Set_Glow(pGameObject);

	//pGameObject = CShootingPlant::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShootingPlant_00", pGameObject), E_FAIL);
	//static_cast<CShootingPlant*>(pGameObject)->Init_Pos(400.f, 600.f);	

	//pGameObject = CShootingPlant::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShootingPlant_01", pGameObject), E_FAIL);
	//static_cast<CShootingPlant*>(pGameObject)->Set_ImageID(0);
	//static_cast<CShootingPlant*>(pGameObject)->Init_Pos(450.f, 600.f);

#pragma endregion

#pragma region Color Stone
	//pGameObject = CColorStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(200.f, 14.f, 600.f);
	//static_cast<CColorStone*>(pGameObject)->Set_ImageID(0);

	//pGameObject = CColorStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_01", pGameObject), E_FAIL);
	//static_cast<CColorStone*>(pGameObject)->Set_ImageID(1);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(250.f, 14.f, 600.f);

	//pGameObject = CColorStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_02", pGameObject), E_FAIL);
	//static_cast<CColorStone*>(pGameObject)->Set_ImageID(2);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(300.f, 14.f, 600.f);

	//pGameObject = CColorStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_03", pGameObject), E_FAIL);
	//static_cast<CColorStone*>(pGameObject)->Set_ImageID(2);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(200.f, 14.f, 640.f);

	//pGameObject = CColorStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_04", pGameObject), E_FAIL);
	//static_cast<CColorStone*>(pGameObject)->Set_ImageID(2);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(250.f, 14.f, 640.f);

	//pGameObject = CColorStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_05", pGameObject), E_FAIL);
	//static_cast<CColorStone*>(pGameObject)->Set_ImageID(2);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(300.f, 14.f, 640.f);

#pragma endregion

#pragma region Hole
	//pGameObject = CStoneHole::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(45.f + fOffsetX, 0.05f, fOffsetZ - 100.5f);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(45.f + fOffsetX, 0.05f, fOffsetZ - 100.5f);
#pragma endregion



	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CDefaultUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Default_UI", pGameObject), E_FAIL);

	pGameObject = CInvenUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Inven_UI", pGameObject), E_FAIL);

	pGameObject = CQuickSlot::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"QuickSlot_UI", pGameObject), E_FAIL);

	pGameObject = CQuestUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Quest_UI", pGameObject), E_FAIL);

	pGameObject = CPowerUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Power_UI", pGameObject), E_FAIL);

	pGameObject = CSpeedUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Speed_UI", pGameObject), E_FAIL);

	pGameObject = CStoreUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Store_UI", pGameObject), E_FAIL);

	pGameObject = CAddHP::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"AddHP_UI", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage* pStage = new CStage(pGraphicDev);

	//if (FAILED(pStage->Ready_Scene()))
	//{
	//	Safe_Release(pStage);
	//	MSG_BOX("Stage Create Failed");
	//	return nullptr;
	//}
	return pStage;
}

void CStage::Free()
{
	Engine::CScene::Free();
}


void CStage::init()
{
	Engine::CLayer* pLayer = CLayer::Create();	
	
	DWORD bytesRead = 1; 
	HANDLE hFile = CreateFile(L"../Map/realmap10.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		while (bytesRead > 0)
		{
			//wstring ObjectName = L"";
			//WCHAR* buffer = new WCHAR[256];
			WCHAR buffer[256] = { 0 };	
			bytesRead = 100;

			//오브젝트키 (오브젝트 이름)
			if (!ReadFile(hFile, buffer, 100, &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ TextrueKey");
				CloseHandle(hFile);
			}
			
			wstring* pObjectName = new wstring;
			*pObjectName = buffer;
		
			



			//wstring TextureName = L"";
			//WCHAR* buffer2 = new WCHAR[256];
			WCHAR buffer2[256] = { 0 };	
			bytesRead = 100;

			//텍스처키 (텍스처 이름)
			if (!ReadFile(hFile, buffer2, 100, &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ TextrueKey");
				CloseHandle(hFile);
			}

			wstring* pTextureWstring = new wstring;
			*pTextureWstring = buffer2;

			


			int a = 4;


			D3DXMATRIX worldmatrix;
			bytesRead = 0;

			if (!ReadFile(hFile, worldmatrix, sizeof(D3DXMATRIX), &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ WORLDMAXTIRX");
				CloseHandle(hFile);
			}

			D3DXVECTOR3 Rotation_vec3;
			bytesRead = 0;

			if (!ReadFile(hFile, Rotation_vec3, sizeof(D3DXVECTOR3), &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ D3DXVECTOR3 INFO");
				CloseHandle(hFile);
			}

			D3DXVECTOR3 Sclae_vec3;
			bytesRead = 0;

			if (!ReadFile(hFile, Sclae_vec3, sizeof(D3DXVECTOR3), &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ D3DXVECTOR3 INFO");
				CloseHandle(hFile);
			}


			Engine::CGameObject* pGameObject = nullptr;

			pGameObject = CObject::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
			if (pGameObject == nullptr)
			{
				MSG_BOX("CObject nullptr Error");
			}

			

			pGameObject->SetTextureKey((*pTextureWstring).c_str());

			//  지금 신이 변경이 안돼서 startScene을 가져와서 문제 발생 



			map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
			pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*pObjectName).c_str(), pGameObject);

			CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

			// 왜 못찾는거지?;;; 아 시발 아직도 신이 안바뀐거였음  이거 찾는 기준이 현재신기준이였음.
			CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", (*pObjectName).c_str(), L"Com_Transform"));
			//pTransform->ForGetWorldMaxtrix() = worldmatrix;

			
			// 월드매트릭스에 넣기 전의 크기값을 넣어줘야하네 
			pTransform->m_vScale = { Sclae_vec3.x,Sclae_vec3.y,Sclae_vec3.z };
			pTransform->Rotation(ROT_X, Rotation_vec3.x * 3.14f / 180.f);
			pTransform->Rotation(ROT_Y, Rotation_vec3.y * 3.14f / 180.f);
			pTransform->Rotation(ROT_Z, Rotation_vec3.z * 3.14f / 180.f);
			pTransform->Set_Pos(worldmatrix._41, worldmatrix._42, worldmatrix._43);

			//pTransform->m_vAngle = vec3;
			//D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
		}
		CloseHandle(hFile);
	}
	

	//9월25일 충돌관련
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::OBJECT);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::NPC);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PUZZLE, GROUP_TYPE::PUZZLE);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER_EFFECT);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::OBJECT);	
}