#include "pch.h"
#include "MoonForestStage.h"
#include "Export_Utility.h"

CMoonForestStage::CMoonForestStage(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CScene(pGraphicDev)
{

}

CMoonForestStage::~CMoonForestStage()
{
}

HRESULT CMoonForestStage::Ready_Scene()
{

    FAILED_CHECK_RETURN(Ready_Layer_Environmnet(L"Layer_Environment"), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);


    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

    return S_OK;
}

void CMoonForestStage::LateReady_Scene()
{
	CPlayer* player = dynamic_cast<CPlayer*>(
		Get_GameObject(L"Layer_GameLogic", L"Player"));

	dynamic_cast<CTransform*>(
		player->Get_Component(ID_DYNAMIC, L"Com_Transform")
		)->Set_Pos(500.f, 30.f, 150.f);

	Engine::CScene::LateReady_Scene();
}

_int CMoonForestStage::Update_Scene(const _float& fTimeDelta)
{
    _int  iExit = Engine::CScene::Update_Scene(fTimeDelta);


    if (GetAsyncKeyState('B') & 0x8000)
    {
        Engine::CScene* pStage3 = CMoonTempleStage::Create(m_pGraphicDev);
        NULL_CHECK_RETURN(pStage3, -1);

        FAILED_CHECK_RETURN(Engine::Set_Scene(pStage3), E_FAIL);
        //pStage3->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용   

        return 0;
    }

    return iExit;
}

void CMoonForestStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CMoonForestStage::Render_Scene()
{
}

void CMoonForestStage::init()
{
    Engine::CLayer* pLayer = CLayer::Create();
    
    DWORD bytesRead = 1;
    HANDLE hFile = CreateFile(L"../Map/MoonForestMap.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        if (hFile != INVALID_HANDLE_VALUE) {
    
            Engine::CLayer* pLayer = CLayer::Create();
    
            DWORD bytesRead = 1;
    
            while (bytesRead > 0)
            {
                ObjectData* objData = new ObjectData;
    
                if (!ReadFile(hFile, objData, sizeof(ObjectData), &bytesRead, NULL))
                {
                    MSG_BOX("읽기 오류");
                };
    
                if (bytesRead == 0)
                {
                    break;
                }
    
    
                int a = 4;
    
                Engine::CGameObject* pGameObject = nullptr;
                pGameObject = CObject::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                if (pGameObject == nullptr)
                {
                    MSG_BOX("CObject nullptr Error");
                }
    
                pGameObject->SetObjectKey(objData->objectName);
                pGameObject->SetTextureKey(objData->textureKey);
    
    
                map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
                pMapLayer[L"Layer_GameLogic"]->Add_GameObject(objData->objectName, pGameObject);
    
                CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", objData->objectName, L"Com_Transform"));
    
    
                pTransform->m_vScale = { objData->scale.x,objData->scale.y,objData->scale.z };
                pTransform->Rotation(ROT_X, objData->rotation.x * 3.14f / 180.f);
                pTransform->Rotation(ROT_Y, objData->rotation.y * 3.14f / 180.f);
                pTransform->Rotation(ROT_Z, objData->rotation.z * 3.14f / 180.f);
                pTransform->Set_Pos(objData->worldMatrix._41, objData->worldMatrix._42, objData->worldMatrix._43);
    
    
                pTransform->Rotation_x = objData->rotation.x;
                pTransform->Rotation_y = objData->rotation.y;
                pTransform->Rotation_z = objData->rotation.z;
    
    
    
                pTransform->Scale_x = objData->scale.x;
                pTransform->Scale_y = objData->scale.y;
                pTransform->Scale_z = objData->scale.z;
    
    
    
            }
            CloseHandle(hFile);
        }
    }
}

HRESULT CMoonForestStage::Ready_LightInfo()
{
    return S_OK;
}

HRESULT CMoonForestStage::Ready_Layer_Environmnet(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CMoonForestStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::CGameObject* pGameObject = nullptr;

    //여기 수정해야하고
    pGameObject = CMoonForestMap::Create(m_pGraphicDev);    
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoonForestMap", pGameObject), E_FAIL);

	CGameObject* PlayerObj = pLayer->Get_GameObject(L"Layer_GameLogic", L"Player");

	_float fOffsetX = 400;
	_float fOffsetZ = 400;

#pragma region Match Puzzle

	pGameObject = CMatchPuzzle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMatchPuzzle_00", pGameObject), E_FAIL);
	CMatchPuzzle* pMatchPuzzle = static_cast<CMatchPuzzle*>(pGameObject);
	pMatchPuzzle->Push_Combination(5);
	pMatchPuzzle->Push_Combination(3);
	pMatchPuzzle->Push_Combination(4);

	pGameObject = CColorBugStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorBugStatue_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 100.f, 35.f, fOffsetZ + 100.f);
	static_cast<CColorBugStatue*>(pGameObject)->Set_ImageID(2);

	pGameObject = CColorBugStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorBugStatue_01", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 35.f, fOffsetZ + 40.f);
	static_cast<CColorBugStatue*>(pGameObject)->Set_ImageID(0);

	pGameObject = CColorBugStatue::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorBugStatue_02", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 200.f, 35.f, fOffsetZ + 40.f);
	static_cast<CColorBugStatue*>(pGameObject)->Set_ImageID(1);

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Match_CStonePedestal_00", pGameObject), E_FAIL);
	static_cast<CStonePedestal*>(pGameObject)->Init(fOffsetX + 100.f, fOffsetZ + 20.f);
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(pMatchPuzzle);
	pMatchPuzzle->Add_Pedestals(pGameObject);

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Match_CStonePedestal_01", pGameObject), E_FAIL);
	static_cast<CStonePedestal*>(pGameObject)->Init(fOffsetX, fOffsetZ);
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(pMatchPuzzle);
	pMatchPuzzle->Add_Pedestals(pGameObject);

	pGameObject = CStonePedestal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Match_CStonePedestal_02", pGameObject), E_FAIL);
	static_cast<CStonePedestal*>(pGameObject)->Init(fOffsetX + 200.f, fOffsetZ);
	static_cast<CStonePedestal*>(pGameObject)->Set_Group(pMatchPuzzle);
	pMatchPuzzle->Add_Pedestals(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Match_StoneBlock_00", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + 117.f, fOffsetZ + 250, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	pMatchPuzzle->Add_StoneBlock(pGameObject);

	pGameObject = CStoneBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Match_StoneBlock_02", pGameObject), E_FAIL);
	static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + 82.f, fOffsetZ + 250, true);
	static_cast<CStoneBlock*>(pGameObject)->Set_ImageID(1);
	pMatchPuzzle->Add_StoneBlock(pGameObject);

	pGameObject = CColorStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_00", pGameObject), E_FAIL);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 50.f, 15.f, fOffsetZ - 80.f);
	static_cast<CColorStone*>(pGameObject)->Set_ImageID(0);

	pGameObject = CColorStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_01", pGameObject), E_FAIL);
	static_cast<CColorStone*>(pGameObject)->Set_ImageID(1);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 100.f, 20.f, fOffsetZ - 80.f);

	pGameObject = CColorStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_02", pGameObject), E_FAIL);
	static_cast<CColorStone*>(pGameObject)->Set_ImageID(2);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 150.f, 20.f, fOffsetZ - 80.f);

	pGameObject = CColorStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_03", pGameObject), E_FAIL);
	static_cast<CColorStone*>(pGameObject)->Set_ImageID(0);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 50.f, 20.f, fOffsetZ - 160.f);

	pGameObject = CColorStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_04", pGameObject), E_FAIL);
	static_cast<CColorStone*>(pGameObject)->Set_ImageID(1);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 100.f, 20.f, fOffsetZ - 160.f);

	pGameObject = CColorStone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColorStone_05", pGameObject), E_FAIL);
	static_cast<CColorStone*>(pGameObject)->Set_ImageID(2);
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 150.f, 20.f, fOffsetZ - 160.f);


#pragma endregion
#pragma region Stone Puzzle

	//_float fOffsetX = 400;
	//_float fOffsetZ = 230;

	//pGameObject = CMonkeyStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyStatue_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 22.f, fOffsetZ);
	//CMonkeyStatue* pMS = static_cast<CMonkeyStatue*>(pGameObject);
	//pMS->SetPlayer(static_cast<CPlayer*>(PlayerObj));

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_00", pGameObject), E_FAIL);
	//static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX , fOffsetZ + (30.f * 4.f), true);
	//pMS->Add_StoneBlock(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_01", pGameObject), E_FAIL);
	//static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 3.f), fOffsetZ + (30.f * 6.f), false);
	//pMS->Add_StoneBlock(pGameObject);

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ);

	//pGameObject = CMonkeyStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyStatue_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 2), 22.f, fOffsetZ);
	//pMS = static_cast<CMonkeyStatue*>(pGameObject);
	//pMS->SetPlayer(static_cast<CPlayer*>(PlayerObj));

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_02", pGameObject), E_FAIL);
	//static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 2), fOffsetZ + (30.f), true);
	//pMS->Add_StoneBlock(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_03", pGameObject), E_FAIL);
	//static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX , fOffsetZ + (30.f), false);
	//pMS->Add_StoneBlock(pGameObject);

	//pGameObject = CStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 16.f, fOffsetZ + (30.f * 3.f));

	//pGameObject = CStoneHole::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 5.f), 0.2f, fOffsetZ);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5.f), 0.2f, fOffsetZ);

	//pGameObject = CMonkeyStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyStatue_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7.f), 22.f, fOffsetZ);
	//pMS = static_cast<CMonkeyStatue*>(pGameObject);
	//pMS->SetPlayer(static_cast<CPlayer*>(PlayerObj));

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_04", pGameObject), E_FAIL);
	//static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 5.f), fOffsetZ + (30.f * 2.f), false);
	//pMS->Add_StoneBlock(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_05", pGameObject), E_FAIL);
	//static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX, fOffsetZ + (30.f * 2.f), true);
	//pMS->Add_StoneBlock(pGameObject);

	//pGameObject = CStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 4.f));

	//pGameObject = CStoneHole::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 7.f), 0.2f, fOffsetZ + (30.f));
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7.f), 0.2f, fOffsetZ + (30.f));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 3));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_03", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 3.f));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_04", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 2.f));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_05", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 3.f));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_06", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 3.f));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_07", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 5.f));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_08", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 4), 16.f, fOffsetZ + (30.f * 5.f));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_09", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 5.f));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_10", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f));

	//pGameObject = CBreakableStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BreakableStone_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 4.f));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_11", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 6));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_12", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 7));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_13", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f), 16.f, fOffsetZ + (30.f * 8));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_14", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 7));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_15", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 8));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_16", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 16.f, fOffsetZ + (30.f * 9));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_17", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 16.f, fOffsetZ + (30.f * 8));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_18", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6), 16.f, fOffsetZ + (30.f * 8));

	//pGameObject = CRock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Rock_19", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 16.f, fOffsetZ + (30.f * 8));

	//pGameObject = CMonkeyStatue::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MonkeyStatue_03", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 22.f, fOffsetZ + (30.f * 8.f));
	//pMS = static_cast<CMonkeyStatue*>(pGameObject);
	//pMS->SetPlayer(static_cast<CPlayer*>(PlayerObj));

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_06", pGameObject), E_FAIL);
	//static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 2), fOffsetZ + (30.f * 8.f), false);
	//pMS->Add_StoneBlock(pGameObject);

	//pGameObject = CStoneBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneBlock_07", pGameObject), E_FAIL);
	//static_cast<CStoneBlock*>(pGameObject)->Init(fOffsetX + (30.f * 4), fOffsetZ + (30.f * 8.f), true);
	//pMS->Add_StoneBlock(pGameObject);

	//pGameObject = CStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5.f), 16.f, fOffsetZ + (30.f * 6.f));

	//pGameObject = CStoneHole::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX, 0.2f, fOffsetZ + (30.f * 7.f));
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 0.2f, fOffsetZ + (30.f * 7.f));

	//pGameObject = CStoneHole::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StoneHole_03", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(fOffsetX + (30.f * 3), 0.2f, fOffsetZ + (30.f * 4.f));
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 3), 0.2f, fOffsetZ + (30.f * 4.f));

	//pGameObject = CCrystalPuzzle::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrystalPuzzle", pGameObject), E_FAIL);
	//CCrystalPuzzle* pCrystalPuzzle = static_cast<CCrystalPuzzle*>(pGameObject);

	//pGameObject = CCrystal::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_00", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6.f), 18.f, fOffsetZ + (30.f * 3.f));
	//pCrystalPuzzle->Add_Crystal(pGameObject);

	//pGameObject = CPressBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_00", pGameObject), E_FAIL);
	//static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 6), fOffsetZ + (30.f * 4));
	//pCrystalPuzzle->Add_PressBlock(pGameObject);

	//pGameObject = CCrystal::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_01", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 7), 18.f, fOffsetZ + (30.f * 3));
	//pCrystalPuzzle->Add_Crystal(pGameObject);

	//pGameObject = CPressBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_01", pGameObject), E_FAIL);
	//static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 7), fOffsetZ + (30.f * 4));
	//pCrystalPuzzle->Add_PressBlock(pGameObject);

	//pGameObject = CCrystalPuzzle::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrystalPuzzle_01", pGameObject), E_FAIL);
	//pCrystalPuzzle = static_cast<CCrystalPuzzle*>(pGameObject);

	//pGameObject = CCrystal::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_02", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 6.f), 18.f, fOffsetZ + (30.f * 6.f));
	//pCrystalPuzzle->Add_Crystal(pGameObject);

	//pGameObject = CPressBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_02", pGameObject), E_FAIL);
	//static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 4), fOffsetZ + (30.f * 6));
	//pCrystalPuzzle->Add_PressBlock(pGameObject);

	//pGameObject = CCrystal::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_03", pGameObject), E_FAIL);
	//static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + (30.f * 5), 18.f, fOffsetZ + (30.f * 7));
	//pCrystalPuzzle->Add_Crystal(pGameObject);

	//pGameObject = CPressBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_03", pGameObject), E_FAIL);
	//static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + (30.f * 4), fOffsetZ + (30.f * 7));
	//pCrystalPuzzle->Add_PressBlock(pGameObject);

#pragma endregion


    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CMoonForestStage::Ready_Layer_UI(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CDefaultUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Default_UI", pGameObject), E_FAIL);

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

	pGameObject = CMapName::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CMapName*>(pGameObject)->Set_MapName(L"달의 숲");
	dynamic_cast<CMapName*>(pGameObject)->CallName();
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMapName", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CMoonForestStage* CMoonForestStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMoonForestStage* pMoonForestStage = new CMoonForestStage(pGraphicDev); 

    return pMoonForestStage;
}

void CMoonForestStage::Free()
{
    Engine::CScene::Free();
}
