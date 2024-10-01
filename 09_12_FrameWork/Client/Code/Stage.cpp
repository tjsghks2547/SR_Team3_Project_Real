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

	
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	

	Engine::Sound_Initialize();
	Engine::Play_Sound(L"BGM_1_CentralArea.wav", SOUND_BGM, 1.f);

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
		pStage2->init(); // �������� ������ ������Ʈ���� ���� ��� 

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
	//����ٰ� �� ������Ʈ�� �ֱ� ���� �о���±�� 
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

	CGameObject* PlayerObj = pLayer->Get_GameObject(L"Layer_GameLogic", L"Player");
	CGameObject* InteractionObj = pLayer->Get_GameObject(L"Layer_GameLogic", L"PlayerInteractionBox");
	dynamic_cast<CPlayerInteractionBox*>(InteractionObj)->SetPlayer(
		dynamic_cast<CPlayer*>(PlayerObj));

	//0926
	pGameObject = CExpressMonkey::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_ExMonkey", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::NPC, pGameObject);



	pGameObject = CTestCol::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestCol", pGameObject), E_FAIL);
	dynamic_cast<CTestCol*>(pGameObject)->SetPlayer(
		dynamic_cast<CPlayer*>(PlayerObj)
	);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);

	CScene* pScene = CManagement::GetInstance()->GetCurScenePtr();

	//0926
	pGameObject = CExpressMonkey::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_ExMonkey", pGameObject), E_FAIL);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);


	//0927
	pGameObject = CTestObject::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Test_Object", pGameObject), E_FAIL);

	//pGameObject = CMonster::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);

	

	//pGameObject = CSkyBox::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

#pragma region Pipe Game
	//pGameObject = CPipeBoard::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PipeBoard", pGameObject), E_FAIL);
#pragma endregion

#pragma region Crystal Puzzle
	Engine::CGameObject* pCP = nullptr;
	pCP = CCrystalPuzzle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCP, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrystalPuzzle", pCP), E_FAIL);
	CCrystalPuzzle* temp = static_cast<CCrystalPuzzle*>(pCP);

	pGameObject = CCrystal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(10.f, 5.55f, 110.f);
	temp->Add_Crystal(pGameObject);

	pGameObject = CPressBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(10.f, .5f, 90.f);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(10.f, 6.f, 90.f);
	static_cast<CPressBlock*>(pGameObject)->Set_ImageID(2);
	static_cast<CPressBlock*>(pGameObject)->Set_Group(pCP);
	temp->Add_PressBlock(pGameObject);

	//pGameObject = CCrystal::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(25.f, 5.55f, 100.f);
	//static_cast<CCrystal*>(pGameObject)->Set_ImageID(3);
	//temp->Add_Crystal(pGameObject);

	//pGameObject = CPressBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(25.f, 6.f, 90.f);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(25.f, 0.5f, 90.f);
	//static_cast<CPressBlock*>(pGameObject)->Set_Group(pCP);
	//static_cast<CPressBlock*>(pGameObject)->Set_ImageID(0);
	//temp->Add_PressBlock(pGameObject);

	//pGameObject = CCrystal::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(40.f, 5.55f, 100.f);
	//temp->Add_Crystal(pGameObject);

	//pGameObject = CPressBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(40.f, 6.f, 90.f);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(40.f, 0.5f, 90.f);
	//static_cast<CPressBlock*>(pGameObject)->Set_Group(pCP);
	//static_cast<CPressBlock*>(pGameObject)->Set_ImageID(2);
	//temp->Add_PressBlock(pGameObject);

	//pCP = CCrystalPuzzle::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pCP, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrystalPuzzle_1", pCP), E_FAIL);
	//temp = static_cast<CCrystalPuzzle*>(pCP);

	//pGameObject = CCrystal::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_03", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(10.f, 5.55f, 50.f);
	//temp->Add_Crystal(pGameObject);

	//pGameObject = CPressBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_03", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(10.f, 6.f, 40.f);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(10.f, 0.5f, 40.f);
	//static_cast<CPressBlock*>(pGameObject)->Set_Group(pCP);
	//static_cast<CPressBlock*>(pGameObject)->Set_ImageID(1);
	//temp->Add_PressBlock(pGameObject);
#pragma endregion

#pragma region Monkey Statue Puzzle
	pGameObject = CMonkeyStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyStatue_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(10.f, 20.f, 10.f);
	CMonkeyStatue* pMS = static_cast<CMonkeyStatue*>(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(10.f, 20.f, 40.f);
	pMS->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlockHole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlockHole_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(10.f, 0.05f, 55.f);
	pMS->Add_StoneBlockHole(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_01", pGameObject), E_FAIL);	
	static_cast<CStoneBlock*>(pGameObject)->Init(50.f, 40.f, false, 0);
	pMS->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlockHole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlockHole_01", pGameObject), E_FAIL);	
	static_cast<CStoneBlockHole*>(pGameObject)->Init(50.f, 55.f, 1);
	pMS->Add_StoneBlockHole(pGameObject);

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
	pGameObject = CScale::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CScale_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(170.f, 15.f, 60.f);
	CScale* scale = static_cast<CScale*>(pGameObject);\

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStonePedestal_00", pGameObject), E_FAIL);
	scale->Set_LeftPedestal(pGameObject);
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(scale);

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStonePedestal_01", pGameObject), E_FAIL);
	scale->Set_RightPedestal(pGameObject);
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(scale);
	scale->Init_Position(170.f, 55.f);

	pGameObject = CWeightComparator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WeightComparator_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(170.f, .02f, 30.f);
	CWeightComparator* weightComparator = static_cast<CWeightComparator*>(pGameObject);

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStonePedestal_02", pGameObject), E_FAIL);
	weightComparator->Add_Pedestal(pGameObject);
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(weightComparator);

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStonePedestal_03", pGameObject), E_FAIL);
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(weightComparator);
	weightComparator->Add_Pedestal(pGameObject);

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStonePedestal_04", pGameObject), E_FAIL);
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(weightComparator);
	weightComparator->Add_Pedestal(pGameObject);

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStonePedestal_05", pGameObject), E_FAIL);
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(weightComparator);
	weightComparator->Add_Pedestal(pGameObject);

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStone_00", pGameObject), E_FAIL);
	static_cast<CStone*>(pGameObject)->Set_StoneID(0);
	weightComparator->Add_Stone(pGameObject);

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStone_02", pGameObject), E_FAIL);
	static_cast<CStone*>(pGameObject)->Set_StoneID(1);
	weightComparator->Add_Stone(pGameObject);

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStone_03", pGameObject), E_FAIL);
	static_cast<CStone*>(pGameObject)->Set_StoneID(2);
	weightComparator->Add_Stone(pGameObject);

	pGameObject = CStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CStone_04", pGameObject), E_FAIL);
	weightComparator->Add_Stone(pGameObject);
	static_cast<CStone*>(pGameObject)->Set_StoneID(3);
	weightComparator->Set_PedestalPos(170.f, 30.f);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_15", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(55.f, 3.f, 50.f);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	weightComparator->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlockHole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlockHole_15", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(55.f, 0.05f, 52.5f);
	weightComparator->Add_StoneBlockHole(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_16", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(55.f, 3.f, 50.f);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	weightComparator->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlockHole::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlockHole_16", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(55.f, 0.05f, 52.5f);
	weightComparator->Add_StoneBlockHole(pGameObject);


#pragma endregion

#pragma region Catapult
	pGameObject = CCatapult::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Catapult_00", pGameObject), E_FAIL);
	static_cast<CCatapult*>(pGameObject)->Init_Pos(170.f - 22.5f, 15.f);
#pragma endregion

#pragma region ColorStone
	pGameObject = CColorStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_00", pGameObject), E_FAIL);
	static_cast<CColorStone*>(pGameObject)->Set_ImageID(0);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(40.f, 4.f, -10.f);

	//pGameObject = CColorStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_01", pGameObject), E_FAIL);
	//static_cast<CColorStone*>(pGameObject)->Set_ImageID(1);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(40.f, 4.f, -10.f);

	pGameObject = CColorStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_02", pGameObject), E_FAIL);
	static_cast<CColorStone*>(pGameObject)->Set_ImageID(2);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(0.f, 4.f, -10.f);

	//pGameObject = CColorStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_03", pGameObject), E_FAIL);
	//static_cast<CColorStone*>(pGameObject)->Set_ImageID(2);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(20.f, 4.f, -10.f);

	pGameObject = CColorStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_04", pGameObject), E_FAIL);
	static_cast<CColorStone*>(pGameObject)->Set_ImageID(1);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(80.f, 4.f, -10.f);

	//pGameObject = CColorStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_05", pGameObject), E_FAIL);
	//static_cast<CColorStone*>(pGameObject)->Set_ImageID(2);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(60.f, 4.f, -10.f);
#pragma endregion

#pragma region ShootingPlant

	pGameObject = CLightFlower::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CightFlower_00", pGameObject), E_FAIL);
	static_cast<CLightFlower*>(pGameObject)->Set_ImageID(2);
	static_cast<CLightFlower*>(pGameObject)->Init_Pos(60.f, -50.f);

	//pGameObject = CShootingPlant::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShootingPlant_00", pGameObject), E_FAIL);
	//static_cast<CShootingPlant*>(pGameObject)->Set_ImageID(2);
	//static_cast<CShootingPlant*>(pGameObject)->Init_Pos(100.f, -50.f);

	//pGameObject = CShootingPlant::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShootingPlant_01", pGameObject), E_FAIL);
	//static_cast<CShootingPlant*>(pGameObject)->Set_ImageID(0);
	//static_cast<CShootingPlant*>(pGameObject)->Init_Pos(110.f, -50.f);
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
	m_mapLayer.insert({ pLayerTag, pLayer });

	pGameObject = CInvenUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Inven_UI", pGameObject), E_FAIL);
	m_mapLayer.insert({ pLayerTag, pLayer });

	pGameObject = CQuickSlot::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"QuickSlot_UI", pGameObject), E_FAIL);
	m_mapLayer.insert({ pLayerTag, pLayer });

	pGameObject = CQuestUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Quest_UI", pGameObject), E_FAIL);
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

			//������ƮŰ (������Ʈ �̸�)
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

			//�ؽ�óŰ (�ؽ�ó �̸�)
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

			//  ���� ���� ������ �ȵż� startScene�� �����ͼ� ���� �߻� 



			map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
			pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*pObjectName).c_str(), pGameObject);

			CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

			// �� ��ã�°���?;;; �� �ù� ������ ���� �ȹٲ�ſ���  �̰� ã�� ������ ����ű����̿���.
			CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", (*pObjectName).c_str(), L"Com_Transform"));
			//pTransform->ForGetWorldMaxtrix() = worldmatrix;

			////ȸ������ ���� �־��ָ� �� ( ũ�� -> ���� -> �̵� ) ������  �� �ù� ���Ű��� �����Ʈ������ �־�Z��;;
			// �����Ʈ������ �ֱ� ���� ũ�Ⱚ�� �־�����ϳ� 
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
	

	//9��25�� �浹����
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::OBJECT);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::NPC);
	CCollisionMgr::GetInstance()->CheckGroup(GROUP_TYPE::PUZZLE, GROUP_TYPE::PUZZLE);
}